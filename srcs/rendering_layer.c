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

void				rendering_layer_render(t_rendering_layer *layer,
											int interpolate, int n, t_bmp *bmp)
{
	float				dist;
	t_pix				(*get_pix)(t_bmp *, t_coord_f);

	get_pix = (interpolate) ? &get_pix_complex : &get_pix_simple;
	while (n--)
	{
		layer->result = get_pix(&bmp[layer->type], layer->uv);
		if ((dist = layer->dist) > SHADOW_LIMIT)
		{
			dist =  SHADOW_LIMIT / dist;
			layer->result.c.b *= dist;
			layer->result.c.g *= dist;
			layer->result.c.r *= dist;
		}
		layer++;
	}
}

void				rendering_layer_render_sprite(t_rendering_layer *layer,
											int interpolate, int n, t_bmp *bmp)
{
	float	dist;
	t_pix	(*get_pix)(t_bmp *, t_coord_f);

	get_pix = (interpolate) ? &get_pix_complex : &get_pix_simple;
	while (n--)
	{
		layer->result = get_pix_simple(&bmp[layer->type], layer->uv);
		dist = layer->dist;
		if (layer->result.i == 0xff00ff)
			layer->result.c.a = 0xff;
		if (dist > SHADOW_LIMIT)
		{
			dist = SHADOW_LIMIT / dist;
			layer->result.c.b *= dist;
			layer->result.c.g *= dist;
			layer->result.c.r *= dist;
		}
		layer++;
	}
}

void				rendering_layer_put(t_pix *pix, t_rendering_layer *layer,
																		int n)
{
	while (n--)
	{
		pix[WIDTH * layer->ij.y + layer->ij.x] = layer->result;
		layer++;
	}
}

void				rendering_layer_put_sprite(t_pix *pix,
											t_rendering_layer *layer, int n)
{
	while (n--)
	{
		pix[WIDTH * layer->ij.y + layer->ij.x].i = layer->result.i * (layer->result.c.a != 0xff) +
							pix[WIDTH * layer->ij.y + layer->ij.x].i * (layer->result.c.a == 0xff);
		layer++;
	}
}
