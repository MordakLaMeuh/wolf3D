/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   floor.c                                            :+:      :+:    :+:   */
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

void				init_floor(t_env *env, char **textures, int n)
{
	rendering_layer_init(&(env->scene.floor), textures, n);
}

static inline t_coord_f	calc_tex_coord(t_coord_f location, float angle_x,
									float dist, t_coord_i bmp_dim)
{
	t_coord_f	c_floor;

	c_floor.x = (location.x + dist * cosf(angle_x)) / 4.f;
	c_floor.y = (location.y + dist * sinf(angle_x)) / 4.f;
	c_floor.x = (c_floor.x - floorf(c_floor.x)) * (bmp_dim.x - 1);
	c_floor.y = (c_floor.y - floorf(c_floor.y)) * (bmp_dim.y - 1);
	return (c_floor);
}

//cosf(env->angle_x[c.x])

void				render_floor(t_env *env, t_rendering_layer *layer)
{
	t_coord_i	c;
	t_coord_f	angle;

	layer->n = 0;
	c.y = -1;
	while (++c.y < HEIGHT)
	{
		angle.y = env->angle_y[c.y];
		c.x = -1;
		while (++c.x < WIDTH)
		{
			if (angle.y <= env->scene.columns[c.x].wall_min_angle)
			{
				angle.x = env->angle_x[c.x] + env->player.angle;
				layer->ij[layer->n] = c;
				layer->uv[layer->n] = calc_tex_coord(env->player.location,
								angle.x, env->dist_floor[c.y], layer->bmp->dim);
				layer->dist[layer->n++] = env->dist_floor[c.y];
			}
		}
	}
	rendering_layer_render(layer);
}
