#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmickael <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/23 15:41:46 by bmickael          #+#    #+#              #
#    Updated: 2017/07/03 12:55:27 by bmickael         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME = wolf3d
CC = gcc
HEADER = wolf3d.h

SRCDIR = srcs
OBJDIR = objs
INCDIR = includes
LIBDIR = libft/includes
LIBFT_HEADER = $(LIBDIR)
MINILIBX = minilibx_sierra

ifeq ($(DEBUG),yes)
	CFLAGS = -Wall -Werror -Wextra -g -O0 -fsanitize=address -I $(INCDIR) -I $(LIBFT_HEADER) -I./$(MINILIBX)
else
	CFLAGS = -Ofast -Wall -Werror -Wextra -std=c99 -I $(INCDIR) -I $(LIBFT_HEADER) -I./$(MINILIBX)
endif

SRC		= wolf3d image_mlx_tools init_mlx actions keyboard bmp_load bmp_save \
		load_config rendering_layer floor sky walls sprites scene \
		get_next_line load ft_secure_atoi_spaces debug minimap draw_line draw \
		lib timer render_sky render_threads get_wall_infos define_mouvements \
		move_sprites

TMP = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(TMP)))

.PHONY: all clean fclean re help

all: $(NAME)

$(NAME): $(OBJ) $(INCDIR)/$(HEADER)
	make -C $(MINILIBX)/ all
	make -C libft/ all DEBUG=$(DEBUG)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L./libft -lft -framework openGL -framework AppKit ./$(MINILIBX)/libmlx.a

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/$(HEADER)
ifeq ($(NOSTALGIA),yes)
	$(CC) -D NOSTALGIA -c $(CFLAGS) -o $@ $<
else
	$(CC) -c $(CFLAGS) -o $@ $<
endif

clean:
	make -C $(MINILIBX)/ clean
	make -C libft/ clean
	rm -vf $(OBJ)

fclean: clean
	make -C libft/ fclean
	rm -vf $(NAME)

re: 	fclean all

help:
	@echo
	@echo "Programm $(NAME)"
	@echo
	@echo "--------------------------------------------------------------------------"
	@echo " Disp rules."
	@echo
	@echo " all     : Compile the programm $(NAME) into $(BINDIR) directory."
	@echo " re      : Recompile all objets of the programm."
	@echo " clean   : Remove objects."
	@echo " fclean  : Remove objects and programm."
	@echo " help    : Display this."
	@echo "--------------------------------------------------------------------------"
	@echo
