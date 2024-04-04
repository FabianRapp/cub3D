CC= cc
CFLAGS=

MLX=MLX42/build/libmlx42.a
MLX_INC= $(MLX) -Iinclude -ldl -lglfw -pthread -lm -I MLX42/include/MLX42
INCLUDES= $(MLX_INC) -I headers

NAME=cub3D
SOURCES= \
	main.c

OBJECTS=$(SOURCES:.c=.o)

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

.PHONY: clone_mlx42 all clean fclean ffclean

all: mlx $(OBJECTS)
	@$(CC) $(CFLAGS) $(INCLUDES) $(OBJECTS) -o $(NAME)
	@echo "$(GREEN)$(NAME) compiled!$(CLEAR)"

%.o: %.c mlx
	$(CC) $(CFLAGS) $(INCLUDES)  -c $< -o $@

clean:
	@rm -f $(OBJECTS)
	@echo "$(CYAN)object files cleaned$(CLEAR)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(CYAN)minishell fclean$(CLEAR)"

ffclean: fclean
	@rm -rf MLX42
	@echo "$(CYAN)minishell ffclean$(CLEAR)"

re: fclean all

rre: ffclean all

mlx: clone_mlx
	@if [ ! -e $(MLX) ]; then \
		cd MLX42; \
		cmake -B build; \
		cmake --build build -j4; \
		echo "$(GREEN) lib_MLX compiled!$(CLEAR)"; \
	fi

clone_mlx:
	@if [ ! -d "MLX42" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi

