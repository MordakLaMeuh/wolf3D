/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   internal_render.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 11:57:54 by bmickael          #+#    #+#             */
/*   Updated: 2017/07/05 11:57:56 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INTERNAL_RENDER_H
# define INTERNAL_RENDER_H

# include "graphic_types.h"

typedef struct			s_floor_uv_format
{
	t_rendering_layer	*layer;
	int					n;
	t_env				*e;
}						t_floor_uv_format;

#endif
