/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_wall.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/06 11:43:04 by stoupin           #+#    #+#             */
/*   Updated: 2017/07/06 11:43:05 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "render.h"

float						dist(t_coord_f a, t_coord_f b)
{
	t_coord_f				delta;

	delta.x = b.x - a.x;
	delta.y = b.y - a.y;
	return (sqrtf(delta.x * delta.x + delta.y * delta.y));
}

void						init_sprites(t_env *env, char **textures, int n)
{
	if (!(env->scene.bmp_sprite = load_bitmap(textures, n)))
		exit(EXIT_FAILURE);
}

int							m_cmp(void *a, void *b)
{
	if (((t_sprite *)a)->dist < ((t_sprite *)b)->dist)
		return (1);
	return (0);
}
