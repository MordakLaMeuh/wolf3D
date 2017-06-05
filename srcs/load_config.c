/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 23:42:57 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 00:48:43 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "wolf3d.h"
#include "bmp.h"

t_bmp					*load_bitmap(char **name, int n)
{
	t_bmp 	*array;
	t_bmp	*bmp;
	int		i;

	if (!(array = malloc(sizeof(t_bmp) * n)))
		return (NULL);
	i = 0;
	while (i < n)
	{
		bmp = array + i;
		if (!(bmp_load(name[i], &(bmp->dim.x), &(bmp->dim.y), &(bmp->pix))))
			return (NULL);
		i++;
	}
	return (array);
}

static inline int		get_color(int b_color, int f_color, float ratio)
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

static int				get_clrs(t_bmp *src, t_coord_f c_src)
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

void					copy_img(t_bmp *dst, t_bmp *src)
{
	t_coord_i c_dst;
	t_coord_f c_src;

	c_dst.y = -1;
	while (++c_dst.y < dst->dim.y)
	{
		c_dst.x = -1;
		while (++c_dst.x < dst->dim.x)
		{
			c_src = (t_coord_f){c_dst.x * (float)src->dim.x / dst->dim.x,
								c_dst.y * (float)src->dim.y / dst->dim.y};
			if ((int)c_src.x >= ((src->dim.x - 1))
				|| ((int)c_src.y >= (src->dim.y - 1)))
				dst->pix[dst->dim.x * c_dst.y + c_dst.x] =
				src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x)];
			else
				dst->pix[dst->dim.x * c_dst.y + c_dst.x] = get_clrs(src, c_src);
		}
	}
}

/*
void					copy_img(t_bmp *dst, t_bmp *src)
{
	t_coord_i c_dst;
	t_coord_f c_src;

	c_dst.y = -1;
	while (++c_dst.y < dst->dim.y)
	{
		c_dst.x = -1;
		while (++c_dst.x < dst->dim.x)
		{
			c_src = (t_coord_f){c_dst.x * (float)src->dim.x / dst->dim.x,
								c_dst.y * (float)src->dim.y / dst->dim.y};
			if ((int)c_src.x >= ((src->dim.x - 1)) || ((int)c_src.y >= (src->dim.y - 1)))
				dst->pix[dst->dim.x * c_dst.y + c_dst.x] = src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x)];
			else
				dst->pix[dst->dim.x * c_dst.y + c_dst.x] = get_color(
					get_color(src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x)],
								src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x + 1)],
								c_src.x - (int)c_src.x),
					get_color(src->pix[(int)(src->dim.x * ((int)c_src.y + 1) + (int)c_src.x)],
								src->pix[(int)(src->dim.x * ((int)c_src.y + 1) + (int)c_src.x + 1)],
								c_src.x - (int)c_src.x),
					c_src.y - (int)c_src.y);
		}
	}
}
*/
