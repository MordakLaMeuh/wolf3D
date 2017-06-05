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

typedef struct			s_bmp
{
	t_coord_i			dim;
	int					*pix;
}						t_bmp;

typedef struct s_env	t_env;

struct			s_env
{
	void		*mlx;
	void		*win;
	void		*image;
	int			bpp;
	int			endian;
	int			s_l;
	t_bmp		*pix;
	int			*img_string;
	char		keyb[256];
};

t_bmp			*load_bitmap(char **name, int n);
void			copy_img(t_bmp *dst, t_bmp *src);

void			init_sky(char *file_name);
void			move_sky(int *img_string, int direction);

int				init_mlx(t_env *env, char *window_name, int width, int height);
int				create_mlx_image(t_env *e);
void			set_mlx_image_bg_color(t_env *e, int color);
int				exit_mlx(t_env *e);

int				mlx_key_release(int keycode, t_env *e);
int				mlx_key_press(int keycode, t_env *e);
int				common_action(t_env *e);

int				modify_sky(t_env *e);
#endif
