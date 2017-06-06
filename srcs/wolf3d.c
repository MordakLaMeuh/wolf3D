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
	redraw |= modify_sky(e);
	if (!redraw)
		return (0);
	draw_weapon(e);
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
	if ((init_mlx(&env, "Wolf3D -^^,--,~", WIDTH, HEIGHT)))
		return (err_msg("Error during initialisation"));
	if (!(map = get_map(argv[1], &env)))
		return (err_usage(argv[0]));
	if (DEBUG_MAP)
		view_map(map, env.map.size_x, env.map.size_y);
	create_mlx_image(&env);
	init_sky(&env, "images/2157a.bmp");
	mlx_hook(env.win, X11_KEY_RELEASE, 0xFF, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, 0xFF, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);
	return (0);
}
