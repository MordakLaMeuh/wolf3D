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

static int		err_usage(char *cmd)
{
	ft_eprintf("Illegal map!\n"
		"usage: %s [input_file]\n", cmd, cmd);
	return (EXIT_FAILURE);
}

static int		err_msg(char *msg)
{
	ft_eprintf("Fatal error: %s\nYou should made an other try!\n", msg);
	return (EXIT_FAILURE);
}

/*
** draw_weapon(e);
*/

static int		move(t_env *e)
{
	common_action(e);
	move_player(e);
	render_scene(e);
	if (e->display_minimap)
		draw_minimap(e);
	scene_to_win(e);
	mlx_put_image_to_window(e->mlx, e->win, e->image, 0, 0);
	eval_fps(e);
	return (0);
}

static void		init_all(t_env *e)
{
	int i;

	init_sky(e, "images/astro.bmp");
	init_floor(e, (char*[]){"images/parquet.bmp"}, 1);
	init_walls(e, (char*[]){"images/mur.bmp", "images/pig.bmp",
														"images/panic.bmp"}, 3);
	init_scene(e);
	init_minimap(e);
	i = 0;
	while (i < HEIGHT)
	{
		e->angle_y[i] = (atanf((float)((HEIGHT / 2) - i) / (WIDTH / 2))
										* (VIEW_ANGLE / 2.f / atanf(1.f)));
		e->dist_floor[i] = e->player.height / tanf(-e->angle_y[i]);
		e->atan_list[i] = tanf(e->angle_y[i]);
		i++;
	}
}

int				main(int argc, char **argv)
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
	env.player.angle = 3 * M_PI / 2;
	env.player.height = 2.f;
	env.player.location = (t_coord_f){env.map.size.x / 2., env.map.size.y / 2.};
	env.display_minimap = TRUE;
	create_mlx_image(&env);
	init_all(&env);
	mlx_hook(env.win, X11_KEY_RELEASE, 0xFF, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, 0xFF, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);
	return (0);
}
