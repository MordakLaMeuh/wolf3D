/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/13 03:51:41 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/12 01:24:26 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "wolf3d.h"

#ifdef DEBUG_KEYBOARD

static void	print_register(char *k)
{
	int i;

	i = 0;
	while (i < 256)
	{
		if (k[i])
			ft_printf("{green}%c{eoc}", k[i] + '0');
		else
			ft_putchar(k[i] + '0');
		i++;
	}
	write(1, "\n", 1);
}

#endif

int			sdl_key_release(t_env *e)
{
	int keycode;

	keycode = e->event.key.keysym.sym;
	if (keycode > 1073741900 && keycode < 1073741910)
		keycode -= (KEYS_ARROWS_CONST - KEYS_ARROWS_ADD);
	e->keyb[keycode & 0xFF] = FALSE;
	if (DEBUG_KEYBOARD)
	{
		ft_printf("keycode %3i RELEASED -> ", keycode);
		print_register(e->keyb);
	}
	return (keycode);
}

int			sdl_key_press(t_env *e)
{
	int keycode;

	keycode = e->event.key.keysym.sym;
	if (keycode > 1073741900 && keycode < 1073741910)
		keycode -= KEYS_ARROWS_CONST - KEYS_ARROWS_ADD;
	e->keyb[keycode & 0xFF] = TRUE;
	if (DEBUG_KEYBOARD)
	{
		ft_printf("keycode %3i PRESSED  -> ", keycode);
		print_register(e->keyb);
	}
	return (keycode);
}
