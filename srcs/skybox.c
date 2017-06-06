/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 23:43:10 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 04:30:56 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "wolf3d.h"
#include "bmp.h"

/*
** Vue size : ((WIDTH * HEIGHT) * VIEW_ANGLE) / 360
** Example : (1080*1920*66)/360 = 380160
** Total size : (1080 * 1920 * 360) / 66 = 11310545
** Total HEIGHT = 1080
** Total WIDTH = (1920 * 360) / 66 = 10472
*/

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

void					copy_img_bis(t_bmp *dst, t_bmp *src, int new_dim_x)
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
				dst->pix[new_dim_x * c_dst.y + c_dst.x] =
				src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x)];
			else
				dst->pix[new_dim_x * c_dst.y + c_dst.x] = get_clrs(src, c_src);
		}
	}
}

void			paste_red(int *data)
{
	int y;
	int x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 6 * WIDTH;
		while (x < (7 * WIDTH))
		{
			data[y * (WIDTH * 7) + x] = 0xFF0000;
			x++;
		}
		y++;
	}
}

void			paste_bout(int *data)
{
	int y;
	int x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 6 * WIDTH;
		while (x < (7 * WIDTH))
		{
			data[y * (WIDTH * 7) + x] = data[y * (WIDTH * 7) + (x - (6 * WIDTH))];
			x++;
		}
		y++;
	}
}

void			init_sky(t_env *e, char *file_name)
{
	t_bmp	*sky_bmp;
	int		i;
	int		j;

	if (!(e->sky.data = malloc(sizeof(t_bmp))))
		exit (EXIT_FAILURE);
	sky_bmp = load_bitmap((char*[]){file_name}, 1);
	sky_bmp = &sky_bmp[0];
	e->sky.ratio = 360 / 60;
	e->sky.data->dim.x = WIDTH * e->sky.ratio;
	e->sky.data->dim.y = HEIGHT;
	if (!(e->sky.data->pix = (int *)ft_memalloc
								(WIDTH * (e->sky.ratio + 1) * HEIGHT * sizeof(int))))
		exit (EXIT_FAILURE);
	copy_img_bis(e->sky.data, sky_bmp, WIDTH * (e->sky.ratio + 1));
//	paste_red(e->sky.data->pix);
	paste_bout(e->sky.data->pix);
	free(sky_bmp->pix);
	free(sky_bmp);
	i = 0;
	j = 0;
	while (j < SCREENSIZE)
	{
		e->img_string[j++] = e->sky.data->pix[i++];
		if (j % WIDTH == 0)
			i += WIDTH * e->sky.ratio;
	}
}

void			move_sky(t_env *e, int direction)
{
	int		i;
	int		j;
	int		h;

	j = 0;
	h = 0;
	if (direction == 0)
	{
		e->sky.pos += 30;
		if (e->sky.pos >= WIDTH * e->sky.ratio)
			e->sky.pos = 0;
		i = e->sky.pos;
		while (j < SCREENSIZE)
		{
			e->img_string[j++] = e->sky.data->pix[i++];
			if (j % WIDTH == 0)
				i += WIDTH * e->sky.ratio;
		}
	}
	else
	{
		e->sky.pos -= 30;
		if (e->sky.pos < 0)
			e->sky.pos = WIDTH * e->sky.ratio;
		i = e->sky.pos;
		while (j < SCREENSIZE)
		{
			while (i < 0)
			 	i += HEIGHT * e->sky.data->dim.x;
			e->img_string[j++] = e->sky.data->pix[i++];
			if (j % WIDTH == 0)
				i += WIDTH * e->sky.ratio;
		}
	}
}
