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
