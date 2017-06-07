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

static inline unsigned char	interp(unsigned char b, unsigned char f,
									float ratio)
{
	unsigned char	result;

	result = (unsigned char)((1.f - ratio) * b + ratio * f);
	return (result);
}

static inline t_pix			interp_pix(t_pix b_pix, t_pix f_pix, float ratio)
{
	t_pix	new_pix;

	new_pix.c.a = interp(b_pix.c.a, f_pix.c.a, ratio);
//	new_pix.c.a = 0;

	new_pix.c.b = interp(b_pix.c.b, f_pix.c.b, ratio);
	new_pix.c.g = interp(b_pix.c.g, f_pix.c.g, ratio);
	new_pix.c.r = interp(b_pix.c.r, f_pix.c.r, ratio);
	return (new_pix);
}

static inline t_pix				get_pix(t_bmp *src, t_coord_f c_src)
{
	t_pix		pix;
	t_coord_i	c_src_i;

	c_src_i = (t_coord_i){(int)c_src.x, (int)c_src.y};
	pix = interp_pix(
			interp_pix(
				src->pix[(int)(src->dim.x * c_src_i.y + c_src_i.x)],
				src->pix[(int)(src->dim.x * c_src_i.y + c_src_i.x + 1)],
				c_src.x - c_src_i.x),
			interp_pix(
				src->pix[(int)(src->dim.x * (c_src_i.y + 1) + c_src_i.x)],
				src->pix[(int)(src->dim.x * (c_src_i.y + 1) + c_src_i.x + 1)],
				c_src.x - c_src_i.x),
			c_src.y - c_src_i.y);
	return (pix);
}

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
	while (j < (SCREENSIZE >> 1))
	{
		e->scene.scene[j++] = e->sky->data->pix[i++];
		if (j % WIDTH == 0)
			i += e->sky->ratio * WIDTH;
	}
}

/*
#include <math.h>
#include <stdlib.h>
#include "wolf3d.h"

void				init_sky(t_env *env, char *file_name)
{
	rendering_layer_init(&(env->scene.sky), file_name);
}

static t_coord_f	calc_tex_coord(t_coord_f angle)
{
	t_coord_f	c_sky;

	c_sky.x = angle.x / (2.f * M_PI);
	c_sky.y = angle.y / M_PI;
	if (c_sky.x < 0.f)
		c_sky.x += 1.f;
	if (c_sky.x >= 1.f)
		c_sky.x -= 1.f;
	return (c_sky);
}

static inline float	angle_on_screen(int x)
{
	return (atanf((float)x / (WIDTH / 2)) * (VIEW_ANGLE / 2.f / atanf(1.f)));
}

void				render_sky(t_env *env, t_rendering_layer *layer)
{
	t_coord_i	c;
	t_coord_f	angle;

	layer->n = 0;
	c.y = -1;
	while (++c.y < HEIGHT)
	{
		angle.y = angle_on_screen(HEIGHT / 2 - c.y);
		c.x = -1;
		while (++c.x < WIDTH)
		{
			if (angle.y >= env->scene.columns[c.x].wall_max_angle)
			{
				angle.x = env->scene.columns[c.x].angle_x;
				layer->ij[layer->n] = c;
				layer->uv[layer->n] = calc_tex_coord(angle);
				layer->uv[layer->n].x *= layer->bmp->dim.x - 1;
				layer->uv[layer->n].y *= layer->bmp->dim.y - 1;
				layer->dist[layer->n] = 0.f;
				layer->n++;
			}
		}
	}
	rendering_layer_render(layer);
}
*/
