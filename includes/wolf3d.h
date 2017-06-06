/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 15:43:32 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/03 15:46:33 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include "mlx.h"
# include "libft.h"

# define DEBUG_KEYBOARD		FALSE
# define DEBUG_MAP			TRUE

# define WIDTH		1920
# define HEIGHT		1080
# define SCREENSIZE			(WIDTH * HEIGHT)

# define X11_KEY_RELEASE		3
# define X11_KEY_PRESS			2
# define X11_DESTROY_NOTIFY		17
# define X11_BUTTON_4			4
# define X11_MOTION_NOTIFY		6

# define KEYB_1				18
# define KEYB_2				19
# define KEYB_3				20
# define KEYB_4				21
# define KEYB_5				23
# define KEYB_6				22
# define KEYB_7				26
# define KEYB_8				28
# define KEYB_9				26
# define KEYB_C				8
# define KEYB_M				46
# define KEYB_P				35
# define KEYB_R				15
# define KEYB_HELP			44
# define KEYB_PLUS			69
# define KEYB_MINUS			78
# define KEYB_ESCAPE		53
# define KEYB_MMO_W			13
# define KEYB_MMO_S			1
# define KEYB_MMO_A			0
# define KEYB_MMO_D			2
# define KEYB_ARROW_UP		126
# define KEYB_ARROW_DOWN	125
# define KEYB_ARROW_LEFT	123
# define KEYB_ARROW_RIGHT	124

# define N_CONTROL			4

typedef struct			s_coord_i
{
	int					x;
	int					y;
}						t_coord_i;

typedef struct			s_coord_f
{
	float				x;
	float				y;
}						t_coord_f;

typedef struct			s_line
{
	t_coord_i			p1;
	t_coord_i			p2;
	int					dx;
	int					dy;
	int					b_color;
	int					f_color;
}						t_line;

typedef struct			s_bmp
{
	t_coord_i			dim;
	int					*pix;
}						t_bmp;

typedef struct			s_perso
{
	t_coord_f			location;
	t_coord_f			cam;
}						t_perso;

/*
** size_x -> longueur de la carte.
** size_y -> hauteur de la carte.
** scale -> echelle pour les petites cartes, varie de 1 a 10.
** Par convention, une map ne peut etre plus petite que 1 * 1 !
** au mieux, 10 * 10 cases sont affichees.
** le joueur est centre au millieu sauf s'il va vers un bord.
** la carte est centree.
*/

typedef struct			s_map
{
	int					size_x;
	int					size_y;
	float				scale;


}						t_map;

typedef struct			s_sky
{
	int					ratio;
	int					pos;
	t_bmp				*data;
}						t_sky;

typedef struct			s_weapon
{
	int					i;
	int					j;
	t_bmp				*data;
	t_bmp				*data_2;
}						t_weapon;

typedef struct s_env	t_env;

struct					s_env
{
	void				*mlx;
	void				*win;
	void				*image;
	int					bpp;
	int					endian;
	int					s_l;

	t_bmp				*pix;

	t_sky				sky;							// image 360 du ciel.
	t_perso				p;								// donnees du bonhomme.
	t_map				map;							// infos sur la map.
	t_weapon			weapon;
	int					*img_string;
	char				keyb[256];
};

typedef struct			s_tile
{
	int	value;
}						t_tile;

int				get_clrs(t_bmp *src, t_coord_f c_src);

t_bmp			*load_bitmap(char **name, int n);
void			copy_img(t_bmp *dst, t_bmp *src);

void			init_sky(t_env *e, char *file_name);
void			move_sky(t_env *e, int direction);

int				init_mlx(t_env *env, char *window_name, int width, int height);
int				create_mlx_image(t_env *e);
void			set_mlx_image_bg_color(t_env *e, int color);
int				exit_mlx(t_env *e);

int				mlx_key_release(int keycode, t_env *e);
int				mlx_key_press(int keycode, t_env *e);
int				common_action(t_env *e);

int				ft_secure_atoi_spaces(const char *nptr, int *error);
t_tile			**get_map(char *file, t_env *env);
void			view_map(t_tile **map, int width, int height);

void			modify_minimap(t_env *e, int direction);
int				modify_sky(t_env *e);

void			draw_line(t_env *env, t_line *p);
void			draw_box(t_coord_i p1, t_coord_i p2, int color, t_env *e);
void			fill_box(t_coord_i p1, t_coord_i p2, int color, t_env *e);

void			draw_weapon(t_env *e);
#endif
