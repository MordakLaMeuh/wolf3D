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

void			init_walls(t_env *env, char *file_name)
{
	rendering_layer_init(&(env->scene.wall), file_name);
}

/*
** This functions finds the intersection between
** segments (a[0])-(a[1]) and (b[0])-(b[1]).
** If there is an intersection, it returns 1 and (*i) contains the intersection.
** If there is no intersection, it returns 0.
*/

static int		segment_intersection(t_coord_f a[2], t_coord_f b[2],
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

static float	square_intersection(t_coord_f origin, t_coord_f c,
														t_coord_f *intersection)
{
	t_coord_f	corners[4];
	t_coord_f	segment[2];

	corners[0] = (t_coord_f){floorf(c.x), floorf(c.y)};
	corners[1] = (t_coord_f){corners[0].x + 1.f, corners[0].y};
	corners[2] = (t_coord_f){corners[0].x + 1.f, corners[0].y + 1.f};
	corners[3] = (t_coord_f){corners[0].x, corners[0].y + 1.f};
	segment[0] = origin;
	segment[1] = c;
	if (c.x >= origin.x && segment_intersection(segment,
						(t_coord_f[2]){corners[0], corners[3]}, intersection))
		return (1.f - (intersection->y - corners[0].y));
	if (c.x < origin.x && segment_intersection(segment,
						(t_coord_f[2]){corners[1], corners[2]}, intersection))
		return (intersection->y - corners[1].y);
	if (c.y >= origin.y && segment_intersection(segment,
						(t_coord_f[2]){corners[0], corners[1]}, intersection))
		return (intersection->x - corners[0].x);
	segment_intersection(segment, (t_coord_f[2]){corners[3], corners[2]},
																intersection);
	return (1. - (intersection->x - corners[3].x));
}

void			find_wall(t_env *env, float angle_x, t_coord_f *intersect,
																float *x_tex)
{
	int			d;
	t_coord_f	inc;
	t_coord_f	c;
	t_coord_i	c_i;

	inc = (t_coord_f){cosf(angle_x) / 20.f, sinf(angle_x) / 20.f};
	d = 0;
	while (1)
	{
		c = (t_coord_f){env->player.location.x + inc.x * d,
						env->player.location.y + inc.y * d};
		c_i = (t_coord_i){floorf(c.x), floorf(c.y)};
		if (env->map_tiles[c_i.y][c_i.x].value > 0)
		{
			*x_tex = square_intersection(env->player.location, c, intersect);
			break ;
		}
		d++;
	}
}

void			render_wall(t_env *env, t_rendering_layer *layer)
{
	t_coord_i	c;
	float		angle_y;
	float		wall_y_tex;
	t_column	*cl;

	layer->n = 0;
	c.y = -1;
	while (++c.y < HEIGHT)
	{
		angle_y = (atanf((float)((HEIGHT / 2) - c.y) / (WIDTH / 2))
					* (VIEW_ANGLE / 2.f / atanf(1.f)));
		c.x = -1;
		while (++c.x < WIDTH)
			if ((cl = &(env->scene.columns[c.x])) &&
				angle_y > cl->wall_min_angle && angle_y < cl->wall_max_angle)
			{
				wall_y_tex = (env->player.height + cl->wall_h_dist
											* tanf(angle_y)) / env->wall_height;
				layer->ij[layer->n] = c;
				layer->uv[layer->n] = (t_coord_f){cl->wall_x_tex
			* (layer->bmp->dim.x - 1), wall_y_tex * (layer->bmp->dim.y - 1)};
				layer->dist[layer->n++] = cl->wall_h_dist;
			}
	}
	rendering_layer_render(layer);
}
