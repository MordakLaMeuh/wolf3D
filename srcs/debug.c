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

#include <stdlib.h>
#include <sys/time.h>
#include "wolf3d.h"

void			view_map(t_tile **map, int width, int height)
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

void			eval_fps(t_env *e)
{
	static int				count = 0;
	static struct timeval	start;
	static char				*s = NULL;
	struct timeval			stop;

	if (count == 0)
	{
		gettimeofday(&start, NULL);
		count++;
	}
	else
	{
		gettimeofday(&stop, NULL);
		if ((stop.tv_sec - start.tv_sec) == 0)
			count++;
		else
		{
			if (s)
				free(s);
			s = ft_itoa(count);
			count = 0;
		}
	}
	mlx_string_put(e->mlx, e->win, 20, 20, 0x00FFFFFF, "FPS:");
	mlx_string_put(e->mlx, e->win, 70, 20, 0x00FFFFFF, (s) ? s : "");
}
