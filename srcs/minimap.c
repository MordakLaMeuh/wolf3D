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

void		init_minimap(t_env *e)
{
	draw_minimap(e);
}

void		draw_minimap(t_env *e)
{
	t_coord_i		l1;
	t_pix			color;

	l1 = (t_coord_i){MAP_ORIGIN_X, MAP_ORIGIN_Y};
	draw_arrow(e, l1, e->player.angle);
	color.i = 0xffffff;
	draw_circle(e, l1, MAP_RADIUS, color);
}
