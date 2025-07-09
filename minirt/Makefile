CC = cc
CFLAGS = -Wall -Wextra -Werror -fPIE -Iincludes/

MLX_FLAGS = -lmlx -lXext -lX11 -lm -lbsd
LDFLAGS = -pie

NAME = minirt

LIBFT_DIR = libft

LIBFT = $(LIBFT_DIR)/libft.a


PARSING = src/parser/add_to_scene.c \
          src/parser/parse_colors.c \
          src/parser/parse_double.c \
          src/parser/parse_elements.c \
          src/parser/parse_file.c \
          src/parser/parse_vectors.c \
          src/parser/parser_utils.c \
          src/parser/validate_elements.c \
          src/parser/validate_scene.c

UTILS = src/utils/math_utils.c \
        src/utils/vector_ops.c

CAMERA = src/camera/setup_camera.c

RENDRING = src/rendering/setup_image.c


# SRC = src/main.c $(PARSING) $(UTILS)

SRC = src/main.c $(UTILS) $(CAMERA) $(RENDRING)


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
