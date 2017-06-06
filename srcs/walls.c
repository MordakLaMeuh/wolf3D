/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   walls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 13:28:16 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/06 13:28:17 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

void				init_walls(t_env *env, char *file_name)
{
	env->wall = load_bitmap((char*[]){file_name}, 1);
	if (!env->wall)
		exit(EXIT_FAILURE);
}

/*
** This functions finds the intersection between
** segments (a[0])-(a[1]) and (b[0])-(b[1]).
** If there is an intersection, it returns 1 and (*i) contains the intersection.
** If there is no intersection, it returns 0.
*/

static int			segment_intersection(t_coord_f a[2], t_coord_f b[2],
																t_coord_f *i)
{
	t_coord_f	sa;
	t_coord_f	sb;
	float		s;
	float		t;

	sa = (t_coord_f){a[1].x - a[0].x, a[1].y - a[0].y};
	sb = (t_coord_f){b[1].x - b[0].x, b[1].y - b[0].y};
	t = -sb.x * sa.y + sa.x * sb.y;
	if (t == 0)
		return (0);
	s = (-sa.y * (a[0].x - b[0].x) + sa.x * (a[0].y - b[0].y)) / t;
	t = (sb.x * (a[0].y - b[0].y) - sb.y * (a[0].x - b[0].x)) / t;
	if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
	{
		i->x = a[0].x + t * sa.x;
		i->y = a[0].y + t * sa.y;
		return (1);
	}
	return (0);
}

/*
** This function finds the closest intersection between segment (origin)-(c)
** and the unit square that contains (c).
** The intersection tests are implemented in the following order:
**   west side;
**   east side;
**   south side;
**   north side.
*/

static t_coord_f	square_intersection(t_coord_f origin, t_coord_f c)
{
	t_coord_f	intersection;
	t_coord_f	corners[4];
	t_coord_f	segment[2];

	corners[0] = (t_coord_f){floor(c.x), floor(c.y)};
	corners[1] = (t_coord_f){corners[0].x + 1.f, corners[0].y};
	corners[2] = (t_coord_f){corners[0].x + 1.f, corners[0].y + 1.f};
	corners[3] = (t_coord_f){corners[0].x, corners[0].y + 1.f};
	segment[0] = origin;
	segment[1] = c;
	if (c.x >= origin.x && segment_intersection(segment,
						(t_coord_f[2]){corners[0], corners[3]}, &intersection))
		return (intersection);
	if (c.x < origin.x && segment_intersection(segment,
						(t_coord_f[2]){corners[1], corners[2]}, &intersection))
		return (intersection);
	if (c.y >= origin.y && segment_intersection(segment,
						(t_coord_f[2]){corners[0], corners[1]}, &intersection))
		return (intersection);
	segment_intersection(segment, (t_coord_f[2]){corners[3], corners[2]},
																&intersection);
	return (intersection);
}

void				find_wall(t_env *env, float angle_x, t_coord_i *c_i,
														t_coord_f *intersect)
{
	int			d;
	t_coord_f	inc;
	t_coord_f	c;

	inc = (t_coord_f){cos(angle_x) / 20.f, sin(angle_x) / 20.f};
	d = 0;
	while (1)
	{
		c = (t_coord_f){env->player.location.x + inc.x * d,
						env->player.location.y + inc.y * d};
		*c_i = (t_coord_i){floor(c.x), floor(c.y)};
		if (env->map_tiles[c_i->y][c_i->x].value > 0)
		{
			*intersect = square_intersection(env->player.location, c);
			break ;
		}
		d++;
	}
}

void				render_wall(t_env *env, t_coord_i c, float dist)
{
	t_pix		pix;

	pix.i = 0;
	pix.c.r = 255.;
	if (dist > 5)
		pix.c.r /= (dist / 5.);
	env->scene[c.y * WIDTH + c.x] = pix;
}
