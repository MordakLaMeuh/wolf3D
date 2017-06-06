/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bmickael <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/04/30 18:19:29 by bmickael          #+#    #+#             */
/*   Updated: 2017/04/30 18:19:34 by bmickael         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include "wolf3d.h"
#include "get_next_line.h"

static void		del(void *content, size_t i)
{
	(void)i;
	free(content);
}

int				count_nb(char *content)
{
	int i;
	int j;
	int nb;

	nb = 0;
	i = 0;
	j = 0;
	while (content[i])
	{
		if (content[i] != ' ')
		{
			if (j == 0)
				nb++;
			j = 1;
		}
		else
			j = 0;
		i++;
	}
	return (nb);
}

static t_list	*extract_input(char *arg, int *size)
{
	t_list	*list;
	char	*get_line;
	int		fd;
	int		ret;

	*size = 0;
	list = NULL;
	if ((fd = open(arg, O_RDONLY)) < 0)
	{
		ft_eprintf("Could not open %s : %s\n", arg, strerror(errno));
		return (NULL);
	}
	while ((ret = get_next_line(fd, &get_line)) > 0)
	{
		if (!(ft_lst_push_front(&list, get_line, ret)))
			return (NULL);
		(*size)++;
	}
	if (ret == -1)
		return (NULL);
	ft_lst_invert_it(&list);
	return (list);
}

static t_tile	*create_tab(char *content, int *tabsize)
{
	t_tile		*new;
	int			err;
	int			j;

	j = 0;
	if ((*tabsize = count_nb(content)) == 0)
		return (NULL);
	if (!(new = malloc(*tabsize * sizeof(t_tile))))
		return (NULL);
	while (*content)
		if (*content != ' ')
		{
			new[j].value = ft_secure_atoi_spaces(content, &err);
			if (err)
			{
				ft_eprintf("ft_secure_atoi: Problem detected\n");
				return (NULL);
			}
			j++;
			while (*content && *content != ' ')
				content++;
		}
		else
			content++;
	return (new);
}

t_tile			**get_map(char *file, t_env *env)
{
	size_t	i;
	t_list	*input;
	t_list	*ptr;
	t_tile	**tile;
	int		old_size_x;

	if (!(input = extract_input(file, &env->map.size_y)) ||
		env->map.size_y <= 0 ||
		(!(tile = (t_tile **)malloc(env->map.size_y * sizeof(t_tile *)))))
		return (NULL);
	i = 0;
	old_size_x = -1;
	ptr = input;
	while (input)
	{
		if (!(tile[i++] = create_tab(input->content, &env->map.size_x)))
			return (NULL);
		if (old_size_x < 0)
			old_size_x = env->map.size_x;
		else if (old_size_x != env->map.size_x)
			return (NULL);
		input = input->next;
	}
	ft_lstdel(&ptr, &del);
	return (tile);
}
