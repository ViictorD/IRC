/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_connect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarmaya <vdarmaya@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/08 13:50:23 by vdarmaya          #+#    #+#             */
/*   Updated: 2018/09/16 20:12:20 by vdarmaya         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.h"

void		connect_server(t_client *client)
{
	struct protoent		*pe;
	struct sockaddr_in	addr;
	socklen_t			len;

	if (!(pe = getprotobyname("tcp")))
	{
		ft_fputstr("Fail to get the protocol name.\n", 2);
		return ;
	}
	if ((client->fd = socket(PF_INET, SOCK_STREAM, pe->p_proto)) < 0)
	{
		ft_fputstr("Failed to create server socket.\n", 2);
		return ;
	}
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = client->address;
	addr.sin_port = client->port;
	len = sizeof(addr);
	if ((connect(client->fd, (struct sockaddr*)&addr, len)) < 0)
	{
		ft_fputstr("Failed to connect to the server.\n", 2);
		clear_client(client);
	}
}

static char	get_port(t_client *client, char *arg)
{
	int		tmp;

	if (!arg)
		client->port = htons(DEF_PORT);
	else if (arg)
	{
		if (!(tmp = ft_atoi(arg)))
		{
			connect_usage();
			free(arg);
			return (1);
		}
		client->port = htons(tmp);
	}
	return (0);
}

void		manage_connect(char *str, t_client *client)
{
	char			*arg;
	struct in_addr	addr;

	arg = get_next_word(&str);
	if (!arg)
	{
		connect_usage();
		return ;
	}
	clear_client(client);
	if (!inet_aton(arg, &addr))
	{
		connect_usage();
		free(arg);
		return ;
	}
	client->address = addr.s_addr;
	free(arg);
	arg = get_next_word(&str);
	if (get_port(client, arg))
		return ;
	arg ? free(arg) : NULL;
	connect_server(client);
}
