/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 12:02:34 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/09 12:02:35 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

static t_sprite		*assign_sprites(t_env *env)
{
	t_coord_i		c;
	t_sprite		*sprites;

	if (!(sprites = (t_sprite*)malloc(sizeof(t_sprite) * env->n_sprites)))
		exit(EXIT_FAILURE);
	env->n_sprites = 0;
	c.y = 0;
	while (c.y < env->map.size.y)
	{
		c.x = 0;
		while (c.x < env->map.size.x)
		{
			if (env->map_tiles[c.y][c.x].value < 0)
			{
				sprites[env->n_sprites].location =
					(t_coord_f){c.x + .5f, c.y + .5f};
				sprites[env->n_sprites].type =
					-env->map_tiles[c.y][c.x].value - 1;
				env->n_sprites++;
			}
			c.x += 1;
		}
		c.y += 1;
	}
	return (sprites);
}

void				init_sprites(t_env *env, char **textures, int n)
{
	t_coord_i	c;

	if (!(env->scene.bmp_sprite = load_bitmap(textures, n)))
		exit(EXIT_FAILURE);
	env->scene.n_layer_sprite = 0;
	env->scene.sprites = (t_rendering_layer *)
					ft_memalloc(sizeof(t_rendering_layer) * WIDTH * HEIGHT);
	env->n_sprites = 0;
	c.y = -1;
	while (++c.y < env->map.size.y)
	{
		c.x = -1;
		while (++c.x < env->map.size.x)
			if (env->map_tiles[c.y][c.x].value < 0)
				env->n_sprites++;
	}
	env->sprites = assign_sprites(env);
	init_sprite_ai(env);
}

static inline int	angle_to_pix(float angle)
{
	int x;

	x = (int)(tanf(angle) / tanf((float)VIEW_ANGLE / 2.f) * (WIDTH / 2));
	return (x);
}

static int				m_cmp(void *a, void *b)
{
	if (((t_sprite *)a)->dist < ((t_sprite *)b)->dist)
		return (1);
	return (0);
}

t_sprite 			**create_z_buffer_order(t_env *env)
{
	int			i;
	int			l;
	t_sprite	**tmp;

	if (!(tmp = (t_sprite **)malloc((env->n_sprites + 1) * sizeof(t_sprite *))))
		exit (EXIT_FAILURE);
	tmp[env->n_sprites] = NULL;
	i = 0;
	while (i < env->n_sprites)
	{
		env->sprites[i].angle0_x = atan2f(env->sprites[i].location.y -
			env->player.location.y, env->sprites[i].location.x -
			env->player.location.x) - env->player.angle;
		l = angle_to_pix(env->sprites[i].angle0_x) + WIDTH / 2;
		if (l < 0)
			l = 0;
		if (l >= WIDTH)
			l = WIDTH - 1;
		env->sprites[i].dist = dist(env->player.location,
								env->sprites[i].location) * env->cos_list[l];
		tmp[i] = &env->sprites[i];
		i++;
	}
	ft_merge_tab((void ***)&tmp, env->n_sprites, &m_cmp);
	return (tmp);
}

void				render_sprites(t_env *env, t_rendering_layer *layer)
{
	t_coord_f			angle_topleft;
	t_coord_f			angle_bottomright;
	t_coord_i			c;
	t_coord_i			c_topleft;
	t_coord_i			c_bottomright;
	t_coord_i			c_max;
	t_coord_f			c_tex;
	t_rendering_layer	*origin;
	t_sprite			**tmp;

	tmp = create_z_buffer_order(env);
	origin = layer;
	env->scene.n_layer_sprite = 0;
	while (*tmp)
	{
		if ((*tmp)->angle0_x < 0.f)
			(*tmp)->angle0_x += 2.f * M_PI;
		if (cosf((*tmp)->angle0_x) <= 0.)
		{
			tmp++;
			continue ;
		}
		angle_topleft.x = (*tmp)->angle0_x - atanf(.5f / (*tmp)->dist);
		angle_bottomright.x = (*tmp)->angle0_x + atanf(.5f / (*tmp)->dist);
		c_topleft.x = angle_to_pix(angle_topleft.x) + WIDTH / 2;
		c_bottomright.x = angle_to_pix(angle_bottomright.x) + WIDTH / 2;
		angle_topleft.y = atanf((env->sprite_height - env->player.height) / (*tmp)->dist);
		angle_bottomright.y = atanf(-env->player.height / (*tmp)->dist);
		c_topleft.y = HEIGHT / 2 - angle_to_pix(angle_topleft.y);
		c_bottomright.y = HEIGHT / 2 - angle_to_pix(angle_bottomright.y);
		c.x = ((c_topleft.x >= 0) ? c_topleft.x : 0) - 1;
		c_max.x = (c_bottomright.x < WIDTH) ? c_bottomright.x : WIDTH - 1;

		while (++c.x < c_max.x)
		{
			if (env->scene.columns[c.x].wall_h_dist <= (*tmp)->dist)
				continue ;
			c.y = ((c_topleft.y >= 0) ? c_topleft.y : 0) - 1;
			c_max.y = (c_bottomright.y < HEIGHT) ? c_bottomright.y : HEIGHT - 1;
			while (++c.y < c_max.y && env->scene.n_layer_sprite <
																WIDTH * HEIGHT)
			{
				c_tex.x = (float)(c.x - c_topleft.x) /
					(c_bottomright.x - c_topleft.x) *
					(env->scene.bmp_sprite[(*tmp)->type].dim.x - 2);
			/*
				c_tex.y = (float)(c.y - c_bottomright.y) /
					(c_topleft.y - c_bottomright.y) *
					(env->scene.bmp_sprite[(*tmp)->type].dim.y - 2);
			*/
				c_tex.y = (float)(c.y - c_topleft.y) /
					(c_bottomright.y - c_topleft.y) *
					(env->scene.bmp_sprite[(*tmp)->type].dim.y - 2);

				layer->ij = c;
				layer->uv = c_tex;
				layer->type = (*tmp)->type;
				layer->dist = (*tmp)->dist;
				env->scene.n_layer_sprite += 1;
				layer++;
			}
		}

		tmp++;
	}
	rendering_layer_render_sprite(origin, env->inter_state,
						env->scene.n_layer_sprite, env->scene.bmp_sprite);
	free(tmp -= env->n_sprites);
}
