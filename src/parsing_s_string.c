/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_s_string.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtimoshy <dtimoshy@student.unit.ua>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 16:28:11 by dtimoshy          #+#    #+#             */
/*   Updated: 2018/10/29 16:44:49 by dtimoshy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ssl_md5.h"

static int		smth_after_s(t_ssl_md5 *handler, char *argv,
				int k, t_content *string)
{
	if (!handler->q && !handler->r)
		ft_printf("%s (\"%s\") = ", ft_strto(handler->name, 1), &argv[k + 1]);
	string->content = (unsigned char *)&argv[k + 1];
	string->cont_len = ft_strlen(&argv[k + 1]) * 8;
	handler->f(string);
	if (!handler->q && handler->r)
		ft_printf(" \"%s\"", &argv[k + 1]);
	k = k + (int)ft_strlen(&argv[k + 1]);
	return (k);
}

void			parse_s_string(t_ssl_md5 *handler, char **argv, int *i, int *k)
{
	t_content *string;

	string = (t_content *)malloc(sizeof(t_content));
	if (argv[*i][*k + 1])
		*k = smth_after_s(handler, argv[*i], *k, string);
	else
	{
		string->content = (unsigned char *)argv[++(*i)];
		string->cont_len = ft_strlen(argv[*i]) * 8;
		if (!handler->q && !handler->r)
			ft_printf("%s (\"%s\") = ", ft_strto(handler->name, 1), argv[*i]);
		handler->f(string);
		if (!handler->q && handler->r)
			ft_printf(" \"%s\"", argv[*i]);
		(*k) = (int)ft_strlen(argv[*i]) - 1;
	}
	free(string);
	ft_printf("\n");
}
