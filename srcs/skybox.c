/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 23:43:10 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 00:49:01 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "wolf3d.h"
#include "bmp.h"

void			init_sky(t_env *e, char *file_name)
{
	t_bmp	*sky_bmp;

	sky_bmp = load_bitmap((char*[]){file_name}, 1);
	(void)e;
}

void			move_sky(t_env *e, int direction)
{
	ft_printf("New direction = %i\n", direction);
	(void)e;
	(void)direction;
}
