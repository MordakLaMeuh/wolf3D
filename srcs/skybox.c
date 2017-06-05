/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skybox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/05 23:43:10 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/05 23:43:12 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "wolf3d.h"
#include "bmp.h"

void			init_sky(char *file_name)
{
	(void)file_name;
}

void			move_sky(int *img_string, int direction)
{
	ft_printf("New direction = %i\n", direction);
	(void)img_string;
	(void)direction;
}
