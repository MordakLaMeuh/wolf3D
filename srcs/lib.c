/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lib.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 07:01:21 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 07:01:24 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static inline int	get_color(int b_color, int f_color, float ratio)
{
	int		color;
	float	r_diff;
	float	g_diff;
	float	b_diff;

	r_diff = (f_color >> 16) - (b_color >> 16);
	g_diff = ((f_color >> 8) & 0xFF) - ((b_color >> 8) & 0xFF);
	b_diff = (f_color & 0xFF) - (b_color & 0xFF);
	color = ((((b_color >> 16) + ((int)(ratio * r_diff))) << 16) |
			((((b_color >> 8) & 0xFF) + (int)(ratio * g_diff)) << 8) |
			((b_color & 0xFF) + (int)(ratio * b_diff)));
	return (color);
}

int					get_clrs(t_bmp *src, t_coord_f c_src)
{
	int color;

	color = get_color(
				get_color(
		src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x)],
		src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x + 1)],
		c_src.x - (int)c_src.x),
				get_color(
		src->pix[(int)(src->dim.x * ((int)c_src.y + 1) + (int)c_src.x)],
		src->pix[(int)(src->dim.x * ((int)c_src.y + 1) + (int)c_src.x + 1)],
		c_src.x - (int)c_src.x), c_src.y - (int)c_src.y);
		return (color);
}
