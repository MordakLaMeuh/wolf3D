/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 11:48:10 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/08 13:46:47 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

void				init_scene(t_env *env)
{
	if (!(env->scene.columns = (t_column*)malloc(sizeof(t_column) * WIDTH)))
		exit(EXIT_FAILURE);
	if (NOSTALGIA_FACTOR == 1)
		env->scene.scene = env->img_string;
	else if (!(env->scene.scene = (t_pix*)malloc(sizeof(t_pix)
															* WIDTH * HEIGHT)))
		exit(EXIT_FAILURE);
}

static void			merge_layers(t_env *env)
{
	t_pix	*scene;

	scene = env->scene.scene;
	render_sky(env, env->player.angle);
	rendering_layer_put(scene, (env->scene.wall), env->scene.n_layer_wall);
	rendering_layer_put(scene, (env->scene.floor), env->scene.n_layer_floor);
	rendering_layer_put_sprite(scene, (env->scene.sprites), env->scene.n_layer_sprite);
}

void				render_scene(t_env *env)
{
	int			x;
	t_coord_f	c_intersect;
	t_column	*c;
	float		angle_x;

	x = -1;
	while (++x < WIDTH)
	{
		c = &(env->scene.columns[x]);
		angle_x = env->angle_x[x] + env->player.angle;
		c->type = find_wall(env, angle_x, &c_intersect, &(c->wall_x_tex));
		c->wall_h_dist = dist(env->player.location, c_intersect) *
															env->cos_list[x];
		c->wall_min_angle = atanf(-env->player.height / c->wall_h_dist);
		c->wall_max_angle = atanf((env->wall_height - env->player.height)
									/ c->wall_h_dist);
	}
	render_wall(env, (env->scene.wall));
	render_floor(env, (env->scene.floor));
	render_sprites(env, (env->scene.sprites));
	merge_layers(env);
}

void				scene_to_win(t_env *env)
{
	t_coord_i	c_scr;
	t_coord_i	c_scn;
	t_pix		*scene;
	int			i;

	if (NOSTALGIA_FACTOR == 1)
		return ;
	scene = env->scene.scene;
	i = 0;
	c_scr.y = -1;
	while (++c_scr.y < HEIGHT * NOSTALGIA_FACTOR)
	{
		c_scr.x = -1;
		while (++c_scr.x < WIDTH * NOSTALGIA_FACTOR)
		{
			c_scn.x = c_scr.x / NOSTALGIA_FACTOR;
			c_scn.y = c_scr.y / NOSTALGIA_FACTOR;
			env->img_string[i++] = scene[WIDTH * c_scn.y + c_scn.x];
		}
	}
}
