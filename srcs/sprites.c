/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 12:02:34 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/09 12:02:35 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

void			init_sprites(t_env *env, char **textures, int n)
{
	t_coord_i	c;

	if (!(env->scene.bmp_sprite = load_bitmap(textures, n)))
		exit(EXIT_FAILURE);
	env->scene.n_layer_sprite = 0;
	env->scene.sprites = (t_rendering_layer *)ft_memalloc(sizeof(t_rendering_layer) * WIDTH * HEIGHT);

	env->n_sprites = 0;
	c.y = -1;
	while (++c.y < env->map.size.y)
	{
		c.x = -1;
		while (++c.x < env->map.size.x)
			if (env->map_tiles[c.y][c.x].value < 0)
				env->n_sprites++;
	}
	env->sprites = (t_sprite*)malloc(sizeof(t_sprite) * env->n_sprites);
	if (!(env->sprites))
		exit(EXIT_FAILURE);
	env->n_sprites = 0;
	c.y = -1;
	while (++c.y < env->map.size.y)
	{
		c.x = -1;
		while (++c.x < env->map.size.x)
			if (env->map_tiles[c.y][c.x].value < 0)
			{
				env->sprites[env->n_sprites].location = (t_coord_f){c.x + .5f, c.y + .5f};
				env->sprites[env->n_sprites].type = -env->map_tiles[c.y][c.x].value - 1;
				env->n_sprites++;
			}
	}
}

static inline int	angle_to_pix(float angle)
{
	int x;

	x = (int)(tanf(angle) / tanf((float)VIEW_ANGLE / 2.f) * (WIDTH / 2));
	return (x);
}

void	render_sprites(t_env *env, t_rendering_layer *layer)
{
	int					i;
	float				angle0_x;
	t_coord_f			angle_topleft;
	t_coord_f			angle_bottomright;
	float				d;
	t_coord_i			c;
	t_coord_i			c_topleft;
	t_coord_i			c_bottomright;
	t_coord_i			c_max;
	t_coord_f			c_tex;
	t_rendering_layer 	*origin;

	i = -1;
	origin = layer;
	env->scene.n_layer_sprite = 0;
	while (++i < env->n_sprites)
	{
		angle0_x = atan2f(env->sprites[i].location.y - env->player.location.y, env->sprites[i].location.x - env->player.location.x) - env->player.angle;
		c.x = angle_to_pix(angle0_x) + WIDTH / 2;
		if (c.x < 0)
			c.x = 0;
		if (c.x >= WIDTH)
			c.x = WIDTH - 1;
		d = dist(env->player.location, env->sprites[i].location) * env->cos_list[c.x];
		if (angle0_x < 0.f)
			angle0_x += 2.f * M_PI;
		if (cosf(angle0_x) <= 0.)
			continue ;
		angle_topleft.x = angle0_x - atanf(.5f / d);
		angle_bottomright.x = angle0_x + atanf(.5f / d);
		c_topleft.x = angle_to_pix(angle_topleft.x) + WIDTH / 2;
		c_bottomright.x = angle_to_pix(angle_bottomright.x) + WIDTH / 2;
		angle_topleft.y = atanf((env->sprite_height - env->player.height) / d);
		angle_bottomright.y = atanf(-env->player.height / d);
		c_topleft.y = HEIGHT / 2 - angle_to_pix(angle_topleft.y);
		c_bottomright.y = HEIGHT / 2 - angle_to_pix(angle_bottomright.y);
		c.x = ((c_topleft.x >= 0) ? c_topleft.x : 0) - 1;
		c_max.x = (c_bottomright.x < WIDTH) ? c_bottomright.x : WIDTH - 1;
		while (++c.x < c_max.x)
		{
			if (env->scene.columns[c.x].wall_h_dist <= d)
				continue ;
			c.y = ((c_topleft.y >= 0) ? c_topleft.y : 0) - 1;
			c_max.y = (c_bottomright.y < HEIGHT) ? c_bottomright.y : HEIGHT - 1;
			while (++c.y < c_max.y && env->scene.n_layer_sprite < WIDTH * HEIGHT)
			{
				c_tex.x = (float)(c.x - c_topleft.x) / (c_bottomright.x - c_topleft.x) * (env->scene.bmp_sprite[env->sprites[i].type].dim.x - 2);
				c_tex.y = (float)(c.y - c_bottomright.y) / (c_topleft.y - c_bottomright.y) * (env->scene.bmp_sprite[env->sprites[i].type].dim.y - 2);
				layer->ij = c;
				layer->uv = c_tex;
				layer->type = env->sprites[i].type;
				layer->dist = d;
				env->scene.n_layer_sprite += 1;
				layer++;
			}
		}
	}
	rendering_layer_render_sprite(origin, env->interpolate_state, env->scene.n_layer_sprite, env->scene.bmp_sprite);
}
