/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 01:30:50 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 01:30:52 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include "wolf3d.h"

void		modify_minimap(t_env *e, int direction)
{
	static float	angle = 0;
	t_coord_i		l1;
	t_coord_i		l2;

	angle += (direction) ? 6 * M_PI / 360 : -6 * M_PI / 360;
	l1.x = W_CENTER - 201;
	l1.y = H_CENTER - 201;
	l2.x = W_CENTER + 201;
	l2.y = H_CENTER + 201;
	draw_box(l1, l2, 0x00ff00, e);
	l1.x = 50;
	l1.y = 50;
	l2.x = 150;
	l2.y = 100;
	fill_box(l1, l2, 0x8800ff00, e);
	l1.x = W_CENTER;
	l1.y = H_CENTER;
	draw_arrow(e, l1, angle);
}
