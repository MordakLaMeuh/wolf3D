/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_threads.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 03:19:02 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/12 03:19:06 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "core/wolf3d.h"

static inline t_pix	interp_pix(t_pix b, t_pix f, float ratio)
{
	t_pix	new_pix;

	new_pix.c.a = 0;
	new_pix.c.b = (unsigned char)(ratio * (f.c.b - b.c.b) + b.c.b);
	new_pix.c.g = (unsigned char)(ratio * (f.c.g - b.c.g) + b.c.g);
	new_pix.c.r = (unsigned char)(ratio * (f.c.r - b.c.r) + b.c.r);
	return (new_pix);
}

static inline t_pix	pix_complex(t_bmp *src, t_coord_f c_src)
{
	t_pix		corners[4];
	t_coord_i	c_src_i;
	t_coord_f	delta;
	int			i;

	c_src_i = (t_coord_i){(int)c_src.x, (int)c_src.y};
	i = src->dim.x * c_src_i.y + c_src_i.x;
	corners[0] = src->pix[i];
	corners[1] = src->pix[i + 1];
	i += src->dim.x;
	corners[2] = src->pix[i];
	corners[3] = src->pix[i + 1];
	delta = (t_coord_f){c_src.x - c_src_i.x, c_src.y - c_src_i.y};
	return (interp_pix(
			interp_pix(corners[0], corners[1], delta.x),
			interp_pix(corners[2], corners[3], delta.x),
			delta.y));
}

static inline t_pix	get_pix_simple(t_bmp *src, t_coord_f c_src)
{
	t_coord_i	c_src_i;
	int			i;

	c_src_i = (t_coord_i){(int)c_src.x, (int)c_src.y};
	i = src->dim.x * c_src_i.y + c_src_i.x;
	return (src->pix[i]);
}

void				*thread_x_base(void *arg)
{
	t_pix				(*get_pix)(t_bmp *, t_coord_f);
	int					n;
	t_rendering_layer	*layer;
	t_bmp				*bmp;
	float				dist;

	get_pix = (((t_th_format *)arg)->inter) ? &pix_complex :
															&get_pix_simple;
	n = ((t_th_format *)arg)->n;
	bmp = ((t_th_format *)arg)->bmp;
	layer = ((t_th_format *)arg)->layer;
	while (n--)
	{
		layer->result = get_pix(&bmp[layer->type], layer->uv);
		if ((dist = layer->dist) > SHADOW_LIMIT)
		{
			dist = SHADOW_LIMIT / dist;
			layer->result.c.b *= dist;
			layer->result.c.g *= dist;
			layer->result.c.r *= dist;
		}
		layer++;
	}
	pthread_exit(NULL);
}

void				*thread_x_sprite(void *arg)
{
	t_pix				(*get_pix)(t_bmp *, t_coord_f);
	int					n;
	t_rendering_layer	*layer;
	t_bmp				*bmp;
	float				dist;

	get_pix = (((t_th_format *)arg)->inter) ? &pix_complex : &get_pix_simple;
	n = ((t_th_format *)arg)->n;
	bmp = ((t_th_format *)arg)->bmp;
	layer = ((t_th_format *)arg)->layer;
	while (n--)
	{
		layer->result = get_pix(&bmp[layer->type], layer->uv);
		if (layer->result.i == 0xff00ff)
		{
			layer->result.c.a = 0xff;
			layer++;
			continue;
		}
		if ((dist = layer->dist) > SHADOW_LIMIT)
		{
			dist = SHADOW_LIMIT / dist;
			layer->result.c.b *= dist;
			layer->result.c.g *= dist;
			layer->result.c.r *= dist;
		}
		layer++;
	}
	pthread_exit(NULL);
}
