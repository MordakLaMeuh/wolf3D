/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/10 01:46:12 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/10 01:46:37 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

# include <math.h>
# include "mlx.h"
# include "libft.h"

# define DEBUG_KEYBOARD		FALSE
# define DEBUG_MAP			TRUE

# define NOSTALGIA_FACTOR	1
# define WIDTH				(1920 / NOSTALGIA_FACTOR)
# define HEIGHT				(1080 / NOSTALGIA_FACTOR)
# define SCREENSIZE			(WIDTH * HEIGHT)

# define RATIO				4
# define VIEW_ANGLE			(2.f * M_PI / RATIO)
# define SHADOW_LIMIT		7

# define MAP_ORIGIN_X			(WIDTH - 120)
# define MAP_ORIGIN_Y			120
# define MAP_RADIUS				100
# define MAP_DEPTH				10
# define LONG_ARROW_RADIUS		30
# define ARROW_RADIUS			15

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

typedef union			u_pix
{
	unsigned int		i;
	struct				s_c
	{
		unsigned char	b;
		unsigned char	g;
		unsigned char	r;
		unsigned char	a;
	}					c;
}						t_pix;

typedef struct			s_line
{
	t_coord_i			p1;
	t_coord_i			p2;
	t_coord_i			d;
	t_pix				b_pix;
	t_pix				f_pix;
}						t_line;

typedef struct			s_bmp
{
	t_coord_i			dim;
	t_pix				*pix;
}						t_bmp;

typedef struct			s_player
{
	t_coord_f			location;
	float				angle;
	float				height;
}						t_player;

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
	t_coord_i			size;
	float				scale;
}						t_map;

typedef struct			s_weapon
{
	int					i;
	int					j;
	t_bmp				*data;
	t_bmp				*data_2;
}						t_weapon;

typedef struct s_env	t_env;

typedef struct			s_tile
{
	int	value;
}						t_tile;

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

typedef struct			s_sprite
{
	int					type;
	t_coord_f			location;
}						t_sprite;

typedef struct			s_sky
{
	int					pos;
	t_bmp				*data;
}						t_sky;

typedef struct			s_vector_2
{
	float				dx;
	float				dy;
	float				module;
}						t_vector_2;

typedef struct			s_wall_vector
{
	t_vector_2			v;
	t_coord_f			norm;
}						t_wall_vector;

typedef struct			s_wall_info
{
	t_coord_f			ray_pos;
	t_coord_f			ray_dir;
	t_coord_i			map;
	t_coord_i			step;
	t_coord_f			delta_dist;
	t_coord_f			side_dist;
	t_wall_vector		w;
}						t_wall_info;

struct					s_env
{
	void				*mlx;
	void				*win;
	void				*image;
	int					bpp;
	int					endian;
	int					s_l;
	int					display_minimap;
	t_player			player;
	t_map				map;
	t_weapon			weapon;
	t_sky				*sky;
	t_pix				*img_string;
	unsigned long int	keyb[256];
	float				wall_height;
	float				sprite_height;
	t_tile				**map_tiles;
	float				angle_x[WIDTH];
	float				angle_y[HEIGHT];
	float				dist_floor[HEIGHT];
	float				atan_list[HEIGHT];
	float				cos_list[WIDTH];
	t_scene				scene;
	int					interpolate_state;
	unsigned long int	interpolate_time;
	int					n_sprites;
	t_sprite			*sprites;
};

typedef struct			s_modify_coord
{
	int					keycode_1;
	int					keycode_2;
	float				q;
	float				l;
}						t_modify_coord;

# define N_THREAD		4

typedef struct			s_thread_format
{
	int					n;
	int					interpolate;
	t_bmp				*bmp;
	t_rendering_layer	*layer;
}						t_thread_format;

typedef struct			s_thread_put
{
	int					n;
	t_rendering_layer	*layer;
	t_pix				*pix;

}						t_thread_put;

t_pix					get_pix_sp(t_bmp *src, t_coord_f c_src);
float					dist(t_coord_f a, t_coord_f b);

t_bmp					*load_bitmap(char **name, int n);

int						init_mlx(t_env *env, char *window_name, int width,
																	int height);
int						create_mlx_image(t_env *e);
void					set_mlx_image_bg_color(t_env *e, t_pix color);
int						exit_mlx(t_env *e);

int						mlx_key_release(int keycode, t_env *e);
int						mlx_key_press(int keycode, t_env *e);
int						common_action(t_env *e);

int						ft_secure_atoi_spaces(const char *nptr, int *error);
t_tile					**get_map(char *file, t_env *env);

void					view_map(t_tile **map, int width, int height);
void					eval_fps(t_env *e);

void					init_minimap(t_env *e);
void					draw_minimap(t_env *e);

int						move_player(t_env *e);

void					draw_arrow(t_env *e, t_coord_i c, float angle);
void					draw_line(t_env *env, t_line *p);
void					draw_box(t_coord_i p1, t_coord_i p2, t_pix pix,
																	t_env *e);
void					fill_box(t_coord_i p1, t_coord_i p2, t_pix pix,
																	t_env *e);
void					draw_circle(t_env *e, t_coord_i position, int radius,
																t_pix color);

void					init_floor(t_env *e, char **textures, int n);
void					render_floor(t_env *env, t_rendering_layer *layer);

void					init_sky(t_env *e, char *file_name);
void					render_sky(t_env *env, float angle);

void					init_walls(t_env *e, char **textures, int n);
int						find_wall(t_env *env, float angle_x,
											t_coord_f *intersect, float *x_tex);
void					render_wall(t_env *env, t_rendering_layer *layer);

void					init_sprites(t_env *e, char **textures, int n);
void					render_sprites(t_env *env, t_rendering_layer *layer);

void					init_scene(t_env *env);
void					render_scene(t_env *env);
void					scene_to_win(t_env *env);

void					draw_weapon(t_env *e);

void					rendering_layer_render(t_rendering_layer *layer,
											int interpolate, int n, t_bmp *bmp);
void					rendering_layer_render_sprite(t_rendering_layer *layer,
											int interpolate, int n, t_bmp *bmp);

void					rendering_layer_put(t_pix *pix,
											t_rendering_layer *layer, int n);
void					rendering_layer_put_sprite(t_pix *pix,
											t_rendering_layer *layer, int n);
unsigned long int		get_time(void);
#endif
