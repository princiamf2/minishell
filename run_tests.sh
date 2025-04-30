#!/usr/bin/env bash

set -uo pipefail

# --- Configuration ---
LOG_FILE="tests.log"
TEST_FILE="tests.txt"
MINISHELL_BIN="./minishell"
END_MARKER="__TEST_END__"
READ_TIMEOUT=5

# --- Initialisation ---
# Vide le fichier log au début
: >"$LOG_FILE"

# --- Variables Globales ---
MS_PID=""
PIPE_IN=""
PIPE_OUT=""
FD_IN=-1
FD_OUT=-1

# --- Fonction de Nettoyage ---
# Ne log plus dans le fichier, seulement sur stderr en cas d'erreur éventuelle
cleanup(){
    local exit_code=$?
    # Arrêter le processus minishell s'il est en cours
    if [[ -n "$MS_PID" ]] && kill -0 "$MS_PID" 2>/dev/null; then
        kill -9 "$MS_PID" 2>/dev/null || true # Tuer directement
        wait "$MS_PID" 2>/dev/null || true
    fi
    MS_PID=""

    # Fermer les descripteurs de fichiers des pipes
    [[ "$FD_IN" -ne -1 ]] && exec {FD_IN}>&- 2>/dev/null || true
    [[ "$FD_OUT" -ne -1 ]] && exec {FD_OUT}<&- 2>/dev/null || true
    FD_IN=-1
    FD_OUT=-1

    # Supprimer les pipes nommés s'ils existent
    [[ -n "$PIPE_IN" ]] && [ -p "$PIPE_IN" ] && rm -f "$PIPE_IN"
    [[ -n "$PIPE_OUT" ]] && [ -p "$PIPE_OUT" ] && rm -f "$PIPE_OUT"
    PIPE_IN=""
    PIPE_OUT=""
}
trap cleanup EXIT INT TERM

# --- Fonction pour démarrer/redémarrer minishell avec mkfifo ---
start_minishell_fifo(){
    cleanup

    # Créer des noms uniques pour les pipes
    local timestamp
    timestamp=$(date +%s%N)
    PIPE_IN="/tmp/minishell_pipe_in_${timestamp}_$$"
    PIPE_OUT="/tmp/minishell_pipe_out_${timestamp}_$$"

    # Créer les pipes nommés
    if ! mkfifo "$PIPE_IN" || ! mkfifo "$PIPE_OUT"; then
        echo "ERREUR CRITIQUE: Impossible de créer les pipes nommés dans /tmp/." >&2
        exit 1
    fi

    # Lancer minishell en arrière-plan, redirigeant stdin/stdout/stderr vers les pipes
    "$MINISHELL_BIN" <"$PIPE_IN" >"$PIPE_OUT" 2>&1 &
    MS_PID=$!

    # Donner un instant et vérifier s'il est vivant
    sleep 0.2
    if ! kill -0 "$MS_PID" 2>/dev/null; then
        echo "ERREUR CRITIQUE: '$MINISHELL_BIN' a échoué à démarrer ou a quitté immédiatement (PID $MS_PID)." >&2
        # Essayer de lire une erreur potentielle du pipe de sortie avant de quitter
        echo "--- Début de la sortie d'erreur potentielle de minishell ---" >&2
        head -n 5 "$PIPE_OUT" >&2 2>/dev/null || true
        echo "--- Fin de la sortie d'erreur potentielle ---" >&2
        exit 1
    fi

    # Ouvrir les pipes depuis le script pour communication
    exec {FD_IN}>"$PIPE_IN"
    if [[ $? -ne 0 ]]; then
       echo "ERREUR CRITIQUE: Impossible d'ouvrir le pipe d'entrée '$PIPE_IN' en écriture." >&2
       exit 1
    fi
    exec {FD_OUT}<"$PIPE_OUT"
     if [[ $? -ne 0 ]]; then
       echo "ERREUR CRITIQUE: Impossible d'ouvrir le pipe de sortie '$PIPE_OUT' en lecture." >&2
       exit 1
    fi
}

# --- Vérifications initiales (sortie sur stderr) ---
if [ ! -x "$MINISHELL_BIN" ]; then
    echo "ERREUR: Le fichier '$MINISHELL_BIN' n'existe pas ou n'est pas exécutable." >&2
    exit 1
fi
if [ ! -f "$TEST_FILE" ]; then
    echo "ERREUR: Le fichier de tests '$TEST_FILE' n'a pas été trouvé." >&2
    exit 1
fi

# --- Boucle Principale des Tests ---
start_minishell_fifo

line_num=0
while IFS= read -r cmd || [[ -n "$cmd" ]]; do
    ((line_num++))
    [[ -z "$cmd" || "$cmd" =~ ^\s*# ]] && continue

    # Vérifier si minishell est vivant AVANT d'envoyer
    if ! kill -0 "$MS_PID" 2>/dev/null; then
        echo "ATTENTION: Minishell (ancien PID $MS_PID) était mort. Redémarrage..." >&2 # Info sur stderr
        start_minishell_fifo
    fi

    # Envoyer la commande via le pipe d'entrée
    if ! printf '%s\n' "$cmd" >&"$FD_IN"; then
        echo "ERREUR: Échec de l'écriture de la commande vers minishell (PID $MS_PID)." >&2
        continue
    fi

    # Envoyer la commande pour le marqueur
    if ! printf 'echo %s\n' "$END_MARKER" >&"$FD_IN"; then
        echo "ERREUR: Échec de l'écriture du marqueur vers minishell (PID $MS_PID)." >&2
        continue
    fi

    # Lire la sortie depuis le pipe de sortie
    marker_found=false
    while true; do
        if ! IFS= read -r -t "$READ_TIMEOUT" line <&"$FD_OUT"; then
            read_status=$?
            # Afficher erreurs/timeout sur stderr, pas dans le log
            if [[ $read_status -gt 128 ]]; then
                echo "ATTENTION: TIMEOUT ($READ_TIMEOUT s) en attendant la sortie pour '$cmd' (PID $MS_PID)." >&2
            elif [[ $read_status -ne 0 ]]; then # EOF ou autre erreur read
                 echo "ATTENTION: Erreur/EOF ($read_status) en lisant la sortie pour '$cmd' (PID $MS_PID)." >&2
            fi
            # Quitter la boucle de lecture si read échoue
            # Vérifier si minishell est mort *après* l'échec de lecture
             if ! kill -0 "$MS_PID" 2>/dev/null; then
                 echo "INFO: Minishell (PID $MS_PID) est mort après l'échec de lecture." >&2
             fi
            break
        fi

        # Vérifier si c'est le marqueur
        if [[ "$line" == "$END_MARKER" ]]; then
            marker_found=true
            break # Sortir de la boucle, commande terminée
        else
            # *** LA SEULE CHOSE ÉCRITE DANS LE LOG FILE ***
            printf '%s\n' "$line" >> "$LOG_FILE"
        fi
    done

    # Optionnel: Avertir sur stderr si le marqueur n'a pas été trouvé
    # if [[ "$marker_found" == false ]]; then
    #      echo "ATTENTION: Marqueur non reçu pour la commande '$cmd'." >&2
    # fi

done < "$TEST_FILE"

# --- Nettoyage Final Spécifique ---
# Le trap cleanup s'occupera d'arrêter minishell et de supprimer les pipes

# Liste EXPLICITE des éléments à supprimer. Ajout de '&'.
cleanup_items=(
    "testdir" "subdir" "not_a_dir" "no_perms"
    "outfile.txt" "appendfile.txt" "emptyfile.txt" "infile.txt"
    "out.txt" "outfile2.txt" "sub_out.txt" "existing_dir"
    "testglob" "a" "b" "file1.txt" "file2.log"
    "test.sh" "no_exec"
    # Ajout du fichier '&'
    "&"
)
items_removed_count=0
for item in "${cleanup_items[@]}"; do
    if [ -e "./$item" ]; then
        rm -rf -- "./$item"
        ((items_removed_count++))
    fi
done

# --- Message Final (sur le terminal) ---
echo "Tests terminés – voir $LOG_FILE"

exit 0