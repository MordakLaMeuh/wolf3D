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
#include <pthread.h>
#include "core/wolf3d.h"
#include "render/internal_render.h"

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
	if ((layer->type =
			e->map_tiles[(int)layer->uv.y][(int)layer->uv.x].value) >= 50)
		layer->type = 0;
	layer->uv.x /= 4.f;
	layer->uv.y /= 4.f;
	layer->uv.x = (layer->uv.x - floorf(layer->uv.x)) *
								(e->scene.bmp_floor[layer->type].dim.x - 1);
	layer->uv.y = (layer->uv.y - floorf(layer->uv.y)) *
								(e->scene.bmp_floor[layer->type].dim.y - 1);
}

void					*thread_floor_uv(void *arg)
{
	int					n;
	t_rendering_layer	*layer;
	t_env				*env;

	n = ((t_floor_uv_format *)arg)->n;
	env = ((t_floor_uv_format *)arg)->e;
	layer = ((t_floor_uv_format *)arg)->layer;
	while (n--)
	{
		calc_tex_coord(layer, env,
			env->angle_x[layer->ij.x] + env->player.angle,
			env->dist_floor[layer->ij.y] / env->cos_list[layer->ij.x]);
		layer->dist = env->dist_floor[layer->ij.y];
		layer++;
	}
	pthread_exit(NULL);
}

void					prepare_thread_floor_uv(t_env *env,
												t_rendering_layer *layer)
{
	t_floor_uv_format		format[NB_CORES];
	pthread_t				thread[NB_CORES];
	int						i;
	int						l;
	int						b;

	i = -1;
	b = 0;
	l = env->scene.n_layer_floor / NB_CORES;
	while (++i < NB_CORES)
	{
		format[i].layer = layer + b;
		b += l;
		if (i == (NB_CORES - 1))
			l += env->scene.n_layer_floor % NB_CORES;
		format[i].n = l;
		format[i].e = env;
		pthread_create(&thread[i], NULL, thread_floor_uv, &format[i]);
	}
	i = -1;
	while (++i < NB_CORES)
		pthread_join(thread[i], NULL);
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
				layer->ij = c;
				layer++;
				env->scene.n_layer_floor += 1;
			}
	}
	layer -= env->scene.n_layer_floor;
	prepare_thread_floor_uv(env, layer);
	rendering_layer_render(layer, env->inter_state, env->scene.n_layer_floor,
														env->scene.bmp_floor);
}
