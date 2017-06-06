/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/06 01:30:50 by bmickael          #+#    #+#             */
/*   Updated: 2017/06/06 01:30:52 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <math.h>
#include "wolf3d.h"

#define W_CENTER (WIDTH / 2)
#define H_CENTER (HEIGHT / 2)

void		modify_minimap(t_env *e, int direction)
{
	static float angle = 0;

	if (direction)
		angle += 6 * M_PI / 360;
	else
		angle -= 6 * M_PI / 360;
	//printf("dx: %f && ", cos(angle));
	//printf("dy: %f\n", sin(angle));
	//e->img_string[((H_CENTER + (int)(round)(sin(angle) * 200)) * WIDTH) +
	//				(W_CENTER + (int)(round)(cos(angle) * 200))] = 0x00FF00;
	//printf("%f\n", M_PI * 1 / 4);

	t_line line;



/* draw_box */

/*
	line.x1 = l1.x;
	line.y1 = l1.y;
	line.x2 = l2.x;
	line.y2 = l2.y;
	line.b_color = 0x0000FF;
	line.f_color = 0x00FF00;
*/




	int loc1;
	int loc2;
	int loc3;

	loc1 = ((H_CENTER + (int)(round)(sin(angle + M_PI * 3 / 4) * 100)) * WIDTH) +
					(W_CENTER + (int)(round)(cos(angle + M_PI * 3 / 4) * 100));
	loc2 = ((H_CENTER + (int)(round)(sin(angle - M_PI * 3 / 4) * 100)) * WIDTH) +
					(W_CENTER + (int)(round)(cos(angle - M_PI * 3 / 4) * 100));
	loc3 = ((H_CENTER + (int)(round)(sin(angle) * 200)) * WIDTH) +
					(W_CENTER + (int)(round)(cos(angle) * 200));

	t_coord_i l1;
	t_coord_i l2;
	t_coord_i l3;

	//printf("%i\n", (int)((round)((sin(angle + M_PI * 3 / 4)) * 100)));

	l1.y = H_CENTER + (int)((round)((sin(angle + M_PI * 3 / 4)) * 100));
	l1.x = W_CENTER + (int)((round)((cos(angle + M_PI * 3 / 4)) * 100));
	l2.y = H_CENTER + (int)((round)((sin(angle - M_PI * 3 / 4)) * 100));
	l2.x = W_CENTER + (int)((round)((cos(angle - M_PI * 3 / 4)) * 100));
	l3.y = H_CENTER + (int)((round)((sin(angle)) * 200));
	l3.x = W_CENTER + (int)((round)((cos(angle)) * 200));



	line.x1 = l1.x;
	line.y1 = l1.y;
	line.x2 = l2.x;
	line.y2 = l2.y;
	line.b_color = 0x0000FF;
	line.f_color = 0x00FF00;
//	ft_printf("x1 = %i, y1 = %i, x2 = %i, y2 = %i\n", line.x1, line.y1, line.x2, line.y2);
	draw_line(e, &line);

	line.x1 = l2.x;
	line.y1 = l2.y;
	line.x2 = l3.x;
	line.y2 = l3.y;
	line.b_color = 0x00FF00;
	line.f_color = 0xFF0000;
//	ft_printf("x1 = %i, y1 = %i, x2 = %i, y2 = %i\n", line.x1, line.y1, line.x2, line.y2);
	draw_line(e, &line);

	line.x1 = l1.x;
	line.y1 = l1.y;
	line.x2 = l3.x;
	line.y2 = l3.y;
	line.b_color = 0x0000FF;
	line.f_color = 0xFF0000;
//	ft_printf("x1 = %i, y1 = %i, x2 = %i, y2 = %i\n", line.x1, line.y1, line.x2, line.y2);
	draw_line(e, &line);

/*
					typedef struct			s_line
					{
						int					x1;
						int					y1;
						int					x2;
						int					y2;
						int					dx;
						int					dy;
						int					b_color;
						int					f_color;
					}						t_line;
*/


	e->img_string[loc1] = 0x0000FF;
	e->img_string[loc2] = 0x00FF00;
	e->img_string[loc3] = 0xFF0000;
	(void)direction;
	(void)e;

//	draw_line(t_env *env, t_line *p)
}
