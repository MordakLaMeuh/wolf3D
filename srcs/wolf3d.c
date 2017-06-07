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

#include <sys/time.h>

static void		eval_fps(t_env *e)
{
	static int 				count = 0;
	static struct timeval 	start;
	struct timeval			stop;
	static char 			*s = NULL;

	if (count == 0)
	{
		gettimeofday(&start, NULL);
		count++;
	}
	else
	{
		gettimeofday(&stop, NULL);
		if ((stop.tv_sec - start.tv_sec) == 0)
			count++;
		else
		{
			if (s)
				free(s);
			s = ft_itoa(count);
			count = 0;
		}
	}
	mlx_string_put(e->mlx, e->win, 20, 20, 0x00FFFFFF, "FPS:");
	if (s)
		mlx_string_put(e->mlx, e->win, 70, 20, 0x00FFFFFF, s);
}


static int		move(t_env *e)
{
	static int		redraw = TRUE;

	redraw |= common_action(e);
	redraw |= move_player(e);

//	if (!redraw)
//		return (0);

	render_scene(e);
	draw_minimap(e);
	//draw_weapon(e);
	scene_to_win(e);

	mlx_put_image_to_window(e->mlx, e->win, e->image, 0, 0);
	eval_fps(e);
	redraw = FALSE;
	return (0);
}

static void		init_all(t_env *e)
{
	init_floor(e, "images/parquet.bmp");
	init_sky(e, "images/astro2.bmp");
	init_walls(e, "images/mur.bmp");
	init_scene(e);
	init_minimap(e);
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
	create_mlx_image(&env);
	init_all(&env);
	mlx_hook(env.win, X11_KEY_RELEASE, 0xFF, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, 0xFF, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);
	return (0);
}
