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

static void		locate_enemy(t_env *e, float ref_angle)
{
	t_coord_f	l;
	int			i;
	t_pix		pix;
	float		dist;
	float		angle;

	pix.i = 0x800000;
	i = -1;
	while (++i < e->n_sprites)
	{
		l.x = e->player.location.x - e->sprites[i].location.x;
		l.y = e->player.location.y - e->sprites[i].location.y;
		if ((dist = sqrtf((l.x * l.x) + (l.y * l.y))) < (MAP_DEPTH - 1))
		{
			angle = acosf(-l.x / dist);
			pix.i = (ref_angle - angle < 0) ? 0x800000 : 0xff0000;
			l.x = l.x * -(MAP_RADIUS / MAP_DEPTH) + MAP_ORIGIN_X;
			l.y = l.y * -(MAP_RADIUS / MAP_DEPTH) + MAP_ORIGIN_Y;
			if (FALSE)
				draw_enemy(e, (t_coord_i){(int)l.x, (int)l.y});
			draw_circle(e, (t_coord_i){(int)l.x, (int)l.y}, 4, pix);
		}
	}
}

void			init_minimap(t_env *e)
{
	draw_minimap(e);
}

/*
** void			draw_minimap(t_env *e)
** {
**	struct timespec	spec;
**	t_coord_i		l1;
**	t_pix			color;
**	float			angle;
**	t_line			line;
**
**	l1 = (t_coord_i){MAP_ORIGIN_X, MAP_ORIGIN_Y};
**	draw_arrow(e, l1, e->player.angle);
**	color.i = 0xffffff;
**	draw_circle(e, l1, MAP_RADIUS, color);
**	clock_gettime(CLOCK_REALTIME, &spec);
**	angle = e->player.angle + (float)(
**		(((((int)spec.tv_sec & 0x0F) * 1000) +
**		round(spec.tv_nsec / 1.0e6)) / 2000) * M_PI * 2);
**	line.p1 = l1;
**	line.p2.x = (floor)(cosf(angle) * MAP_RADIUS) + MAP_ORIGIN_X;
**	line.p2.y = (floor)(sinf(angle) * MAP_RADIUS) + MAP_ORIGIN_Y;
**	line.b_pix.i = 0xFFFF00;
**	line.f_pix.i = 0x00FFFF;
**	draw_line(e, &line);
**	locate_enemy(e, atan2(sin(angle), cos(angle)));
** }
*/

void			draw_minimap(t_env *e)
{
	struct timeval	spec;
	t_coord_i		l1;
	t_pix			color;
	float			angle;
	t_line			line;

	l1 = (t_coord_i){MAP_ORIGIN_X, MAP_ORIGIN_Y};
	draw_arrow(e, l1, e->player.angle);
	color.i = 0xffffff;
	draw_circle(e, l1, MAP_RADIUS, color);
	gettimeofday(&spec, NULL);
	angle = e->player.angle + (float)(
		(((((int)spec.tv_sec & 0x0F) * 1000) +
		round(spec.tv_usec / 1.0e3)) / 2000) * M_PI * 2);
	line.p1 = l1;
	line.p2.x = (floor)(cosf(angle) * MAP_RADIUS) + MAP_ORIGIN_X;
	line.p2.y = (floor)(sinf(angle) * MAP_RADIUS) + MAP_ORIGIN_Y;
	line.b_pix.i = 0xFFFF00;
	line.f_pix.i = 0x00FFFF;
	draw_line(e, &line);
	locate_enemy(e, atan2(sin(angle), cos(angle)));
}
