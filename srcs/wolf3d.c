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

static int		move(t_env *e)
{
	static int		redraw = TRUE;

	redraw |= common_action(e);
	redraw |= move_player(e);
	if (!redraw)
		return (0);
	render_scene(e);
	draw_minimap(e);
	draw_weapon(e);
	scene_to_win(e);
	mlx_put_image_to_window(e->mlx, e->win, e->image, 0, 0);
	redraw = FALSE;
	return (0);
}

int				main(int argc, char **argv)
{
	t_env		env;
	t_tile		**map;

	ft_bzero(&env, sizeof(t_env));
	if (argc != 2)
		return (err_usage(argv[0]));
	if ((init_mlx(&env, "Wolf3D -^^,--,~", WIDTH * NOSTALGIA_FACTOR,
											HEIGHT * NOSTALGIA_FACTOR)))
		return (err_msg("Error during initialisation"));
	if (!(map = get_map(argv[1], &env)))
		return (err_usage(argv[0]));
	if (DEBUG_MAP)
		view_map(map, env.map.size.x, env.map.size.y);
	env.player.angle = -M_PI / 2;
	env.player.height = 2.f;
	create_mlx_image(&env);
	init_sky(&env, "images/2157a.bmp");
	init_scene(&env);
	init_minimap(&env);
	mlx_hook(env.win, X11_KEY_RELEASE, 0xFF, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, 0xFF, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);
	return (0);
}
