CC= cc
CFLAGS=  -O3
#-Wextra -Wall -Werror
LIBFT 	=	libft/libft.a
INCLUDES=-I./includes -I./MLX42/include/MLX42
MLX=MLX42/build/libmlx42.a
MLX_FLAGS_LINUX=-Iinclude -ldl -lglfw -pthread -lm
MLX_FLAGS_MAC= -framework Cocoa $(MLX) -framework OpenGL -framework IOKit -Iinclude -lglfw -L"/Users/$(USER)/.brew/opt/glfw/lib/"
MLX_FLAGS_LINUX =-I MLX42/include/MLX42 $(MLX) -Iinclude -ldl -lglfw -pthread -lm
OS := $(shell uname)
ifeq ($(OS), Darwin)
	MLX_FLAGS = $(MLX_FLAGS_MAC)
else
	MLX_FLAGS = $(MLX_FLAGS_LINUX)
endif


NAME=cub3D
MAIN= main.c
SOURCES= \
	utils/fps.c \
	utils/fixed_point/fixed_point_float_conv.c

OBJECTS=$(SOURCES:.c=.o)
MAIN_OB=$(MAIN:.c=.o)

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

.PHONY: clone_mlx42 all clean fclean ffclean

all: SOURCES += $(MAIN)
all: mlx $(OBJECTS) $(MAIN_OB)
	@cd libft && make
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(INCLUDES) -o $(NAME) $(MLX_FLAGS)
	@echo "$(GREEN)$(NAME) compiled!$(CLEAR)"

%.o: %.c mlx
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

#clean: OBJECTS += MAIN_OB
clean: SOURCES += main.c
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
		cd MLX42 && cmake -B build && cmake --build build -j4; \
		echo "$(GREEN) lib_MLX compiled!$(CLEAR)"; \
	fi

clone_mlx:
	@if [ ! -d "MLX42" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi

testing: SOURCES += testing.c
testing: mlx $(OBJECTS) testing.o
	@$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(MLX_FLAGS)
	@echo "$(GREEN) testing compiled!$(CLEAR)"

