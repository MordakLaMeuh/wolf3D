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

#include <sys/time.h>
#include <stdio.h>
#include <math.h>
#include "wolf3d.h"

void		init_minimap(t_env *e)
{
	draw_minimap(e);
}

void		draw_minimap(t_env *e)
{
	struct timespec	spec;
	t_coord_i		l1;
	t_pix			color;
	float			angle;
	t_line			line;

	l1 = (t_coord_i){MAP_ORIGIN_X, MAP_ORIGIN_Y};
	draw_arrow(e, l1, e->player.angle);
	color.i = 0xffffff;
	draw_circle(e, l1, MAP_RADIUS, color);
	clock_gettime(CLOCK_REALTIME, &spec);
	angle = e->player.angle + (float)(
		(((((int)spec.tv_sec & 0x0F) * 1000) +
		round(spec.tv_nsec / 1.0e6)) / 2000) * M_PI * 2);
	line.p1 = l1;
	line.p2.x = (floor)(cosf(angle) * MAP_RADIUS) + MAP_ORIGIN_X;
	line.p2.y = (floor)(sinf(angle) * MAP_RADIUS) + MAP_ORIGIN_Y;
	line.b_pix.i = 0xFFFF00;
	line.f_pix.i = 0x00FFFF;
	draw_line(e, &line);
}
