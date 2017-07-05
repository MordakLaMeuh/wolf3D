NAME = wolf3d
CC = gcc

### MAIN FLAGS ###

ifeq ($(DEBUG),yes)
	CFLAGS = -Wall -Werror -Wextra -std=c99 -g -O0 -fsanitize=address
else
	CFLAGS = -Ofast -Wall -Werror -Wextra -std=c99
endif

### SOURCES ###

SRC_CORE = wolf3d image_mlx_tools init_mlx actions keyboard \
		load_config rendering_layer floor sky walls sprites scene \
		debug lib timer render_sky render_threads get_wall_infos define_mouvements \
		move_sprites
SRC_PARSE = constructor load_map get_next_line get_player_location get_sprites get_map_struct verif_texture_range
SRC_OVERLAY = draw_line draw minimap
SRC_BMP = bmp_load bmp_save

SRC_LIST = $(SRC_CORE) $(SRC_PARSE) $(SRC_BMP) $(SRC_OVERLAY)
VPATH = srcs/core srcs/parse srcs/bmp srcs/overlay

## HEADERS

HEADERS = wolf3d.h parse.h

### LIBRAIRIES ###

LIB_DIR = libs
_MLX = minilibx_sierra
MLX = $(addprefix $(LIB_DIR)/, $(_MLX))
_LIBFT = libft
LIBFT = $(addprefix $(LIB_DIR)/, $(_LIBFT))

### ~~~~~~~~~~ ###

SRC = $(addsuffix .c, $(SRC_LIST))
OBJ_DIR = objs
TMP = $(basename $(notdir $(SRC)))
OBJ = $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(TMP)))


IFLAGS = -Isrcs -I$(LIBFT)/includes -I$(MLX)
LDFLAGS = -L$(LIBFT) -lft -framework openGL -framework AppKit $(MLX)/libmlx.a

.PHONY: all clean fclean re help

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(MLX)/ all
	make -C $(LIBFT)/ all DEBUG=$(DEBUG)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJ) $(LDFLAGS)

$(OBJ_DIR)/%.o: %.c $(HEADERS)
	$(CC) -c $(CFLAGS) -o $@ $< $(IFLAGS)

clean:
	make -C $(MLX)/ clean
	make -C $(LIBFT)/ clean
	rm -f $(OBJ)

fclean: clean
	make -C $(LIBFT)/ fclean
	rm -f $(NAME)

re: fclean all

help:
	@echo
	@echo "Programm $(NAME)"
	@echo
	@echo "--------------------------------------------------------------------------"
	@echo " Disp rules."
	@echo
	@echo " all     : Compile the program $(NAME) into $(BINDIR) directory."
	@echo " re      : Recompile all objets of the programm."
	@echo " clean   : Remove objects."
	@echo " fclean  : Remove objects and programm."
	@echo " help    : Display this."
	@echo "--------------------------------------------------------------------------"
