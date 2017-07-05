/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/05 04:33:29 by bmickael          #+#    #+#             */
/*   Updated: 2017/07/05 04:33:31 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSE_H
# define PARSE_H

# include "libft.h"
# include "graphic_types.h"

typedef struct	s_sprite_info
{
	t_coord_f	location;
	int			type;
}				t_sprite_info;

int				load_map(char *filename);
int				get_player_location(t_coord_f *l, char c);
int				get_nbr_sprites(void);
t_sprite_info	*get_sprites(int n);
int				**get_map_struct(int *height, int *width);
void			free_map_content(void);
int				verif_texture_range(int n_floor, int n_wall, int n_sprite);

#endif
