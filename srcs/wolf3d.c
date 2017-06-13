/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/03 15:42:11 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/12 08:26:07 by erucquoy         ###   ########.fr       */
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


static inline float	angle_on_screen(int x)
{
	float angle;

	angle = atanf((float)x / (WIDTH / 2) * tanf((float)VIEW_ANGLE / 2.f));
	return (angle);
}

static void		init_all(t_env *e)
{
	int i;

	init_sky(e, "images/astro.bmp");
	init_floor(e, (char*[]){"images/parquet.bmp"}, 1);
	init_walls(e, (char*[]){"images/mur.bmp", "images/pig.bmp",
		"images/panic.bmp", "images/brique2.bmp"}, 4);
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

int			sdl_key_is_pushed(t_env *env)
{
	(void)env;
	return (0);
	/*
	if (env->event.key.keysym.sym == SDLK_a)
		env->keyb[SDLK_a] = 1;
	if (env->event.key.keysym.sym == SDLK_RIGHT)
		env->keyb[SDLK_RIGHT] = 1;
	if (env->event.key.keysym.sym == SDLK_d)
		env->keyb[SDLK_d] = 1;
	if (env->event.key.keysym.sym == SDLK_LEFT)
		env->keyb[SDLK_LEFT] = 1;
	if (env->event.key.keysym.sym == SDLK_w)
		env->keyb[SDLK_w] = 1;
	if (env->event.key.keysym.sym == SDLK_UP)
		env->keyb[SDLK_UP] = 1;
	if (env->event.key.keysym.sym == SDLK_s)
		env->keyb[SDLK_s] = 1;
	if (env->event.key.keysym.sym == SDLK_DOWN)
		env->keyb[SDLK_DOWN] = 1;
	if (env->event.key.keysym.sym == SDLK_x)
		env->keyb[SDLK_x] = 1;
	if (env->event.key.keysym.sym == SDLK_e)
		SDL_SetRelativeMouseMode(1);
	if (env->event.key.keysym.sym == SDLK_r)
		SDL_SetRelativeMouseMode(0);
		*/
}

int			sdl_keyhook(t_env *env)
{
	if (SDL_PollEvent(&(env->event)))
	{
		if (env->event.type == SDL_KEYDOWN && env->event.key.keysym.sym == SDLK_ESCAPE)
			return (1);
		if (env->event.type == SDL_KEYDOWN)
			sdl_key_press(env);
		if (env->event.type == SDL_KEYUP)
			sdl_key_release(env);
		//	return (ft_key_is_released(env));

		//if (env->event.type == SDL_WINDOWEVENT)
		//	ft_win_ev(env);
	}
	return (0);
}

/*static int		move(t_env *e)
{
	t_pix pix;

	common_action(e);
	move_player(e);
	render_scene(e);
	if (e->display_minimap)
		draw_minimap(e);
	scene_to_win(e);
	pix.i = 0xff0000;
	draw_box((t_coord_i){WIDTH / 2 - 10, HEIGHT / 2 - 10},
	(t_coord_i){WIDTH / 2 + 10, HEIGHT / 2 + 10}, pix, e);
	//mlx_put_image_to_window(e->mlx, e->win, e->image, 0, 0);
	eval_fps(e);
	return (0);
}*/



void		sdl_main_loop(t_env *e)
{
	t_pix pix;

	e->surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	printf("up=%d down=%d right=%d left=%d\n", SDLK_UP, SDLK_DOWN, SDLK_RIGHT, SDLK_LEFT);
	printf("SDLK_d=%d SDLK_s=%d SDLK_a=%d SDLK_w=%d\n", SDLK_d, SDLK_s, SDLK_a, SDLK_w);
	while (TRUE)
	{

		/* TEST LINE
		t_pix pixel_white;
		pixel_white.c.r = (Uint8)0xff;
		pixel_white.c.g = (Uint8)0xff;
		pixel_white.c.b = (Uint8)0xff;
		pixel_white.c.a = (Uint8)128;
		int x;
		x = 0;
		while (++x < 200)
			sdl_put_pixel(e->surface,x,x,&pixel_white);
		 END TEST */
		common_action(e);
		move_player(e);
		render_scene(e);
		if (e->display_minimap)
			draw_minimap(e);
		//e->a_time = SDL_GetTicks();

		if (sdl_keyhook(e) == 1)
			break;

		//eval_fps(e);

		pix.i = 0xff00ff;
		eval_fps(e);
		SDL_BlitSurface(e->gun0, NULL, e->surface, &(SDL_Rect){900, 825, 20, 20});
		draw_box((t_coord_i){WIDTH / 2 - 10, HEIGHT / 2 - 10},
						(t_coord_i){WIDTH / 2 + 10, HEIGHT / 2 + 10}, pix, e);
		SDL_BlitSurface(e->surface , NULL, e->screen, NULL);
		SDL_UpdateWindowSurface(e->window);

	}
}



/*
	stop = FALSE;
	while (stop == FALSE)
	{
		while (SDL_PollEvent(&(env->event)))
		{


		//if (ft_sdl_keyhook(env) == 1)
		//	return ;

			if (env->event.type == SDL_KEYDOWN && env->event.key.keysym.sym == SDLK_ESCAPE)
			{
				stop = TRUE;
				break ;
			}

			printf("%i ", env->event.type);
		if (env->gen_random_map == 1)
		{
			env->gen_random_map = 0;
			ft_free_tab(env->map, 16);
			env->map = random_map((int)env->player.posx, (int)env->player.posy);
		}
		ft_print_fps(env);
//		create_sdl_image(env);
		}
	}
	ft_printf("EXIT\n");
}
*/

static void 	f_sdl_init_ttf(t_env *env)
{
	TTF_Init();
	env->font = TTF_OpenFont("fonts/opensans.ttf", 40);
	env->color_white = (SDL_Color){255, 255, 255, 0};
}

static void 	f_sdl_init_sounds(t_env *env)
{
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	env->sound = Mix_LoadWAV("sounds/sound.wav");
	env->fire = Mix_LoadWAV("sounds/gun.wav");
	env->sound_channel = Mix_PlayChannel(-1, env->sound, -1);
	Mix_Playing(env->sound_channel);
}

static void f_sdl_init_image(t_env *env)
{
	IMG_Init(IMG_INIT_PNG);
	env->gun0 = IMG_Load("images/gun.png");
	env->gun1 = IMG_Load("images/gun_fire.png");
	env->in_fire = 0;
}

static void		f_sdl_init(t_env *env)
{
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
	f_sdl_init_ttf(env);
	f_sdl_init_sounds(env);
	env->window = SDL_CreateWindow("Wolfd3D",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE
	);
	env->screen = SDL_GetWindowSurface(env->window);
	env->surface = SDL_LoadBMP("images/loading.bmp");
    SDL_BlitSurface(env->surface , NULL, env->screen, NULL);
    SDL_FreeSurface(env->surface );
    SDL_UpdateWindowSurface(env->window);
    SDL_Delay(5000);
	SDL_ShowCursor(SDL_DISABLE);
	create_mlx_image(env);
	f_sdl_init_image(env);
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
	env.player.angle = 6.f / 4 * M_PI;
	env.player.height = 2.f;
	env.player.location = (t_coord_f){env.map.size.x / 2., env.map.size.y / 2.};
	env.display_minimap = TRUE;
	f_sdl_init(&env);
	env.surface = SDL_CreateRGBSurface(0, WIDTH, HEIGHT, 32, 0, 0, 0, 0);
	init_all(&env);
	sdl_main_loop(&env);
	/*mlx_hook(env.win, X11_KEY_RELEASE, 0xFF, &mlx_key_release, &env);
	mlx_hook(env.win, X11_KEY_PRESS, 0xFF, &mlx_key_press, &env);
	mlx_hook(env.win, X11_DESTROY_NOTIFY, 0xFF, &exit_mlx, &env);
	mlx_loop_hook(env.mlx, &move, &env);
	mlx_loop(env.mlx);*/
	return (0);
}

/*
int main( void )
{
	SDL_Surface *screen; // even with SDL2, we can still bring ancient code back
 SDL_Window *window;
 SDL_Surface *image;

 SDL_Init(SDL_INIT_VIDEO); // init video

 // create the window like normal
 window = SDL_CreateWindow("SDL2 Example", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);

 // but instead of creating a renderer, we can draw directly to the screen
 screen = SDL_GetWindowSurface(window);

 // let's just show some classic code for reference
 image = SDL_LoadBMP("images/brique.bmp"); // loads image
 SDL_BlitSurface(image, NULL, screen, NULL); // blit it to the screen
 SDL_FreeSurface(image);

 // this works just like SDL_Flip() in SDL 1.2
 SDL_UpdateWindowSurface(window);

 // show image for 2 seconds
 SDL_Delay(4000);
 SDL_DestroyWindow(window);
 SDL_Quit();
 return 0;
}
*/
