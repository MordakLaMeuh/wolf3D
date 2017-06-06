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

#define W_CENTER (WIDTH / 2)
#define H_CENTER (HEIGHT / 2)

void		modify_minimap(t_env *e, int direction)
{
	static float angle = 0;
	t_line line;

	if (direction)
		angle += 6 * M_PI / 360;
	else
		angle -= 6 * M_PI / 360;

	t_coord_i l1;
	t_coord_i l2;
	t_coord_i l3;

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


	l1.y = H_CENTER + (int)((round)((sin(angle + M_PI * 3 / 4)) * 20));
	l1.x = W_CENTER + (int)((round)((cos(angle + M_PI * 3 / 4)) * 20));
	l2.y = H_CENTER + (int)((round)((sin(angle - M_PI * 3 / 4)) * 20));
	l2.x = W_CENTER + (int)((round)((cos(angle - M_PI * 3 / 4)) * 20));
	l3.y = H_CENTER + (int)((round)((sin(angle)) * 40));
	l3.x = W_CENTER + (int)((round)((cos(angle)) * 40));

	line.p1 = l1;
	line.p2 = l2;
	line.b_color = 0x0000FF;
	line.f_color = 0x00FF00;
	draw_line(e, &line);

	line.p1 = l2;
	line.p2 = l3;
	line.b_color = 0x00FF00;
	line.f_color = 0xFF0000;
	draw_line(e, &line);

	line.p1 = l1;
	line.p2 = l3;
	line.b_color = 0x0000FF;
	line.f_color = 0xFF0000;
	draw_line(e, &line);

	(void)direction;
	(void)e;
}
