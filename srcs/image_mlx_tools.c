/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_mlx_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 04:40:05 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/03 16:02:54 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

void		set_mlx_image_type(t_env *e)
{
	e->bpp = 32;
	e->endian = 0;
	e->s_l = WIDTH * 4;
}

int			create_mlx_image(t_env *e)
{
	if (!(e->image = mlx_new_image(e->mlx, WIDTH, HEIGHT)))
		return (1);
	if (!(e->img_string = (int *)mlx_get_data_addr(e->image, &e->bpp, &e->s_l,
																&e->endian)))
		return (1);
	return (0);
}

void		set_mlx_image_bg_color(t_env *e, int color)
{
	int		max;
	int		i;

	max = 4 * WIDTH * HEIGHT;
	i = 0;
	while (i < max)
	{
		*(int *)(e->img_string + i) = color;
		i += 4;
	}
}