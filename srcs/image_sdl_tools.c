/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_sdl_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/12 04:40:05 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/10 11:35:51 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include "wolf3d.h"

int			create_mlx_image(t_env *e)
{
	e->image = ft_memalloc(WIDTH * HEIGHT * NOSTALGIA_FACTOR);
	e->img_string = (t_pix*)ft_memalloc(sizeof(t_pix) * 4 * WIDTH * HEIGHT);
	/*
	if (!(e->image = mlx_new_image(e->mlx, WIDTH * NOSTALGIA_FACTOR,
									HEIGHT * NOSTALGIA_FACTOR)))
		return (1);
	if (!(e->img_string = (t_pix*)mlx_get_data_addr(e->image, &e->bpp, &e->s_l,
																&e->endian)))
		return (1);
	*/
	(void)e;
	return (0);
}

void		create_sdl_image(t_env *e)
{
	int		i;
	//t_pix	p;

	i = 0;
	(void)e;
	//SDL_RenderClear(e->img);
	/*
	while (i < SCREENSIZE)
	{
		//ft_printf("%d\n", i);
		p = e->scene.scene[i];
		//SDL_SetRenderDrawColor(e->img, p.c.r, p.c.g, p.c.b, p.c.a);
		//SDL_RenderDrawPoint(e->img, i % WIDTH, i / HEIGHT);
		i++;
	}*/
	//SDL_RenderPresent(e->img);
}

void		set_mlx_image_bg_color(t_env *e, t_pix color)
{
	int		x;
	int		y;

	x = -1;
	SDL_SetRenderDrawColor(e->img, color.c.r, color.c.g, color.c.b, color.c.a);
	while (++x < WIDTH)
	{
		y = -1;
		while (++y < HEIGHT)
			SDL_RenderDrawPoint(e->img, x, y);
	}
}
