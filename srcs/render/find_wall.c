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

#include <math.h>
#include "core/wolf3d.h"

/*
** This function finds the intersection between a ray and the first wall
** it encounters, along with the x uv coordinate on the texture
**	inputs:
**		env->map_tiles: 2d array representing the map (value >= 50 == wall)
**		env->player.location.x: x location of the player
**		env->player.location.y: y location of the player
**		angle_x: angle of the ray
**	outputs:
**		intersect: x, y coordinates of the intersection of the ray
**					with the wall
**		x_tex: x uv coordinate of the ray on the texture
*/

int				find_wall(t_env *env, float angle_x, t_coord_f *intersect,
																float *x_tex)
{
	t_coord_f	ray_dir;
	t_coord_i	c_i;
	t_coord_f	delta_dist;
	t_coord_f	side_dist;
	t_coord_i	step;
	int			hit;
	int			side;
	float		wall_dist;

	ray_dir = (t_coord_f){cosf(angle_x), sinf(angle_x)};
	c_i = (t_coord_i){(int)(env->player.location.x),
						(int)(env->player.location.y)};
	delta_dist = (t_coord_f){0.f, 0.f};
	if (ray_dir.x != 0.f)
		delta_dist.x = fabs(1.f / ray_dir.x);
	if (ray_dir.y != 0.f)
		delta_dist.y = fabs(1.f / ray_dir.y);
	step = (t_coord_i){0, 0};
	side_dist = (t_coord_f){0.f, 0.f};
	if (ray_dir.x < 0.f)
	{
		step.x = -1;
		side_dist.x = (env->player.location.x - (float)(c_i.x)) * delta_dist.x;
	}
	else if (ray_dir.x > 0.f)
	{
		step.x = 1;
		side_dist.x = ((float)(c_i.x) + 1.f - env->player.location.x) * delta_dist.x;
	}
	if (ray_dir.y < 0.f)
	{
		step.y = -1;
		side_dist.y = (env->player.location.y - (float)(c_i.y)) * delta_dist.y;
	}
	else if (ray_dir.y > 0.f)
	{
		step.y = 1;
		side_dist.y = ((float)(c_i.y) + 1.f - env->player.location.y) * delta_dist.y;
	}
	hit = 0;
	while (hit == 0)
	{
		if (ray_dir.y == 0.f || side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			c_i.x += step.x;
			side = 0;
		}
		else
		{
			side_dist.y += delta_dist.y;
			c_i.y += step.y;
			side = 1;
		}
		if (env->map_tiles[c_i.y][c_i.x].value >= 50)
			hit = 1;
	}
	if (side == 0)
		wall_dist = ((float)(c_i.x) - env->player.location.x + (1.f - (float)(step.x)) / 2.f) / ray_dir.x;
	else
		wall_dist = ((float)(c_i.y) - env->player.location.y + (1.f - (float)(step.y)) / 2.f) / ray_dir.y;
	intersect->x = env->player.location.x + wall_dist * ray_dir.x;
	intersect->y = env->player.location.y + wall_dist * ray_dir.y;
	*x_tex = 0.f;
	if (side == 1)
		*x_tex = (intersect->x - (float)c_i.x) * step.y;
	else
		*x_tex = (1.f - intersect->y + (float)c_i.y) * step.x;
	return (env->map_tiles[c_i.y][c_i.x].value - 50);
}
