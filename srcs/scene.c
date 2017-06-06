/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 11:48:10 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/06 11:48:12 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

void	init_scene(t_env *env)
{
	env->scene = (t_pix*)malloc(sizeof(t_pix) * WIDTH * HEIGHT);
	if (!env->scene)
		exit(EXIT_FAILURE);
}

static	float	angle_on_screen(int x)
{
	return (atan((float)x / (WIDTH / 2)) * (VIEW_ANGLE / 2.f / atan(1.f)));
}

void	render_scene(t_env *env)
{
	t_coord_i	c;
	t_coord_f	angle;
	t_coord_f	c_intersect;
	float		h_dist;
	float		min_angle;
	float		max_angle;
	t_coord_f	c_tex;

	c.x = -1;
	while (++c.x < WIDTH)
	{
		angle.x = angle_on_screen(c.x - (WIDTH / 2)) + env->player.angle;
		c.y = -1;
		find_wall(env, angle.x, &c_intersect, &c_tex.x);
		h_dist = dist(env->player.location, c_intersect);
		min_angle = atan(-env->player.height / h_dist);
		max_angle = atan((env->wall_height - env->player.height) / h_dist);
		while (++c.y < HEIGHT)
		{
			angle.y = angle_on_screen((HEIGHT / 2) - c.y);
			if (angle.y >= max_angle)
				render_sky(env, c, angle);
			else if (angle.y >= min_angle)
			{
				c_tex.y = (env->player.height + h_dist * tan(angle.y)) / env->wall_height;
				render_wall(env, c, h_dist, c_tex);
			}
			else
				render_floor(env, c, angle);
		}
	}
}

void	scene_to_win(t_env *env)
{
	t_coord_i	c_scr;
	t_coord_i	c_scn;

	c_scr.y = -1;
	while (++c_scr.y < HEIGHT * NOSTALGIA_FACTOR)
	{
		c_scr.x = -1;
		while (++c_scr.x < WIDTH * NOSTALGIA_FACTOR)
		{
			c_scn.x = c_scr.x / NOSTALGIA_FACTOR;
			c_scn.y = c_scr.y / NOSTALGIA_FACTOR;
			env->img_string[WIDTH * NOSTALGIA_FACTOR * c_scr.y + c_scr.x] = env->scene[WIDTH * c_scn.y + c_scn.x];
		}
	}
}
