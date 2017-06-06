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
#include "wolf3d.h"

void	init_scene(t_env *env)
{
	env->scene = (t_pix*)malloc(sizeof(t_pix) * WIDTH * HEIGHT);
	if (!env->scene)
		exit(EXIT_FAILURE);	
}

void	render_scene(t_env *env)
{
	t_coord_i	c;

	c.x = -1;
	while (++c.x < WIDTH)
	{
		c.y = -1;
		while (++c.y <= HEIGHT / 2)
		{
			render_sky(env, c);
		}
		while (++c.y < HEIGHT)
		{
			render_floor(env, c);
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
			env->img_string[WIDTH * NOSTALGIA_FACTOR * c_scr.y + c_scr.x]
				= env->scene[WIDTH * c_scn.y + c_scn.x];
		}
	}
}