/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_player_location.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 04:32:50 by bmickael          #+#    #+#             */
/*   Updated: 2017/07/05 04:32:52 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/coord_types.h"
#include "parse/internal_parse.h"

int						get_player_location(t_coord_f *l, char c)
{
	t_list	*tmp;
	char	*content;

	tmp = get_map_content()->data;
	l->y = 0;
	while (tmp)
	{
		content = tmp->content;
		l->x = 0;
		while (*content)
		{
			if (content[1] == c)
				return (0);
			content += (content[2] != '\0') ? 3 : 2;
			l->x += 1;
		}
		l->y += 1;
		tmp = tmp->next;
	}
	return (-1);
}
