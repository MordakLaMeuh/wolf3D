/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 23:43:10 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 04:30:56 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "wolf3d.h"
#include "bmp.h"

/*
** Vue size : ((WIDTH * HEIGHT) * VIEW_ANGLE) / 360
** Example : (1080*1920*66)/360 = 380160
** Total size : (1080 * 1920 * 360) / 66 = 11310545
** Total HEIGHT = 1080
** Total WIDTH = (1920 * 360) / 66 = 10472
*/

void			init_sky(t_env *e, char *file_name)
{
	t_bmp	*sky_bmp;
	int		i;
	int		j;

	if (!(e->sky.data = malloc(sizeof(t_bmp))))
		exit (EXIT_FAILURE);
	sky_bmp = load_bitmap((char*[]){file_name}, 1);
	sky_bmp = &sky_bmp[0];
	e->sky.ratio = 360 / 60;
	e->sky.data->dim.x = WIDTH * e->sky.ratio;
	e->sky.data->dim.y = HEIGHT;
	if (!(e->sky.data->pix = (int *)malloc
								(e->sky.data->dim.x * HEIGHT * sizeof(int))))
		exit (EXIT_FAILURE);
	copy_img(e->sky.data, sky_bmp);
	free(sky_bmp->pix);
	free(sky_bmp);
	i = 0;
	j = 0;
	while (j < SCREENSIZE)
	{
		e->img_string[j++] = e->sky.data->pix[i++];
		if (j % WIDTH == 0)
			i += WIDTH * (e->sky.ratio - 1);
	}
}

void			move_sky(t_env *e, int direction)
{
	int		i;
	int		j;
	int		h;

	j = 0;
	h = 0;
	if (direction == 0)
	{
		e->sky.pos += 10;
//		if (e->sky.pos >= WIDTH * (e->sky.ratio - 1))
//			e->sky.pos = 0;
		i = e->sky.pos;
		while (j < SCREENSIZE)
		{
			e->img_string[j++] = e->sky.data->pix[i++];
			if (j % WIDTH == 0)
				i += WIDTH * (e->sky.ratio - 1);
		}
	}
	else
	{
		e->sky.pos -= 10;
	//	if (e->sky.pos < 0)
	//		e->sky.pos = WIDTH * (e->sky.ratio - 1);
		i = e->sky.pos;
		while (j < SCREENSIZE)
		{
			while (i < 0)
			 	i += HEIGHT * e->sky.data->dim.x;
			e->img_string[j++] = e->sky.data->pix[i++];
			if (j % WIDTH == 0)
				i += WIDTH * (e->sky.ratio - 1);
		}
	}
}
