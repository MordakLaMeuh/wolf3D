/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 15:42:11 by bmickael          #+#    #+#             */
/*   Updated: 2017/07/03 12:56:08 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "wolf3d.h"

static void			interpolate_switch(t_env *e, unsigned long int m)
{
	char *s;

	if ((m - e->inter_time) < 1000)
	{
		ft_asprintf(&s, "Interpolation lineaire: %s",
									(e->inter_state) ? "ON" : "OFF");
		mlx_string_put(e->mlx, e->win, 20, 40, 0x00FFFFFF, s);
		free(s);
	}
	else
		e->inter_time = 0;
}

static int			move(t_env *e)
{
	t_pix				pix;

	animate_sprites(e);
	common_action(e);
	move_player(e);
	render_scene(e);
	if (e->display_minimap)
		draw_minimap(e);
	scene_to_win(e);
	pix.i = 0xff0000;
	draw_box((t_coord_i){WIDTH / 2 - 10, HEIGHT / 2 - 10},
	(t_coord_i){WIDTH / 2 + 10, HEIGHT / 2 + 10}, pix, e);
	mlx_put_image_to_window(e->mlx, e->win, e->image, 0, 0);
	if (e->inter_time)
		interpolate_switch(e, get_time());
	eval_fps(e);
	return (0);
}

static inline float	angle_on_screen(int x)
{
	float angle;

	angle = atanf((float)x / (WIDTH / 2) * tanf((float)VIEW_ANGLE / 2.f));
	return (angle);
}

static void			init_all(t_env *e)
{
	int i;

	create_mlx_image(e);
	init_sky(e, "images/astro.bmp");
	init_floor(e, (char*[]){"images/parquet.bmp",
										"images/seamless_carpet.bmp"}, 2);
	init_walls(e, (char*[]){"images/mur.bmp", "images/pig.bmp",
								"images/panic.bmp", "images/Brick2.bmp"}, 4);
	init_sprites(e, (char*[]){"images/sprite_pig.bmp"}, 1);
	init_scene(e);
	i = 0;
	while (i < HEIGHT)
	{
		e->angle_y[i] = angle_on_screen((HEIGHT / 2) - i);
		e->dist_floor[i] = e->player.height / tanf(-e->angle_y[i]);
		e->atan_list[i] = tanf(e->angle_y[i]);
		i++;
	}
	i = 0;
	while (i < WIDTH)
	{
		e->angle_x[i] = angle_on_screen(i - (WIDTH / 2));
		e->cos_list[i] = cosf(e->angle_x[i]);
		i++;
	}
}

int					main(int argc, char **argv)
{
	t_env		env;

	ft_bzero(&env, sizeof(t_env));
	if (argc != 2)
		return (err_usage(argv[0]));
	if ((init_mlx(&env, "Wolf3D -^^,--,~", WIDTH * NOSTALGIA_FACTOR,
											HEIGHT * NOSTALGIA_FACTOR)))
		return (err_msg("Error during initialisation"));
	if (!(env.map_tiles = get_map(argv[1], &env)))
		return (err_usage(argv[0]));
	view_map(env.map_tiles, env.map.size.x, env.map.size.y);
	env.wall_height = 3.f;
	env.sprite_height = 2.5f;
	env.player.angle = 6.f / 4 * M_PI;
	env.player.height = 2.f;
	env.player.location = (t_coord_f){env.map.size.x / 2., env.map.size.y / 2.};
	env.display_minimap = TRUE;
	env.inter_state = TRUE;
	init_all(&env);
	mlx_hook(env.win, X11_KEY_RELEASE, KEYRELEASEMASK, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, KEYPRESSMASK, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);
	return (0);
}
