/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 03:36:26 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 03:36:28 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

void	draw_arrow(t_env *e, float angle)
{
	t_line		line;
	t_coord_i	l1;
	t_coord_i	l2;
	t_coord_i	l3;

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
	line.b_color = 0x0000FF;
	draw_line(e, &line);
}

void	draw_box(t_coord_i p1, t_coord_i p2, int color, t_env *e)
{
	t_line line;

	line.b_color = color;
	line.f_color = color;
	line.p1 = p1;
	line.p2.y = p1.y;
	line.p2.x = p2.x;
	draw_line(e, &line);
	line.p1 = p2;
	draw_line(e, &line);
	line.p2.x = p1.x;
	line.p2.y = p2.y;
	draw_line(e, &line);
	line.p1 = p1;
	draw_line(e, &line);
}

void	fill_box(t_coord_i p1, t_coord_i p2, int color, t_env *e)
{
	t_line	line;
	int		i;

	line.b_color = color;
	line.f_color = color;
	line.p1.x = p1.x;
	line.p2.x = p2.x;
	i = p1.y;
	while (i <= p2.y)
	{
		line.p1.y = i;
		line.p2.y = i;
		draw_line(e, &line);
		i++;
	}
}
