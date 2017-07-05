/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   constructor.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 04:32:26 by bmickael          #+#    #+#             */
/*   Updated: 2017/07/05 04:32:29 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parse/internal_parse.h"

static t_map_content	*g_content = NULL;

void					alloc_map_content(void)
{
	if (g_content != NULL)
	{
		ft_eprintf("{green}parse:{eoc} content already allocated\n");
		exit(EXIT_FAILURE);
	}
	if (!(g_content = (t_map_content *)ft_memalloc(sizeof(t_map_content))))
		exit(EXIT_FAILURE);
}

void					free_map_content(void)
{
	t_list		*lst;
	t_list		*next;

	if (g_content == NULL)
	{
		ft_eprintf("{green}parse:{eoc} content wasn't allocated\n");
		exit(EXIT_FAILURE);
	}
	if (g_content->data)
	{
		lst = g_content->data;
		while (lst)
		{
			next = lst->next;
			if (lst->content)
				free(lst->content);
			free(lst);
			lst = next;
		}
	}
	free(g_content);
}

t_map_content			*get_map_content(void)
{
	if (g_content == NULL)
	{
		ft_eprintf("{green}parse:{eoc} cannot get content of NULL\n");
		exit(EXIT_FAILURE);
	}
	return (g_content);
}
