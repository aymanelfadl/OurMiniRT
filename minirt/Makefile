CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIE -Iincludes/

MLX_FLAGS = -lmlx -lXext -lX11 -lm -lbsd
LDFLAGS = -pie

NAME = minirt

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a


PARSING = src/parser/scene_init.c src/parser/utils.c

UTILS = src/utils/math_utils.c \
        src/utils/vector_ops.c

CAMERA = src/camera/setup_camera.c

EVENTS = src/events/rotation.c \
        src/events/translation.c

RENDERING = src/rendering/ray_color.c \
           src/rendering/ray_plane.c \
           src/rendering/ray_sphere.c \
           src/rendering/ray_trace.c \
           src/rendering/setup_image.c

# SRC = src/main.c $(PARSING) $(UTILS)

SRC = src/main.c $(UTILS) $(CAMERA) $(RENDERING) $(PARSING) $(EVENTS)


OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(LDFLAGS) $(OBJ)  $(LIBFT) $(MLX_FLAGS) -o $(NAME)

$(LIBFT):
	@make -C $(LIBFT_DIR)

clean:
	rm -f $(OBJ)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
