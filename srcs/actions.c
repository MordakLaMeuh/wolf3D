/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_coord.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/13 06:04:12 by bmickael          #+#    #+#             */
/*   Updated: 2017/05/18 04:40:21 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "wolf3d.h"
#include "bmp.h"

float				search_wall(t_env *e, t_player *p, t_coord_f *normale,
																int direction)
{
	t_coord_f ray_pos;
	t_coord_f ray_dir;
	t_coord_i map;
	t_coord_i step;
	t_coord_f delta_dist;
	t_coord_f side_dist;

	ray_pos = (t_coord_f){p->location.x, p->location.y};

	if (direction)
		ray_dir = (t_coord_f){cosf(p->angle), sinf(p->angle)};
	else
		ray_dir = (t_coord_f){cosf(-p->angle), sinf(-p->angle)};


	map = (t_coord_i){(int)ray_pos.x, (int)ray_pos.y};
	if (ray_dir.y == 0.)
	{
		step.x = (ray_dir.x > 0) ? 1 : -1;
		while (e->map_tiles[map.y][map.x].value == 0)
			map.x += step.x;
		*normale = (t_coord_f){(ray_dir.x > 0) ? -1 : 1, 0};
		return (fabs((float)map.x - ray_pos.x));
	}
	if (ray_dir.x == 0.)
	{
		step.y = (ray_dir.y > 0) ? 1 : -1;
		while (e->map_tiles[map.y][map.x].value == 0)
			map.y += step.y;
		*normale = (t_coord_f){0, (ray_dir.y > 0) ? -1 : 1};
		return (fabs((float)map.y - ray_pos.y));
	}

	delta_dist.x = sqrt(1. + (ray_dir.y * ray_dir.y) / (ray_dir.x * ray_dir.x));
	delta_dist.y = sqrt(1. + (ray_dir.x * ray_dir.x) / (ray_dir.y * ray_dir.y));
	if (ray_dir.x < 0)
	{
		step.x = -1;
		side_dist.x = (ray_pos.x - map.x) * delta_dist.x;
	}
	else
	{
		step.x = 1;
		side_dist.x = ((float)map.x + 1. - ray_pos.x) * delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		step.y = -1;
		side_dist.y = (ray_pos.y - map.y) * delta_dist.y;
	}
	else
	{
		step.y = 1;
		side_dist.y = ((float)map.y + 1. - ray_pos.y) * delta_dist.y;
	}


	int side;
	float wall_dist;

	while (!e->map_tiles[map.y][map.x].value)
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			map.x += step.x;
			side = 0;
		}
		else
		{
			side_dist.y += delta_dist.y;
			map.y += step.y;
			side = 1;
		}
	*normale = (side == 1) ? (t_coord_f){0, -step.y} : (t_coord_f){-step.x, 0};
	if (side == 1)
		wall_dist = ((float)map.y - ray_pos.y + (1. - step.y) / 2.) / ray_dir.y;
	else
		wall_dist = ((float)map.x - ray_pos.x + (1. - step.x) / 2.) / ray_dir.x;
	return (wall_dist);
}

#include <stdio.h>

static void		set_player_data(t_env *e, t_modify_coord type)
{
	t_coord_f	new;
	float		i;
	t_coord_f	normale;
	float		dist;

	e->player.angle += type.q * M_PI / 360;
	if (e->player.angle < 0)
		e->player.angle += 2.f * M_PI;
	else if (e->player.angle >= 2.f * M_PI)
		e->player.angle -= 2.f * M_PI;


	new.x = e->player.location.x + ((cosf(e->player.angle)) * type.l);
	new.y = e->player.location.y + ((sinf(e->player.angle)) * type.l);
	dist = sqrt((new.y - e->player.location.y) * (new.y - e->player.location.y)
	+ (new.x - e->player.location.x) * (new.x - e->player.location.x));


	if (e->map_tiles[(int)floorf(new.y)][(int)floorf(new.x)].value == 0)
		e->player.location = new;

/*
	i = 0;
	if (type.l > 0)
		i = search_wall(e, &e->player, &normale, TRUE);
	else if (type.l < 0)
		i = search_wall(e, &e->player, &normale, FALSE);
	if (dist < i)
		e->player.location = new;
*/

	i = search_wall(e, &e->player, &normale, TRUE);
	//printf("distance = %f, dist = %f, normale = [%f:%f]\n", i, dist, normale.x, normale.y);
}

int				move_player(t_env *e)
{
	int							trigger;
	int							i;
	static t_modify_coord		types[N_CONTROL] = {
		{KEYB_ARROW_LEFT, KEYB_MMO_A, -6, 0},
		{KEYB_ARROW_RIGHT, KEYB_MMO_D, +6, 0},
		{KEYB_ARROW_UP, KEYB_MMO_W, 0, 1},
		{KEYB_ARROW_DOWN, KEYB_MMO_S, 0, -1}
	};

	trigger = FALSE;
	i = -1;
	while (++i < N_CONTROL)
		if (e->keyb[types[i].keycode_1] || e->keyb[types[i].keycode_2])
		{
			set_player_data(e, types[i]);
			trigger = TRUE;
		}
	return (trigger);
}

static int		event_register(t_env *e, int keycode, int *state)
{
	static int	reg[512];

	keycode &= 0x1FF;
	if (e->keyb[keycode] && reg[keycode] == FALSE)
	{
		reg[keycode] = TRUE;
		*state = TRUE;
		return (1);
	}
	else if (!(e->keyb[keycode]) && reg[keycode] == TRUE)
		reg[keycode] = FALSE;
	return (0);
}

int				common_action(t_env *e)
{
	int state;

	state = FALSE;
	if (e->keyb[KEYB_ESCAPE])
		exit_mlx(e);
	if (event_register(e, KEYB_M, &state))
		e->display_minimap = (e->display_minimap) ? FALSE : TRUE;
	return (state);
}
