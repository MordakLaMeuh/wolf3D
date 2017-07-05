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

typedef struct			s_column
{
	float				wall_h_dist;
	float				wall_x_tex;
	float				wall_min_angle;
	float				wall_max_angle;
	int					type;
}						t_column;

typedef struct			s_rendering_layer
{
	t_coord_i			ij;
	t_coord_f			uv;
	int					type;
	float				dist;
	t_pix				result;
}						t_rendering_layer;

typedef struct			s_scene
{
	t_bmp				*bmp_wall;
	t_bmp				*bmp_floor;
	t_bmp				*bmp_sprite;
	int					n_layer_wall;
	int					n_layer_floor;
	int					n_layer_sprite;
	t_rendering_layer	*wall;
	t_rendering_layer	*floor;
	t_rendering_layer	*sprites;
	t_column			*columns;
	t_pix				*scene;
}						t_scene;

#endif
