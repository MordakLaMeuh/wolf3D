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

#include <stdio.h>

void		init_minimap(t_env *e)
{
	if (e->map.size_x <= 10 && e->map.size_y <= 10)
	{
		e->map.scale = (e->map.size_x < e->map.size_y) ?
												e->map.size_y : e->map.size_x;
		e->map.scale = (float)10 / e->map.scale;
	}
	printf("map scale = %f\n", e->map.scale);
	modify_minimap(e);
}

void		modify_minimap(t_env *e)
{
	t_coord_i		l1;
	t_coord_i		l2;

	l1.x = (X_MAP_CENTER) - MAP_SEMI_LENGTH;
	l1.y = (Y_MAP_CENTER) - MAP_SEMI_LENGTH;
	l2.x = (X_MAP_CENTER) + MAP_SEMI_LENGTH;
	l2.y = (Y_MAP_CENTER) + MAP_SEMI_LENGTH;
	draw_box(l1, l2, 0x00ff00, e);
	l1.x = (X_MAP_CENTER + e->perso.location.x);
	l1.y = (Y_MAP_CENTER + e->perso.location.y);
	draw_arrow(e, l1, e->perso.angle);
}
