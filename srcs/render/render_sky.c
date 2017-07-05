/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sky.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/12 03:05:10 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/12 03:05:25 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "core/wolf3d.h"

void				render_sky(t_env *e, float angle)
{
	int		i;
	int		j;

	e->sky->pos = (int)((RATIO * WIDTH) * (angle / (2.f * M_PI)));
	j = 0;
	i = e->sky->pos;
	while (j < (SCREENSIZE >> 1))
	{
		e->scene.scene[j++] = e->sky->data->pix[i++];
		if (j % WIDTH == 0)
			i += RATIO * WIDTH;
	}
}
