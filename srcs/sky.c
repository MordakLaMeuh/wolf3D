/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 11:51:00 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/06 11:51:02 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include "wolf3d.h"
#include "bmp.h"

/*
** Vue size : ((WIDTH * HEIGHT) * VIEW_ANGLE) / 360
** Example : (1080*1920*66)/360 = 380160
** Total size : (1080 * 1920 * 360) / 66 = 11310545
** Total HEIGHT = 1080
** Total WIDTH = (1920 * 360) / 66 = 10472
*/

static void		copy_img_bis(t_bmp *dst, t_bmp *src, int new_dim_x)
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
				dst->pix[new_dim_x * c_dst.y + c_dst.x] = get_pix(src, c_src);
		}
	}
}

static void		paste_bout(t_pix *data)
{
	int y;
	int x;

	y = 0;
	while (y < HEIGHT)
	{
		x = 6 * WIDTH;
		while (x < (7 * WIDTH))
		{
			data[y * (WIDTH * 7) + x] = data[y * (WIDTH * 7) +
															(x - (6 * WIDTH))];
			x++;
		}
		y++;
	}
}

void			init_sky(t_env *e, char *file_name)
{
	t_bmp	*sky_bmp;

	if (!(e->sky = malloc(sizeof(t_sky))))
		exit(EXIT_FAILURE);
	if (!(e->sky->data = malloc(sizeof(t_bmp))))
		exit(EXIT_FAILURE);
	sky_bmp = load_bitmap((char*[]){file_name}, 1);
	sky_bmp = &sky_bmp[0];
	e->sky->ratio = 360 / 60;
	e->sky->data->dim.x = WIDTH * e->sky->ratio;
	e->sky->data->dim.y = HEIGHT;
	if (!(e->sky->data->pix =
	(t_pix *)ft_memalloc(WIDTH * (e->sky->ratio + 1) * HEIGHT * sizeof(t_pix))))
		exit(EXIT_FAILURE);
	copy_img_bis(e->sky->data, sky_bmp, WIDTH * (e->sky->ratio + 1));
	paste_bout(e->sky->data->pix);
	free(sky_bmp->pix);
	free(sky_bmp);
}

void			render_sky(t_env *e, float angle)
{
	int		i;
	int		j;

	e->sky->pos = (int)((e->sky->ratio * WIDTH) * (angle / (2.f * M_PI)));
	j = 0;
	i = e->sky->pos;
	while (j < SCREENSIZE)
	{
		e->scene[j++] = e->sky->data->pix[i++];
		if (j % WIDTH == 0)
			i += e->sky->ratio * WIDTH;
	}
}

/*
void			init_sky(t_env *env, char *file_name)
{
	env->sky = load_bitmap((char*[]){file_name}, 1);
	if (!env->sky)
		exit(EXIT_FAILURE);
}

void			render_sky(t_env *env, t_coord_i c, t_coord_f angle)
{
	t_coord_f	c_sky;
	t_pix		pix;

	c_sky.x = angle.x * env->sky->dim.x / (2.f * M_PI) + env->sky->dim.x / 2.f;
	c_sky.y = -angle.y * env->sky->dim.x / (2.f * M_PI) + env->sky->dim.y / 2.f;
	if (c_sky.x < 0.f)
		c_sky.x += env->sky->dim.x;
	if (c_sky.x >= env->sky->dim.x)
		c_sky.x -= env->sky->dim.x;
	if (c_sky.y < 0.f)
		c_sky.y += env->sky->dim.y;
	if (c_sky.y >= env->sky->dim.y)
		c_sky.y -= env->sky->dim.y;
	pix = get_pix(env->sky, c_sky);
	env->scene[c.y * WIDTH + c.x] = pix;
}
*/

/*
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
		src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x)].i,
		src->pix[(int)(src->dim.x * (int)c_src.y + (int)c_src.x + 1)].i,
		c_src.x - (int)c_src.x),
				get_color(
		src->pix[(int)(src->dim.x * ((int)c_src.y + 1) + (int)c_src.x)].i,
		src->pix[(int)(src->dim.x * ((int)c_src.y + 1) + (int)c_src.x + 1)].i,
		c_src.x - (int)c_src.x), c_src.y - (int)c_src.y);
	return (color);
}
*/
