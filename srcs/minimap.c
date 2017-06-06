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
	if (e->map.size.x <= 10 && e->map.size.y <= 10)
	{
		e->map.scale = (e->map.size.x < e->map.size.y) ?
												e->map.size.y : e->map.size.x;
		e->map.scale = (float)10 / e->map.scale;
	}
	printf("map scale = %f\n", e->map.scale);
	modify_minimap(e);
}

void		modify_minimap(t_env *e)
{
	t_coord_i		l1;
	t_coord_i		l2;
	t_pix			pix;

	l1.x = (X_MAP_CENTER) - MAP_SEMI_LENGTH;
	l1.y = (Y_MAP_CENTER) - MAP_SEMI_LENGTH;
	l2.x = (X_MAP_CENTER) + MAP_SEMI_LENGTH;
	l2.y = (Y_MAP_CENTER) + MAP_SEMI_LENGTH;
	pix.i = 0x00ff00;
	draw_box(l1, l2, pix, e);
	l1.x = (X_MAP_CENTER + e->player.location.x);
	l1.y = (Y_MAP_CENTER + e->player.location.y);
	draw_arrow(e, l1, e->player.angle);
}
