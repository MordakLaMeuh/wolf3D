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

static void		set_player_data(t_env *e, t_modify_coord type)
{
	t_coord_f					new;

	e->player.angle += type.q * M_PI / 360;
	if (e->player.angle < 0)
		e->player.angle += 2.f * M_PI;
	else if (e->player.angle >= 2.f * M_PI)
		e->player.angle -= 2.f * M_PI;
	new.x = e->player.location.x + ((cosf(e->player.angle)) * type.l);
	new.y = e->player.location.y + ((sinf(e->player.angle)) * type.l);
	if (e->map_tiles[(int)floorf(new.y)][(int)floorf(new.x)].value == 0)
		e->player.location = new;
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
