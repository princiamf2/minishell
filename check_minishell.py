#!/usr/bin/env python3
import subprocess
import sys


def run(cmd, input=None, timeout=5):
    """Exécute une commande shell et retourne (code_retour, stdout, stderr)."""
    try:
        result = subprocess.run(
            cmd,
            input=input,
            text=True,
            capture_output=True,
            timeout=timeout,
            shell=True
        )
        return result.returncode, result.stdout.strip(), result.stderr.strip()
    except subprocess.TimeoutExpired:
        return 1, "", f"Timeout après {timeout}s : {cmd}"


def check_make():
    code, out, err = run("make")
    if code != 0:
        return False, f"Échec de 'make' :\n{err or out}"
    return True, ""


def check_norminette():
    code, out, err = run("norminette .")
    if code != 0:
        return False, f"Erreurs Norminette :\n{out or err}"
    return True, ""


def check_valgrind():
    # envoie 'exit' via stdin pour détecter les fuites
    code, out, err = run(
        "valgrind --leak-check=full --error-exitcode=1 ./minishell",
        input="exit\n",
        timeout=20
    )
    if code != 0:
        return False, f"Valgrind a détecté des problèmes :\n{err}"
    return True, ""


def check_functional(test_desc, script):
    # prépare l'entrée avec un saut de ligne
    input_data = script + "\n"
    # exécution sous bash
    code_b, out_b, err_b = run("bash", input=input_data)
    # exécution sous minishell
    code_m, out_m, err_m = run("./minishell", input=input_data)
    # nettoyage de la sortie minishell (supprime prompts et vides)
    lines = out_m.splitlines()
    filtered = []
    for line in lines:
        if line.startswith('minishell$>'):
            continue
        if line.startswith('herdoc>') or line.startswith('dquotes>'):
            continue
        if line.strip() == '':
            continue
        filtered.append(line)
    out_m_clean = '\n'.join(filtered)
    # comparaison
    if code_b != code_m or out_b.strip() != out_m_clean.strip():
        msg = (f"Test '{test_desc}' échoué :\n"
               f"Attendu (bash code {code_b}) : {out_b.strip()}\n"
               f"Obtenu  (minishell code {code_m}) : {out_m_clean.strip()}")
        return False, msg
    return True, ""


def main():
    controles = [
        ("Make", check_make),
        ("Norminette", check_norminette),
        ("Valgrind", check_valgrind),
    ]

    tests_fonctionnels = [
        ("echo simple", "echo bonjour"),
        ("pipeline et code de sortie", "echo foo | grep f"),
        ("redir. écrasement", "rm -f fichier\necho bar > fichier\ncat fichier"),
        ("redir. ajout", "rm -f fichier\necho a >> fichier\necho b >> fichier\ncat fichier"),
        ("heredoc", "cat << EOF\nligne1\nEOF"),
        ("expansion variable", "export VAR=42\necho $VAR"),
        ("expansion arithmétique", "echo $((1+2*3))"),
        ("dollar question", "false\necho $?"),
        ("et ou logique", "true && echo ok\nfalse || echo fallback"),
        ("sous-shell", "(echo sous)"),
        ("joker", "mkdir -p tmp_test && cd tmp_test\ntouch t1 t2\necho t*\ncd ..\nrm -rf tmp_test"),
    ]

    for desc, script in tests_fonctionnels:
        controles.append((f"Fonctionnel : {desc}", lambda sc=script, de=desc: check_functional(de, sc)))

    erreurs = []
    for nom, fonction in controles:
        ok, msg = fonction()
        if not ok:
            erreurs.append(f"[{nom}] {msg}")

    if not erreurs:
        print("OK : tout est valide !")
        sys.exit(0)
    else:
        print("Erreurs détectées :")
        for e in erreurs:
            print(e)
        sys.exit(1)

if __name__ == "__main__":
    main()
