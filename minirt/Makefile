CC = cc
CFLAGS = -Wall -Wextra -Werror
MLX_FLAGS = -lmlx -lXext -lX11 -lm -lbsd

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
        src/utils/matrix.c \
        src/utils/transforms.c \
        src/utils/vector_ops.c


# SRC = src/main.c $(PARSING) $(UTILS)
SRC = src/myMain.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ)  $(LIBFT) $(MLX_FLAGS) -o $(NAME)

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
