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
#include "core/wolf3d.h"
#include "parse/parse.h"
#include "overlay/overlay.h"
#include "render2/render.h"

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
	(t_coord_i){WIDTH / 2 + 10, HEIGHT / 2 + 10}, pix, e->scene.scene);
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
	int		i;

	create_mlx_image(e);
	init_sky(e, "images/astro.bmp");
	init_floor(e, (char*[]){"images/parquet.bmp",
						"images/seamless_carpet.bmp", "images/Brick2.bmp"}, 3);
	init_walls(e, (char*[]){"images/mur.bmp", "images/Brick2.bmp",
								"images/pig.bmp", "images/panic.bmp"}, 4);
	init_sprites(e, (char*[]){"images/pig_2.bmp", "images/pig_2.bmp",
						"images/pig_2.bmp", "images/pig_2.bmp"}, 4);
	init_scene(e);
	i = 0;
	while (i < HEIGHT)
	{
		e->angle_y[i] = angle_on_screen((HEIGHT / 2) - i);
		e->dist_floor[i] = e->player.height / tanf(-e->angle_y[i]);
		e->atan_list[i] = tanf(e->angle_y[i]);
		i++;
	}
	i = -1;
	while (++i < WIDTH)
	{
		e->angle_x[i] = angle_on_screen(i - (WIDTH / 2));
		e->cos_list[i] = cosf(e->angle_x[i]);
	}
}

static int			get_parse(t_env *e, char *filename)
{
	t_sprite_info	*s_l;
	int				i;

	if (load_map(filename) != 0)
		return (err_msg("bad file"));
	if (get_player_location(&e->player.location, '%') != 0)
		return (err_msg("no player in the map !"));
	ft_printf("location_player.x = %i, location_player.y = %i\n",
					(int)e->player.location.x, (int)e->player.location.y);
	e->n_sprites = get_nbr_sprites();
	s_l = get_sprites(e->n_sprites);
	if (!(e->sprites = (t_sprite*)malloc(sizeof(t_sprite) * e->n_sprites)))
		exit(EXIT_FAILURE);
	i = -1;
	while (++i < e->n_sprites)
	{
		ft_printf("sprite type %i: x = %i && y = %i\n", s_l->type,
							(int)s_l->location.x, (int)s_l->location.y);
		e->sprites[i].location = s_l->location;
		e->sprites[i].type = (s_l++)->type;
	}
	ft_printf("verif = %i\n", verif_texture_range(3, 4, 3) ? "KO" : "OK");
	e->map_tiles = (t_tile **)get_map_struct(&e->map.size.y, &e->map.size.x);
	free_map_content();
	return (0);
}

int					main(int argc, char **argv)
{
	t_env			env;

	ft_bzero(&env, sizeof(t_env));
	if (argc != 2)
		return (err_usage(argv[0]));
	if ((init_mlx(&env, "Wolf3D -^^,--,~", WIDTH * NOSTALGIA_FACTOR,
											HEIGHT * NOSTALGIA_FACTOR)))
		return (err_msg("Error during initialisation"));
	if (get_parse(&env, argv[1]))
		return (-1);
	view_map(env.map_tiles, env.map.size.x, env.map.size.y);
	init_sprite_ai(&env);
	env.wall_height = 3.f;
	env.sprite_height = 2.5f;
	env.player.angle = 6.f / 4 * PI;
	env.player.height = 2.f;
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
