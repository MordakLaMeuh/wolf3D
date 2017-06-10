/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_manager.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: erucquoy <erucquoy@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/09 01:11:53 by erucquoy          #+#    #+#             */
/*   Updated: 2017/06/09 08:08:41 by erucquoy         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"



int		sdl_key_is_released(t_env *env)
{
	(void)env;
	/*
	if (env->event.key.keysym.sym == SDLK_ESCAPE)
		return (1);
	if (env->event.key.keysym.sym == SDLK_a)
		env->keyb[SDLK_a] = 0;
	if (env->event.key.keysym.sym == SDLK_RIGHT)
		env->keyb[SDLK_RIGHT] = 0;
	if (env->event.key.keysym.sym == SDLK_d)
		env->keyb[SDLK_d] = 0;
	if (env->event.key.keysym.sym == SDLK_LEFT)
		env->keyb[SDLK_LEFT] = 0;
	if (env->event.key.keysym.sym == SDLK_w)
		env->keyb[SDLK_w] = 0;
	if (env->event.key.keysym.sym == SDLK_UP)
		env->keyb[SDLK_UP] = 0;
	if (env->event.key.keysym.sym == SDLK_s)
		env->keyb[SDLK_s] = 0;
	if (env->event.key.keysym.sym == SDLK_DOWN)
		env->keyb[SDLK_DOWN] = 0;*/
	return (0);
}

/*
void	sdl_key_is_pushed(t_env *env)
{
	if (env->event.key.keysym.sym == SDLK_a)
		env->key.left = 1;
	if (env->event.key.keysym.sym == SDLK_RIGHT)
		env->key.pad_right = 1;
	if (env->event.key.keysym.sym == SDLK_d)
		env->key.right = 1;
	if (env->event.key.keysym.sym == SDLK_LEFT)
		env->key.pad_left = 1;
	if (env->event.key.keysym.sym == SDLK_w)
		env->key.forward = 1;
	if (env->event.key.keysym.sym == SDLK_UP)
		env->key.forward = 1;
	if (env->event.key.keysym.sym == SDLK_s)
		env->key.back = 1;
	if (env->event.key.keysym.sym == SDLK_DOWN)
		env->key.back = 1;
	if (env->event.key.keysym.sym == SDLK_x)
		env->gen_random_map = 1;
	if (env->event.key.keysym.sym == SDLK_e)
		SDL_SetRelativeMouseMode(1);
	if (env->event.key.keysym.sym == SDLK_r)
		SDL_SetRelativeMouseMode(0);
}

int		sdl_key_is_released(t_env *env)
{
	if (env->event.key.keysym.sym == SDLK_ESCAPE)
		return (1);
	if (env->event.key.keysym.sym == SDLK_a)
		env->key.left = 0;
	if (env->event.key.keysym.sym == SDLK_RIGHT)
		env->key.pad_right = 0;
	if (env->event.key.keysym.sym == SDLK_d)
		env->key.right = 0;
	if (env->event.key.keysym.sym == SDLK_LEFT)
		env->key.pad_left = 0;
	if (env->event.key.keysym.sym == SDLK_w)
		env->key.forward = 0;
	if (env->event.key.keysym.sym == SDLK_UP)
		env->key.forward = 0;
	if (env->event.key.keysym.sym == SDLK_s)
		env->key.back = 0;
	if (env->event.key.keysym.sym == SDLK_DOWN)
		env->key.back = 0;
	return (0);
}
*/
