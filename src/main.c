/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtimoshy <dtimoshy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 16:44:02 by dtimoshy          #+#    #+#             */
/*   Updated: 2018/10/29 16:44:03 by dtimoshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

const t_ssl_mode		g_mode_change[] = {
	{ "md5", &md5},
	{"sha256", &sha256},
	{"sha224", &sha224},
	{"sha512", &sha512},
	{"sha384", &sha384},
	{ NULL, NULL}
};

static t_ssl_mode		get_mode(char **argv)
{
	int i;

	i = 0;
	while (g_mode_change[i].name)
	{
		if (ft_strequ(argv[0], g_mode_change[i].name))
			break ;
		i++;
	}
	if (g_mode_change[i].name == NULL)
		ft_mode_error(argv[0]);
	return (g_mode_change[i]);
}

static void				start_ssl(int argc, char **argv)
{
	t_ssl_md5	*handler;
	t_ssl_mode	temp;

	temp = get_mode(argv);
	handler = (t_ssl_md5 *)malloc(sizeof(t_ssl_md5));
	handler->f = temp.f;
	handler->name = ft_strdup(temp.name);
	handler->q = 0;
	handler->r = 0;
	handler->p = 0;
	handler->stdin_write = 0;
	parse_arguments(handler, argv, argc);
	ft_strdel(&handler->name);
	system("leaks -q ft_ssl");
	free(handler);
}

static void				no_arguments(void)
{
	char	*str;
	char	**split;
	int		argc;
	int		i;

	ft_printf("ft_ssl> ");
	while (get_next_line(0, &str))
	{
		i = -1;
		argc = 0;
		if (ft_strequ(str, "exit"))
			exit(0);
		split = ft_strsplit(str, ' ');
		while (str && str[++i])
			if ((str[i] == ' ' && str[i + 1] != ' ' && str[i + 1])
				|| (i == 0 && str[i] != ' '))
				argc++;
		if (str)
			start_ssl(argc, split);
		ft_strdel(&str);
		ft_printf("ft_ssl> ");
	}
}

int						main(int argc, char **argv)
{
	if (argc == 1)
		no_arguments();
	else
		start_ssl(argc - 1, &argv[1]);
	return (0);
}
