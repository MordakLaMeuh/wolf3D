/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 01:30:50 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 01:30:52 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include "wolf3d.h"

#include <stdio.h>

void		init_minimap(t_env *e)
{
	if (e->map.size.x <= 10 && e->map.size.y <= 10)
	{
		e->map.scale = (e->map.size.x < e->map.size.y) ?
												e->map.size.y : e->map.size.x;
		e->map.scale = (float)10 / e->map.scale;
	}
	printf("map scale = %f\n", e->map.scale);
	draw_minimap(e);
}

void		draw_minimap(t_env *e)
{
	t_coord_i		l1;
	t_coord_i		l2;
	t_pix			pix;
//	t_coord_f		offset;

	l1.x = MAP_ORIGIN_X;
	l1.y = MAP_ORIGIN_Y;
	l2.x = MAP_ORIGIN_X + MAP_GLOBAL_SIZE;
	l2.y = MAP_ORIGIN_Y + MAP_GLOBAL_SIZE;
	pix.i = 0x00ff00;
	draw_box(l1, l2, pix, e);

	t_line line;
	float tmp;
	int i;
	i = 0;
	line.b_pix.i = 0xff;
	line.f_pix.i = 0xff;
	line.p1.x = MAP_ORIGIN_X;
	line.p1.y = MAP_ORIGIN_Y;
	line.p2.x = MAP_ORIGIN_X;
	line.p2.y = MAP_ORIGIN_Y + MAP_GLOBAL_SIZE;
	tmp = MAP_ORIGIN_X;
	while (i < MAP_TILE_N)
	{
		tmp += (float)TILE_SIZE;
		line.p1.x = tmp;
		line.p2.x = tmp;
		draw_line(e, &line);
		i++;
	}

	i = 0;
	line.p1.x = MAP_ORIGIN_X;
	line.p1.y = MAP_ORIGIN_Y;
	line.p2.x = MAP_ORIGIN_X + MAP_GLOBAL_SIZE;
	line.p2.y = MAP_ORIGIN_Y;
	tmp = MAP_ORIGIN_Y;
	while (i < MAP_TILE_N)
	{
		tmp += (float)TILE_SIZE;
		line.p1.y = tmp;
		line.p2.y = tmp;
		draw_line(e, &line);
		i++;
	}

	l1.x = MAP_ORIGIN_X + (MAP_GLOBAL_SIZE / 2);
	l1.y = MAP_ORIGIN_Y + (MAP_GLOBAL_SIZE / 2);

	draw_arrow(e, l1, e->player.angle, WIDTH / 1980.f);



/*
	offset.x = (e->player.location.x / e->map.size.x);
	offset.y = (e->player.location.y / e->map.size.y);
	l1.x = (X_MAP_CENTER) - MAP_SEMI_LENGTH + 20 + (int)(offset.x * MAP_LENGTH) - 40;
	l1.y = (Y_MAP_CENTER) - MAP_SEMI_LENGTH + 20 + (int)(offset.y * MAP_LENGTH) - 40;
	draw_arrow(e, l1, e->player.angle, WIDTH / 1980.f);
*/
}
