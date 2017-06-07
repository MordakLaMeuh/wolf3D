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

void	draw_arrow(t_env *e, t_coord_i c, float angle, float factor)
{
	t_line		line;
	t_coord_i	l1;
	t_coord_i	l2;
	t_coord_i	l3;

	l1.y = c.y + (int)(((sinf(angle + M_PI * 3 / 4)) * 20 * factor));
	l1.x = c.x + (int)(((cosf(angle + M_PI * 3 / 4)) * 20 * factor));
	l2.y = c.y + (int)(((sinf(angle - M_PI * 3 / 4)) * 20 * factor));
	l2.x = c.x + (int)(((cosf(angle - M_PI * 3 / 4)) * 20 * factor));
	l3.y = c.y + (int)(((sinf(angle)) * 40 * factor));
	l3.x = c.x + (int)(((cosf(angle)) * 40 * factor));
	line.p1 = l1;
	line.p2 = l2;
	line.b_pix.i = 0x0000FF;
	line.f_pix.i = 0x00FF00;
	draw_line(e, &line);
	line.p1 = l2;
	line.p2 = l3;
	line.b_pix.i = 0x00FF00;
	line.f_pix.i = 0xFF0000;
	draw_line(e, &line);
	line.p1 = l1;
	line.b_pix.i = 0x0000FF;
	draw_line(e, &line);
}

void	draw_box(t_coord_i p1, t_coord_i p2, t_pix pix, t_env *e)
{
	t_line line;

	line.b_pix = pix;
	line.f_pix = pix;
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

void	fill_box(t_coord_i p1, t_coord_i p2, t_pix pix, t_env *e)
{
	t_line	line;
	int		i;

	line.b_pix = pix;
	line.f_pix = pix;
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
