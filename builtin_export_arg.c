/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_arg.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mm-furi <mm-furi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 17:35:50 by nicolsan          #+#    #+#             */
/*   Updated: 2025/05/15 14:48:50 by mm-furi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_export_arguments(char **args, t_data *data)
{
	int	i;
	int	ret_status;

	ret_status = 0;
	if (!args || !args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		if (process_export_argument(args[i], data) != 0)
			ret_status = 1;
		i++;
	}
	return (ret_status);
}

int	process_export_argument(char *arg, t_data *data)
{
	t_export_kv	kv;

	kv.key = NULL;
	kv.raw_value = NULL;
	kv.eq = NULL;
	kv.append = NULL;
	if (!extract_export_kv(arg, &kv))
		return (1);
	if (!validate_export_key(kv.key, arg))
	{
		free(kv.key);
		free(kv.raw_value);
		return (1);
	}
	return (process_and_update_env(data, &kv));
}

char	*extract_key(char *arg, char *eq)
{
	size_t	key_len;

	key_len = eq - arg;
	return (ft_substr(arg, 0, key_len));
}

char	*extract_value(char *eq)
{
	return (ft_strdup(eq + 1));
}

/*a garder au cas ou j'ai 2 versions
je sais plus laquelle est bonne ,
si il y a des erreures cest p-e celle là la bonne*/
/*
int	handle_export_arguments(char **args, t_data *data)
{
  int i;

  i = 1;
  while (args[i])
  {
	if (process_export_argument(args[i], data))
		return (1);
	i++;
  }
  return (0);
}
*/
