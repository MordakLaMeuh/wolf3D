/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 15:42:11 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/03 16:14:38 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <stdlib.h>
#include "wolf3d.h"

static int			err_usage(char *cmd)
{
	ft_eprintf("Illegal map!\n"
		"usage: %s [input_file]\n", cmd, cmd);
	return (EXIT_FAILURE);
}

static int			err_msg(char *msg)
{
	ft_eprintf("Fatal error: %s\nYou should made an other try!\n", msg);
	return (EXIT_FAILURE);
}

/*
** draw_weapon(e);
*/

static int			move(t_env *e)
{
	t_pix				pix;
	unsigned long int	m;
	char *s;

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
	m = get_time();
	if (e->interpolate_time)
	{
		if ((m - e->interpolate_time) < 1000)
		{
			ft_asprintf(&s, "Interpolation lineaire: %s", (e->interpolate_state) ? "ON" : "OFF");
			mlx_string_put(e->mlx, e->win, 20, 40, 0x00FFFFFF, s);
			free(s);
		}
		else
			e->interpolate_time = 0;
	}
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

	init_sky(e, "images/astro.bmp");
	init_floor(e, (char*[]){"images/parquet.bmp"}, 1);
	init_walls(e, (char*[]){"images/mur.bmp", "images/pig.bmp",
								"images/panic.bmp", "images/brique2.bmp"}, 4);
	init_sprites(e, (char*[]){"images/sprite_pig.bmp"}, 1);
	init_scene(e);
	init_minimap(e);
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
	if (DEBUG_MAP)
		view_map(env.map_tiles, env.map.size.x, env.map.size.y);
	env.wall_height = 3.f;
	env.sprite_height = 2.5f;
	env.player.angle = 6.f / 4 * M_PI;
	env.player.height = 2.f;
	env.player.location = (t_coord_f){env.map.size.x / 2., env.map.size.y / 2.};
	env.display_minimap = TRUE;
	env.interpolate_state = TRUE;
	create_mlx_image(&env);
	init_all(&env);
	mlx_hook(env.win, X11_KEY_RELEASE, 0xFF, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, 0xFF, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);
	return (0);
}
