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

int				move_player(t_env *e)
{
	int							trigger;
	int							i;
	static t_modify_coord		types[N_CONTROL] = {
		{KEYB_ARROW_LEFT, KEYB_MMO_A, -6, 0},
		{KEYB_ARROW_RIGHT, KEYB_MMO_D, +6, 0},
		{KEYB_ARROW_UP, KEYB_MMO_W, 0, 2},
		{KEYB_ARROW_DOWN, KEYB_MMO_S, 0, -2}
	};

	trigger = FALSE;
	i = -1;
	while (++i < N_CONTROL)
		if (e->keyb[types[i].keycode_1] || e->keyb[types[i].keycode_2])
		{
			e->player.angle += types[i].q * M_PI / 360;
			e->player.location.x += ((cos(e->player.angle)) * types[i].l);
			e->player.location.y += ((sin(e->player.angle)) * types[i].l);
			//move_sky(e, types[i].q);
			//modify_minimap(e);
			trigger = TRUE;
		}
	return (trigger);
}

int				common_action(t_env *e)
{
	if (e->keyb[KEYB_ESCAPE])
		exit_mlx(e);
	return (0);
}


/*
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
	int			state;
	t_bmp		*img;

	if (e->keyb[KEYB_ESCAPE])
		exit_mlx(e);
	state = FALSE;
	if (event_register(e, KEYB_M, &state))
	{
		static const char *file_names[] = {"images/2157a.bmp", "images/1495151132.bmp"};


		img = load_bitmap((char **)file_names, 2);
		img = &img[0];

		t_bmp		*img_f;

		img_f = malloc(sizeof(t_bmp));
		img_f->dim.x = WIDTH;
		img_f->dim.y = HEIGHT;
		img_f->pix = malloc(WIDTH * HEIGHT * 4);
		copy_img(img_f, img);

		int i;
		i = 0;
		while (i < (WIDTH * HEIGHT))
		{
			e->img_string[i] = img_f->pix[i];
			i++;
		}
	}
	return (state);
}
*/



/*
	if (event_register(e, KEYB_R, &state))
		f_reset(e->active_fractal);
	if (event_register(e, KEYB_HELP, &state))
		e->help_display = (e->help_display) ? FALSE : TRUE;
	if (event_register(e, KEYB_C, &state))
		modify_color(e, e->active_fractal);
	if (event_register(e, KEYB_P, &state))
		create_bmp_image(NULL, WIDTH, HEIGHT, e->img_string);
	if (event_register(e, KEYB_M, &state))
		e->mouse_lock = (e->mouse_lock) ? FALSE : TRUE;

typedef struct
{
	char *name;
} t_texture, *t_p_texture;

		t_p_texture img_table;
		t_texture *img_table = malloc();

		img_push(img_table, "A.bmp", "tex_walls");
		img_push(img_table, "B.bmp", "tex_floor");

		//char *img_table[2];
		//img_table[0] = "A.bmp";
		//img_table[1] = "B.bmp";
*/
