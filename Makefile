# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmickael <bmickael@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/23 15:41:46 by bmickael          #+#    #+#              #
#    Updated: 2017/06/10 11:36:18 by erucquoy         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = wolf3d
CC = gcc
HEADER = wolf3d.h

SRCDIR = srcs
OBJDIR = objs
INCDIR = includes
LIBDIR = libft/includes
LIBFT_HEADER = $(LIBDIR)
SDL_HEADERS = SDL2/Headers

ifeq ($(DEBUG),yes)
	CFLAGS = -g -O0 -fsanitize=address -I $(INCDIR) -I $(LIBFT_HEADER) -I $(SDL_HEADERS)
else
	CFLAGS = -O2 -Wall -Werror -Wextra -I $(INCDIR) -I $(LIBFT_HEADER) -I $(SDL_HEADERS)
endif

SRC		= wolf3d image_sdl_tools init_sdl actions keyboard bmp_load bmp_save \
		load_config rendering_layer floor sky walls scene get_next_line load \
		ft_secure_atoi_spaces debug minimap draw_line draw lib weapon \
		key_manager

TMP = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(TMP)))

.PHONY: all clean fclean re help

all: $(NAME)

$(NAME): $(OBJ) $(INCDIR)/$(HEADER)
	make -C minilibx_sierra/ all
	make -C libft/ all DEBUG=$(DEBUG)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L./libft -lft -framework openGL -framework AppKit ./SDL2/SDL2

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/$(HEADER)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
	make -C minilibx_sierra/ clean
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
