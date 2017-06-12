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

t_vector_2		create_vector(float angle, float dist)
{
	t_vector_2	s;

	s.module = dist;
	s.dx = cos(angle) * dist;
	s.dy = sin(angle) * dist;
	return (s);
}

t_wall_vector	end_seq(t_tile **tiles, float angle, t_wall_info i)
{
	int		side;

	while (tiles[i.map.y][i.map.x].value <= 0)
		if (i.side_dist.x < i.side_dist.y)
		{
			i.side_dist.x += i.delta_dist.x;
			i.map.x += i.step.x;
			side = 0;
		}
		else
		{
			i.side_dist.y += i.delta_dist.y;
			i.map.y += i.step.y;
			side = 1;
		}
	i.w.norm = (side == 1) ? (t_coord_f){0, -i.step.y} :
													(t_coord_f){-i.step.x, 0};
	if (side == 1)
		i.w.v = create_vector(angle,
		((float)i.map.y - i.ray_pos.y + (1. - i.step.y) / 2.) / i.ray_dir.y);
	else
		i.w.v = create_vector(angle,
		((float)i.map.x - i.ray_pos.x + (1. - i.step.x) / 2.) / i.ray_dir.x);
	return (i.w);
}

t_wall_vector	middle_seq(t_tile **tiles, float angle, t_wall_info i)
{
	i.delta_dist.x = sqrt(1. + (i.ray_dir.y * i.ray_dir.y) /
												(i.ray_dir.x * i.ray_dir.x));
	i.delta_dist.y = sqrt(1. + (i.ray_dir.x * i.ray_dir.x) /
												(i.ray_dir.y * i.ray_dir.y));
	if (i.ray_dir.x < 0)
	{
		i.step.x = -1;
		i.side_dist.x = (i.ray_pos.x - i.map.x) * i.delta_dist.x;
	}
	else
	{
		i.step.x = 1;
		i.side_dist.x = ((float)i.map.x + 1. - i.ray_pos.x) * i.delta_dist.x;
	}
	if (i.ray_dir.y < 0)
	{
		i.step.y = -1;
		i.side_dist.y = (i.ray_pos.y - i.map.y) * i.delta_dist.y;
	}
	else
	{
		i.step.y = 1;
		i.side_dist.y = ((float)i.map.y + 1. - i.ray_pos.y) * i.delta_dist.y;
	}
	return (end_seq(tiles, angle, i));
}

t_wall_vector	get_wall_info(t_tile **tiles, float angle, t_coord_f location)
{
	t_wall_info		i;

	i.ray_pos = (t_coord_f){location.x, location.y};
	i.ray_dir = (t_coord_f){cosf(angle), sinf(angle)};
	i.map = (t_coord_i){(int)i.ray_pos.x, (int)i.ray_pos.y};
	if (i.ray_dir.y == 0.)
	{
		i.step.x = (i.ray_dir.x > 0) ? 1 : -1;
		while (tiles[i.map.y][i.map.x].value <= 0)
			i.map.x += i.step.x;
		i.w.norm = (t_coord_f){(i.ray_dir.x > 0) ? -1 : 1, 0};
		i.w.v = create_vector(angle, fabs((float)i.map.x - i.ray_pos.x));
		return (i.w);
	}
	if (i.ray_dir.x == 0.)
	{
		i.step.y = (i.ray_dir.y > 0) ? 1 : -1;
		while (tiles[i.map.y][i.map.x].value <= 0)
			i.map.y += i.step.y;
		i.w.norm = (t_coord_f){0, (i.ray_dir.y > 0) ? -1 : 1};
		i.w.v = create_vector(angle, fabs((float)i.map.y - i.ray_pos.y));
		return (i.w);
	}
	return (middle_seq(tiles, angle, i));
}

float			mvt_right(t_tile **map, t_coord_f mvt, t_coord_f location)
{
	t_wall_vector	w;

	w = get_wall_info(map, 0, (t_coord_f){location.x,
										location.y + mvt.y});
	if ((w.v.dx - 0.4) < mvt.x)
		return (w.v.dx - 0.4);
	else
		return (mvt.x);
}

float			mvt_left(t_tile **map, t_coord_f mvt, t_coord_f location)
{
	t_wall_vector	w;

	w = get_wall_info(map, M_PI, (t_coord_f){location.x,
										location.y + mvt.y});
	if (mvt.x < (w.v.dx + 0.4))
		return (w.v.dx + 0.4);
	else
		return (mvt.x);
}

float			mvt_top(t_tile **map, t_coord_f mvt, t_coord_f location)
{
	t_wall_vector	w;

	w = get_wall_info(map, M_PI * 3 / 2,
								(t_coord_f){location.x + mvt.x, location.y});
	if ((w.v.dy + 0.4) > mvt.y)
		return (w.v.dy + 0.4);
	else
		return (mvt.y);
}

float			mvt_back(t_tile **map, t_coord_f mvt, t_coord_f location)
{
	t_wall_vector	w;

	w = get_wall_info(map, M_PI * 1 / 2,
								(t_coord_f){location.x + mvt.x, location.y});
	if (mvt.y > (w.v.dy - 0.4))
		return (w.v.dy - 0.4);
	else
		return (mvt.y);
}

t_coord_f		test_mvt(t_wall_vector w, t_env *e, t_coord_f new)
{
	if (w.norm.y == 1 && new.y < (w.v.dy + 0.4))
	{
		new.y = w.v.dy + 0.4;
		new.x = (new.x > 0) ? mvt_right(e->map_tiles, new, e->player.location) :
			mvt_left(e->map_tiles, new, e->player.location);
	}
	if (w.norm.x == 1 && new.x < (w.v.dx + 0.4))
	{
		new.x = w.v.dx + 0.4;
		new.y = (w.v.dy > 0) ? mvt_back(e->map_tiles, new, e->player.location) :
			mvt_top(e->map_tiles, new, e->player.location);
	}
	if (w.norm.y == -1 && new.y > (w.v.dy - 0.4))
	{
		new.y = w.v.dy - 0.4;
		new.x = (new.x > 0) ? mvt_right(e->map_tiles, new, e->player.location) :
			mvt_left(e->map_tiles, new, e->player.location);
	}
	if (w.norm.x == -1 && new.x > (w.v.dx - 0.4))
	{
		new.x = w.v.dx - 0.4;
		new.y = (w.v.dy > 0) ? mvt_back(e->map_tiles, new, e->player.location) :
			mvt_top(e->map_tiles, new, e->player.location);
	}
	return (new);
}

static void		set_player_data(t_env *e, float q, float l)
{
	t_coord_f			new;
	t_wall_vector		w;

	e->player.angle += q * M_PI / 360;
	if (e->player.angle < 0)
		e->player.angle += 2.f * M_PI;
	else if (e->player.angle >= 2.f * M_PI)
		e->player.angle -= 2.f * M_PI;
	new.x = (cosf(e->player.angle)) * l;
	new.y = (sinf(e->player.angle)) * l;
	w = get_wall_info(e->map_tiles, e->player.angle, e->player.location);
	new = test_mvt(w, e, new);
	w = get_wall_info(e->map_tiles, e->player.angle + M_PI, e->player.location);
	new = test_mvt(w, e, new);
	e->player.location.x += new.x;
	e->player.location.y += new.y;
}

int				move_player(t_env *e)
{
	int							i;
	static t_modify_coord		types[N_CONTROL] = {
		{KEYB_ARROW_LEFT, KEYB_MMO_A, -0.20, 0},
		{KEYB_ARROW_RIGHT, KEYB_MMO_D, +0.20, 0},
		{KEYB_ARROW_UP, KEYB_MMO_W, 0, 0.015},
		{KEYB_ARROW_DOWN, KEYB_MMO_S, 0, -0.015}
	};
	unsigned long int			time_elapsed;
	float						new_q;
	float						new_l;

	i = -1;
	while (++i < N_CONTROL)
		if (e->keyb[types[i].keycode_1])
		{
			time_elapsed = get_time();
			new_q = (time_elapsed - e->keyb[types[i].keycode_1]) * types[i].q;
			new_l = (time_elapsed - e->keyb[types[i].keycode_1]) * types[i].l;
			e->keyb[types[i].keycode_1] = time_elapsed;
			set_player_data(e, new_q, new_l);
		}
	return (0);
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
	{
		e->interpolate_time = get_time();
		e->interpolate_state = (e->interpolate_state) ? FALSE : TRUE;
	}
	if (event_register(e, KEYB_P, &state))
		bmp_save(NULL, WIDTH, HEIGHT, (int *)e->img_string);
	return (state);
}
