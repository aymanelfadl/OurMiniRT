# Compiler and flags
CC = cc
CFLAGS = -Wall -Wextra -Werror

# Executable name
NAME = cub3d

# Directories
LIBFT_DIR = libft

# Libs
LIBFT = $(LIBFT_DIR)/libft.a
LIBFT_FLAGS = -L$(LIBFT_DIR) -lft
MLX_FLAGS = -lmlx -lXext -lX11 -lm -lbsd

# Includes
INCLUDES = -I includes -I $(LIBFT_DIR)

# Source files
PARSING = srcs/parser/add_to_scene.c \
          srcs/parser/parse_colors.c \
          srcs/parser/parse_double.c \
          srcs/parser/parse_element.c \
          srcs/parser/parser_file.c \
          srcs/parser/parse_vectors.c \
          srcs/parser/parser_utils.c \
          srcs/parser/validate_elements.c \
          srcs/parser/validate_scene.c

SRC = srcs/main.c srcs/$(PARSING)
OBJ = $(SRC:.c=.o)

# Build rules
all: $(NAME)

$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) $(INCLUDES) $(LIBFT_FLAGS) $(MLX_FLAGS) -o $(NAME)

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
