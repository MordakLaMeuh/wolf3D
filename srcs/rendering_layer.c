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

void				rendering_layer_render(t_rendering_layer *layer,
											int inter, int n, t_bmp *bmp)
{
	t_th_format			format[N_THREAD];
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
		format[i].inter = inter;
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
											int inter, int n, t_bmp *bmp)
{
	t_th_format			format[N_THREAD];
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
		format[i].inter = inter;
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
