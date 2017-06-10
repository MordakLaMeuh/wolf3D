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

typedef struct	s_vector_2
{
	float		dx;
	float		dy;
	float		module;
}				t_vector_2;

typedef struct	s_wall_vector
{
	t_vector_2	v;
	t_coord_f 	norm;
}				t_wall_vector;

t_vector_2		create_vector(float angle, float dist)
{
	t_vector_2	s;

	s.module = dist;
	s.dx = cos(angle) * dist;
	s.dy = sin(angle) * dist;
	return (s);
}

t_wall_vector	get_wall_info(t_tile **tiles, float angle, t_coord_f location)

{
	t_coord_f ray_pos;
	t_coord_f ray_dir;
	t_coord_i map;
	t_coord_i step;
	t_coord_f delta_dist;
	t_coord_f side_dist;

	t_wall_vector	w;

	ray_pos = (t_coord_f){location.x, location.y};
	ray_dir = (t_coord_f){cosf(angle), sinf(angle)};

	map = (t_coord_i){(int)ray_pos.x, (int)ray_pos.y};
	if (ray_dir.y == 0.)
	{
		step.x = (ray_dir.x > 0) ? 1 : -1;
		while (tiles[map.y][map.x].value <= 0)
			map.x += step.x;
		w.norm = (t_coord_f){(ray_dir.x > 0) ? -1 : 1, 0};
		w.v = create_vector(angle, fabs((float)map.x - ray_pos.x));
		return (w);
	}
	if (ray_dir.x == 0.)
	{
		step.y = (ray_dir.y > 0) ? 1 : -1;
		while (tiles[map.y][map.x].value <= 0)
			map.y += step.y;
		w.norm = (t_coord_f){0, (ray_dir.y > 0) ? -1 : 1};
		w.v = create_vector(angle, fabs((float)map.y - ray_pos.y));
		return (w);
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

/*
** t_coord_f side_dist
** t_coord_f delta_dist
** t_coord_i map;
** t_coord_i step;
** t_coord_f ray_pos
** t_coord_f ray_dir
** ---------------------
** in arguments:
** t_normale *normale
** t_player *player;
** t_env e (for map)
*/
	int side;

	while (tiles[map.y][map.x].value <= 0)
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
	w.norm = (side == 1) ? (t_coord_f){0, -step.y} : (t_coord_f){-step.x, 0};
	if (side == 1)
		w.v = create_vector(angle,
			((float)map.y - ray_pos.y + (1. - step.y) / 2.) / ray_dir.y);
	else
		w.v = create_vector(angle,
			((float)map.x - ray_pos.x + (1. - step.x) / 2.) / ray_dir.x);
	return (w);
}

#include <stdio.h>

static void		set_player_data(t_env *e, t_modify_coord type)
{
	t_coord_f		new;
	t_wall_vector	w;

	e->player.angle += type.q * M_PI / 360;
	if (e->player.angle < 0)
		e->player.angle += 2.f * M_PI;
	else if (e->player.angle >= 2.f * M_PI)
		e->player.angle -= 2.f * M_PI;

	new.x = (cosf(e->player.angle)) * type.l;
	new.y = (sinf(e->player.angle)) * type.l;

	w = get_wall_info(e->map_tiles, e->player.angle, e->player.location);
	printf("dist: %f, normale: [%f:%f], dx: %f, dy: %f\n", w.v.module, w.norm.x, w.norm.y, w.v.dx, w.v.dy);

	new.x += e->player.location.x;
	new.y += e->player.location.y;

	if (e->map_tiles[(int)floorf(new.y)][(int)floorf(new.x)].value <= 0)
		e->player.location = new;

/*
//	float		i;
//	t_coord_f	normale;

	i = search_wall(e, &e->player, &normale, TRUE);
	printf("distance = %f, normale = [%f:%f]\n", i, normale.x, normale.y);

	t_vector_2 v = create_vector(e->player.angle, i);

	printf("dx: %f, dy; %f\n", v.dx, v.dy);

	new.x = (cosf(e->player.angle)) * type.l;
	new.y = (sinf(e->player.angle)) * type.l;

	printf("new_x: %f, new_y; %f\n", new.x, new.y);

	//new.x = (new.x > v.dx) ? 0 : new.x;
	//new.y = (new.y > v.dy) ? 0 : new.y;

	t_vector_2 	tmp;
	t_player   	false_player;
	t_coord_f	false_normale;

	if (normale.y == 1 && new.y < v.dy)
	{
		new.y = v.dy + 0.4;
		if (new.x > 0)
		{
			false_player.angle = 0;
			false_player.location = (t_coord_f){e->player.location.x, e->player.location.y + new.y};
			i = search_wall(e, &false_player, &false_normale, TRUE);
			tmp = create_vector(0, i);
//			printf("mur droit -> dx: %f, dy; %f\n", tmp.dx, tmp.dy);
			if (tmp.dx < new.x)
				new.x = tmp.dx - 0.4;
		}
		else
		{
			false_player.angle = M_PI;
			false_player.location = (t_coord_f){e->player.location.x, e->player.location.y + new.y};
			i = search_wall(e, &false_player, &false_normale, TRUE);
			tmp = create_vector(M_PI, i);
//			printf("mur gauche -> dx: %f, dy; %f\n", tmp.dx, tmp.dy);
			if (new.x < tmp.dx)
				new.x = tmp.dx + 0.4;
		}
	}
	if (normale.x == 1 && new.x < v.dx)
	{
		new.x = v.dx + 0.4;
		if (new.y > 0)
		{
			false_player.angle = M_PI * 1 / 2;
			false_player.location = (t_coord_f){e->player.location.x + new.x, e->player.location.y};
			i = search_wall(e, &false_player, &false_normale, TRUE);
			tmp = create_vector(M_PI * 1 / 2, i);
			printf("mur bas -> dx: %f, dy; %f\n", tmp.dx, tmp.dy);
			if (tmp.dy < new.y)
				new.y = tmp.dy - 0.4;
		}
		if (new.y < 0)
		{
			false_player.angle = M_PI * 3 / 2;
			false_player.location = (t_coord_f){e->player.location.x + new.x, e->player.location.y};
			i = search_wall(e, &false_player, &false_normale, TRUE);
			tmp = create_vector(M_PI * 3 / 2, i);
			printf("mur haut -> dx: %f, dy; %f\n", tmp.dx, tmp.dy);
			if (new.y < tmp.dy)
				new.y = tmp.dy + 0.4;
		}

	}
	e->player.location.x += new.x;
	e->player.location.y += new.y;

	new.x = e->player.location.x + ((cosf(e->player.angle)) * type.l);
	new.y = e->player.location.y + ((sinf(e->player.angle)) * type.l);

//	if (normale.y == -1)
//		new.y = (v.dy < 0.5) ? 0.5

//	i = 0;
//	if (type.l > 0)
//		i = search_wall(e, &e->player, &normale, TRUE);
//	else if (type.l < 0)
//		i = search_wall(e, &e->player, &normale, FALSE);

	//if (dist < i)
//		e->player.location = new;
*/
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
	if (event_register(e, KEYB_HELP, &state))
		e->interpolate_state = (e->interpolate_state) ? FALSE : TRUE;
	if (event_register(e, KEYB_P, &state))
		bmp_save(NULL, WIDTH, HEIGHT, (int *)e->img_string);
	return (state);
}
