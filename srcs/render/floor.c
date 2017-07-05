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
#include "core/wolf3d.h"

void					init_floor(t_env *e, char **textures, int n)
{
	if (!(e->scene.bmp_floor = load_bitmap(textures, n)))
		exit(EXIT_FAILURE);
	e->scene.n_layer_floor = 0;
	e->scene.floor = (t_rendering_layer *)ft_memalloc(sizeof(t_rendering_layer)
		* WIDTH * HEIGHT);
}


static inline void		calc_tex_coord(t_rendering_layer *layer, t_env *e,
							float angle_x, float dist)
{
	layer->uv.x = (e->player.location.x + dist * cosf(angle_x));
	layer->uv.y = (e->player.location.y + dist * sinf(angle_x));
	if ((layer->type = e->map_tiles[(int)layer->uv.y][(int)layer->uv.x].value) >= 50)
		layer->type = 0;
	layer->uv.x /= 4.f;
	layer->uv.y /= 4.f;
	layer->uv.x = (layer->uv.x - floorf(layer->uv.x)) *
								(e->scene.bmp_floor[layer->type].dim.x - 1);
	layer->uv.y = (layer->uv.y - floorf(layer->uv.y)) *
								(e->scene.bmp_floor[layer->type].dim.y - 1);
}

void					render_floor(t_env *env, t_rendering_layer *layer)
{
	t_coord_i			c;
	t_coord_f			angle;

	env->scene.n_layer_floor = 0;
	c.y = -1;
	while (++c.y < HEIGHT)
	{
		angle.y = env->angle_y[c.y];
		c.x = -1;
		while (++c.x < WIDTH)
			if (angle.y <= env->scene.columns[c.x].wall_min_angle)
			{
				angle.x = env->angle_x[c.x] + env->player.angle;
				layer->ij = c;
				calc_tex_coord(layer, env, angle.x, env->dist_floor[c.y] /
														env->cos_list[c.x]);
				layer->dist = env->dist_floor[c.y];
				layer++;
				env->scene.n_layer_floor += 1;
			}
	}
	rendering_layer_render(layer - env->scene.n_layer_floor, env->inter_state,
							env->scene.n_layer_floor, env->scene.bmp_floor);
}
