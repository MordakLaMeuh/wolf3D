/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/23 18:26:21 by bmickael          #+#    #+#             */
/*   Updated: 2017/04/23 18:26:25 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <math.h>
#include "wolf3d.h"

static void		fill_pixel(t_env *env, int x, int y, int color)
{
	int offset;

	offset = (y * WIDTH) + x;
	if (offset >= (WIDTH * HEIGHT) || offset < 0)
		return ;
	env->img_string[offset] = color;
}

static int		get_color_line(t_line *p, float x, float x_beg, float x_end)
{
	int		color;
	float	r_diff;
	float	g_diff;
	float	b_diff;
	float	ratio;

	ratio = (x - x_beg) / (x_end - x_beg);
	r_diff = (p->f_color >> 16) - (p->b_color >> 16);
	g_diff = ((p->f_color >> 8) & 0xFF) - ((p->b_color >> 8) & 0xFF);
	b_diff = (p->f_color & 0xFF) - (p->b_color & 0xFF);
	color = ((((p->b_color >> 16) + ((int)(ratio * r_diff))) << 16) |
			((((p->b_color >> 8) & 0xFF) + (int)(ratio * g_diff)) << 8) |
			((p->b_color & 0xFF) + (int)(ratio * b_diff)));
	return (color);
}

static void		horizontal_line(t_env *env, t_line *p, int x_inc, int y_inc)
{
	int cumul;
	int x;
	int y;
	int color;

	cumul = p->dx / 2;
	x = p->p1.x;
	y = p->p1.y;
	while (TRUE)
	{
		x += x_inc;
		cumul += p->dy;
		if (cumul >= p->dx)
		{
			y += y_inc;
			cumul -= p->dx;
		}
		color = get_color_line(p, x, p->p1.x, p->p2.x);
		fill_pixel(env, x, y, color);
		if (x == p->p2.x)
			break ;
	}
}

static void		vertical_line(t_env *env, t_line *p, int x_inc, int y_inc)
{
	int cumul;
	int x;
	int y;
	int color;

	cumul = p->dy / 2;
	x = p->p1.x;
	y = p->p1.y;
	while (TRUE)
	{
		y += y_inc;
		cumul += p->dx;
		if (cumul >= p->dy)
		{
			x += x_inc;
			cumul -= p->dy;
		}
		color = get_color_line(p, y, p->p1.y, p->p2.y);
		fill_pixel(env, x, y, color);
		if (y == p->p2.y)
			break ;
	}
}

void			draw_line(t_env *env, t_line *p)
{
	int x_inc;
	int y_inc;

	p->dx = p->p2.x - p->p1.x;
	p->dy = p->p2.y - p->p1.y;
	x_inc = (p->dx < 0) ? -1 : 1;
	y_inc = (p->dy < 0) ? -1 : 1;
	p->dx = abs(p->dx);
	p->dy = abs(p->dy);
	fill_pixel(env, p->p1.x, p->p1.y, p->b_color);
	if (p->dx == 0 && p->dy == 0)
		return ;
	if (p->dx > p->dy)
		horizontal_line(env, p, x_inc, y_inc);
	else
		vertical_line(env, p, x_inc, y_inc);
}
