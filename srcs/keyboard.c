/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keyboard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/13 03:51:41 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/03 16:00:22 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "wolf3d.h"

#ifdef DEBUG_KEYBOARD

static void	print_register(unsigned long int *k)
{
	int i;

	i = 0;
	while (i < 256)
	{
		if (k[i])
			ft_printf("{green}%c{eoc}", (k[i]) ? '1' : '0');
		else
			ft_putchar(k[i] + '0');
		i++;
	}
	write(1, "\n", 1);
}

#endif

int			mlx_key_release(int keycode, t_env *e)
{
	e->keyb[keycode & 0xFF] = 0;
	if (DEBUG_KEYBOARD)
	{
		ft_printf("keycode %3i [mod]%3i RELEASED -> ", keycode, keycode & 0xFF);
		print_register(e->keyb);
	}
	return (keycode);
}

int			mlx_key_press(int keycode, t_env *e)
{
	if (!e->keyb[keycode & 0xFF])
		e->keyb[keycode & 0xFF] = get_time();
	if (DEBUG_KEYBOARD)
	{
		ft_printf("keycode %3i [mod]%3i PRESSED -> ", keycode, keycode & 0xFF);
		print_register(e->keyb);
	}
	return (keycode);
}
