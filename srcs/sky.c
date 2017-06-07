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

static void		copy_img(t_bmp *dst, t_bmp *src, int new_dim_x)
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
				dst->pix[new_dim_x * c_dst.y + c_dst.x] =
														get_pix_sp(src, c_src);
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
	copy_img(e->sky->data, sky_bmp, WIDTH * (e->sky->ratio + 1));
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
