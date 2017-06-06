/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 11:51:00 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/06 11:51:02 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "wolf3d.h"

void			init_sky(t_env *env, char *file_name)
{
	env->sky = load_bitmap((char*[]){file_name}, 1);
	if (!env->sky)
		exit(EXIT_FAILURE);
}

void			render_sky(t_env *env, t_coord_i c)
{
	t_coord_f	angle;
	t_coord_f	c_sky;
	t_pix		pix;

	angle.x = (c.x - WIDTH / 2) * VIEW_ANGLE / WIDTH + env->player.angle;
	angle.y = (c.y - HEIGHT / 2) * VIEW_ANGLE / WIDTH;
	c_sky.x = angle.x * env->sky->dim.x / (2.f * M_PI) + env->sky->dim.x / 2.f;
	c_sky.y = angle.y * env->sky->dim.x / (2.f * M_PI) + env->sky->dim.y / 2.f;
	if (c_sky.x < 0.f)
		c_sky.x += env->sky->dim.x;
	if (c_sky.x >= env->sky->dim.x)
		c_sky.x -= env->sky->dim.x;
	if (c_sky.y < 0.f)
		c_sky.y += env->sky->dim.y;
	if (c_sky.y >= env->sky->dim.y)
		c_sky.y -= env->sky->dim.y;
	pix = get_pix(env->sky, c_sky);
	env->scene[c.y * WIDTH + c.x] = pix;
}