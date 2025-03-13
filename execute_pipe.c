/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 23:51:31 by michel            #+#    #+#             */
/*   Updated: 2025/03/13 16:17:58 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int *create_pipes(int n)
{
    int *pipes;
    int i;

    if (n < 2)
        return (NULL);
    pipes = malloc(2 * (n - 1) * sizeof(int));
    if (!pipes)
        return (NULL);
    i = 0;
    while (i < n - 1)
    {
        if (pipe(&pipes[i * 2]) < 0)
        {
            free(pipes);
            return (NULL);
        }
        i++;
    }
    return (pipes);
}

int execute_child(t_command *cmd, int index, t_pipe_info *pi, t_data *data)
{
    int j;

    /* Rediriger l'entrée pour toutes sauf la première commande */
    if (index > 0)
    {
        if (dup2(pi->pipes[(index - 1) * 2], STDIN_FILENO) < 0)
            exit(1);
    }
    /* Rediriger la sortie pour toutes sauf la dernière commande */
    if (index < pi->n - 1)
    {
        if (dup2(pi->pipes[index * 2 + 1], STDOUT_FILENO) < 0)
            exit(1);
    }
    /* Fermer tous les descripteurs du pipeline */
    j = 0;
    while (j < 2 * (pi->n - 1))
    {
        close(pi->pipes[j]);
        j++;
    }
    if (handle_redirection(cmd) < 0)
        exit(1);
    if (is_builtins(cmd->args[0]))
        exit(execute_builtin(cmd->args, data));
    {
        char *exec_path;
        if (ft_strchr(cmd->args[0], '/') != NULL)
            exec_path = ft_strdup(cmd->args[0]);
        else
            exec_path = find_excutable(cmd->args[0]);
        if (!exec_path)
        {
            ft_putstr_fd("commande pas trouver\n", 2);
            exit(127);
        }
        // char **env_array = env_to_array(env);
        // print_env_array(env_array);
        execve(exec_path, cmd->args, env_to_array(data->env));
        perror("execve");
        exit(126);
    }
}

int fork_pipeline(t_command *pipeline, t_pipe_info *pi, t_data *data)
{
    int i = 0;
    t_command *cmd = pipeline;
    pid_t pid;
    int status, last_status = 0;

    while (cmd)
    {
        pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return (-1);
        }
        if (pid == 0)
        {
            fprintf(stderr, "[debuger] Enfant %d lancé pour commande: %s\n", i, cmd->args[0]);
            execute_child(cmd, i, pi, data);
        }
        cmd = cmd->next_pipe;
        i++;
    }
    if (pi->pipes)
    {
        i = 0;
        while (i < 2 * (pi->n - 1))
        {
            close(pi->pipes[i]);
            i++;
        }
    }
    i = 0;
    while (i < pi->n)
    {
        wait(&status);
        last_status = status;
        i++;
    }
    return (WEXITSTATUS(last_status));
}

int execute_pipeline(t_command *pipeline, t_env *env)
{
    ft_putstr_fd("rentre dans execute_pipeline\n", 1);
    int n = count_commands(pipeline);
    t_pipe_info pi;
    int status;

    pi.n = n;
    pi.pipes = create_pipes(n);
    if (n > 1 && !pi.pipes)
        return (-1);
    status = fork_pipeline(pipeline, &pi, env);
    if (pi.pipes)
        free(pi.pipes);
    return status;
}

void print_env_array(char **env_array)
{
    int i = 0;
    if (!env_array)
    {
        printf("env_array est NULL\n");
        return;
    }
    while (env_array[i])
    {
        printf("%s\n", env_array[i]);
        i++;
    }
}
