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

/*
** TODO minimap en chantier
** doit absolument etre optimisee !
*/

static void		draw_enemy(t_env *e, t_coord_i l)
{
	t_pix pix;

	pix.i = 0xff0000;
	fill_box((t_coord_i){l.x - 3, l.y - 3},
			(t_coord_i){l.x + 3, l.y + 3}, pix, e);
}

static float	get_distance(t_coord_f p1, t_coord_f p2)
{
	float dx;
	float dy;

	dx = p1.x - p2.x;
	dy = p1.y - p2.y;
	return (sqrtf((dx * dx) + (dy * dy)));
}

static void		locate_enemy(t_env *e)
{
	t_coord_f	l;
	int			i;

	i = 0;
	while (i < e->n_sprites)
	{
		if (get_distance(e->player.location, e->sprites[i].location) <
											(MAP_DEPTH - 1))
		{
			l.x = e->player.location.x - e->sprites[i].location.x;
			l.y = e->player.location.y - e->sprites[i].location.y;
			l.x = -l.x;
			l.y = -l.y;
			l.x *= (MAP_RADIUS / MAP_DEPTH);
			l.y *= (MAP_RADIUS / MAP_DEPTH);
			l.x += MAP_ORIGIN_X;
			l.y += MAP_ORIGIN_Y;
			draw_enemy(e, (t_coord_i){(int)l.x, (int)l.y});
		}
		i++;
	}
}

void			init_minimap(t_env *e)
{
	draw_minimap(e);
}

void			draw_minimap(t_env *e)
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
	locate_enemy(e);
}
