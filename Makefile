#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bmickael <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2017/04/23 15:41:46 by bmickael          #+#    #+#              #
#    Updated: 2017/06/06 03:23:44 by bmickael         ###   ########.fr        #
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

CFLAGS = -Wall -Werror -Wextra -g -O0 -fsanitize=address -I $(INCDIR) -I $(LIBFT_HEADER)

SRC		= wolf3d image_mlx_tools init_mlx actions keyboard bmp_load bmp_save \
		load_config skybox get_next_line load ft_secure_atoi_spaces debug minimap \
		draw_line draw

TMP = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJDIR)/, $(addsuffix .o, $(TMP)))

.PHONY: all clean fclean re help

all: $(NAME)

$(NAME): $(OBJ) $(INCDIR)/$(HEADER)
	make -C libft/ all
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) -L./libft -lft -framework openGL -framework AppKit -lmlx

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(INCDIR)/$(HEADER)
	$(CC) -c $(CFLAGS) -o $@ $<

clean:
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
