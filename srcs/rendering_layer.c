/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_layer.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stoupin <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/07 11:30:00 by stoupin           #+#    #+#             */
/*   Updated: 2017/06/07 11:30:02 by stoupin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include <stdlib.h>
#include "wolf3d.h"

static inline t_pix	interp_pix(t_pix b, t_pix f, float ratio)
{
	t_pix	new_pix;

	new_pix.c.a = 0;
	new_pix.c.b = (unsigned char)(ratio * (f.c.b - b.c.b) + b.c.b);
	new_pix.c.g = (unsigned char)(ratio * (f.c.g - b.c.g) + b.c.g);
	new_pix.c.r = (unsigned char)(ratio * (f.c.r - b.c.r) + b.c.r);
	return (new_pix);
}

static inline t_pix	get_pix_complex(t_bmp *src, t_coord_f c_src)
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

	get_pix = (((t_thread_format *)arg)->interpolate) ? &get_pix_complex :
															&get_pix_simple;
	n = ((t_thread_format *)arg)->n;
	bmp = ((t_thread_format *)arg)->bmp;
	layer = ((t_thread_format *)arg)->layer;
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

	get_pix = (((t_thread_format *)arg)->interpolate) ? &get_pix_complex :
															&get_pix_simple;
	n = ((t_thread_format *)arg)->n;
	bmp = ((t_thread_format *)arg)->bmp;
	layer = ((t_thread_format *)arg)->layer;
	while (n--)
	{
		layer->result = get_pix(&bmp[layer->type], layer->uv);
		if (layer->result.i == 0xff00ff)
			layer->result.c.a = 0xff;
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

void				rendering_layer_render(t_rendering_layer *layer,
											int interpolate, int n, t_bmp *bmp)
{
	t_thread_format		format[N_THREAD];
	pthread_t			thread[N_THREAD];
	int					i;
	int					l;
	int					b;

	i = -1;
	b = 0;
	l = n / N_THREAD;
	while (++i < N_THREAD)
	{
		format[i].bmp = bmp;
		format[i].interpolate = interpolate;
		format[i].layer = layer + b;
		b += l;
		if (i == (N_THREAD - 1))
			l += n % N_THREAD;
		format[i].n = l;
		pthread_create(&thread[i], NULL, thread_x_base, &format[i]);
	}
	i = -1;
	while (++i < N_THREAD)
		pthread_join(thread[i], NULL);
}

void				rendering_layer_render_sprite(t_rendering_layer *layer,
											int interpolate, int n, t_bmp *bmp)
{
	t_thread_format		format[N_THREAD];
	pthread_t			thread[N_THREAD];
	int					i;
	int					l;
	int					b;

	i = -1;
	b = 0;
	l = n / N_THREAD;
	while (++i < N_THREAD)
	{
		format[i].bmp = bmp;
		format[i].interpolate = interpolate;
		format[i].layer = layer + b;
		b += l;
		if (i == (N_THREAD - 1))
			l += n % N_THREAD;
		format[i].n = l;
		pthread_create(&thread[i], NULL, thread_x_sprite, &format[i]);
	}
	i = -1;
	while (++i < N_THREAD)
		pthread_join(thread[i], NULL);
}

void				*thread_x_base_put(void *arg)
{
	int					n;
	t_rendering_layer	*layer;
	t_pix				*pix;

	n = ((t_thread_put *)arg)->n;
	layer = ((t_thread_put *)arg)->layer;
	pix = ((t_thread_put *)arg)->pix;
	while (n--)
	{
		pix[WIDTH * layer->ij.y + layer->ij.x] = layer->result;
		layer++;
	}
	pthread_exit(NULL);
}

void				rendering_layer_put(t_pix *pix, t_rendering_layer *layer,
																		int n)
{
	t_thread_put		format[N_THREAD];
	pthread_t			thread[N_THREAD];
	int					i;
	int					l;
	int					b;

	i = -1;
	b = 0;
	l = n / N_THREAD;
	while (++i < N_THREAD)
	{
		format[i].layer = layer + b;
		b += l;
		if (i == (N_THREAD - 1))
			l += n % N_THREAD;
		format[i].n = l;
		format[i].pix = pix;
		pthread_create(&thread[i], NULL, thread_x_base_put, &format[i]);
	}
	i = -1;
	while (++i < N_THREAD)
		pthread_join(thread[i], NULL);
}

void				rendering_layer_put_sprite(t_pix *pix,
											t_rendering_layer *layer, int n)
{
	while (n--)
	{
		pix[WIDTH * layer->ij.y + layer->ij.x].i = layer->result.i *
			(layer->result.c.a != 0xff) + pix[WIDTH * layer->ij.y +
			layer->ij.x].i * (layer->result.c.a == 0xff);
		layer++;
	}
}
