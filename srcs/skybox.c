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
	int		i;
	int		j;
	int		h;
	int		ratio;
	t_bmp	*sky_bmp;

	sky_bmp = load_bitmap((char*[]){file_name}, 1);

	sky_bmp = &sky_bmp[0];

	t_bmp		*img_f;

	img_f = malloc(sizeof(t_bmp));
	img_f->dim.x = ((WIDTH * 360) / 60);
	img_f->dim.y = HEIGHT;
	img_f->pix = (int *)malloc(img_f->dim.x * HEIGHT * 4);
	copy_img(img_f, sky_bmp);
	e->sky.sky = img_f;
	e->sky.ratio = 360 / 60;
	i = 0;
	j = 0;
	h = 0;
	ratio = 360 / 60;
	while (j < (WIDTH * HEIGHT))
	{
		if (j > 0 && j % (WIDTH) == 0)
			i = ++h * ratio * WIDTH;
		e->img_string[j] = img_f->pix[i];
		1 ? j++, i++ : (0);
		if (i >= (img_f->dim.x * HEIGHT))
			break;
	}
}

void			move_sky(t_env *e, int direction)
{
//	static t_sky sky = (t_sky){6, 0, 0, NULL};
	int		i;
	int		j;
	int		h;

	j = 0;
	h = 0;
	i = e->sky.pos;
	//i < 0 ? i +=
//	ft_printf("New direction = %i\n", direction);
	if (direction == 0)
	{
		while (j < (WIDTH * HEIGHT))
		{
			if (j > 0 && j % (WIDTH) == 0)
				i = (++h * e->sky.ratio * WIDTH) + e->sky.pos;
			e->img_string[j] = e->sky.sky->pix[i];
			1 ? j++, i++ : (0);
			if (i >= (e->sky.sky->dim.x * HEIGHT))
				break;
		}
		e->sky.pos += 10;
	}
	if (direction == 1)
	{
		while (j < (WIDTH * HEIGHT))
		{
			if (j > 0 && j % (WIDTH) == 0)
				i = (++h * e->sky.ratio * WIDTH) + e->sky.pos;
			if (j < 0 || i < 0)
				ft_printf("j=%d i=%d\n", j, i);
			/*while (i < 0)
			{
				//i += (e->sky.sky->dim.x * HEIGHT);
				ft_printf("j=%d i=%d / sizeof pix %d\n", j, i, sizeof(e->sky.sky->pix));
			}*/
			e->img_string[j] = e->sky.sky->pix[i];
			1 ? j++, i++ : (0);
			if (i >= (e->sky.sky->dim.x * HEIGHT))
				break;
		}
		e->sky.pos -= 10;
	}
}

/*
void			move_sky(t_env *e, int direction)
{
	int	i;
	int	j;
	int	h;

	j = 0;
	i = e->sky.pos;
	ft_printf("New direction = %i\n", direction);
	if (direction == 0)
	{
		while (j < (WIDTH * HEIGHT))
		{
			if (j > 0 && j % (WIDTH) == 0)
				i = ++h * ratio * WIDTH;
			e->img_string[j] = img_f->pix[i];
			1 ? j++, i++ : (0);
			if (i >= (img_f->dim.x * HEIGHT))
				break;
		}
	}
	(void)e;
}
*/
