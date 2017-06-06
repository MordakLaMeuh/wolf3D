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

	new_pix.c.r = interp(b_pix.c.r, f_pix.c.r, ratio);
	new_pix.c.g = interp(b_pix.c.g, f_pix.c.g, ratio);
	new_pix.c.b = interp(b_pix.c.b, f_pix.c.b, ratio);
	new_pix.c.a = interp(b_pix.c.a, f_pix.c.a, ratio);
	return (new_pix);
}

t_pix						get_pix(t_bmp *src, t_coord_f c_src)
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
