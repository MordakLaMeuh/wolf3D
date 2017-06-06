/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 01:01:36 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 01:01:38 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		view_map(t_tile **map, int width, int height)
{
	int i;
	int j;

	ft_printf("{red}map_x = %i, map_y = %i{eoc}\n", width, height);
	i = 0;
	while (i < height)
	{
		j = 0;
		while (j < width)
		{
			ft_printf("{green}%i{eoc} ", map[i][j]);
			j++;
		}
		ft_printf("\n");
		i++;
	}
}
