CC= cc
FLAGS_SPEED = -O3
CFLAGS= $(FLAGS_SPEED)
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
	@rm -rf $(FIXED_TESTS_OBJECT_DIR)
	@cd libft && make clean
	@echo "$(CYAN)object files cleaned$(CLEAR)"

fclean: clean
	@rm -f $(NAME)
	@cd libft && make fclean
	@echo "$(CYAN)cub3D fclean$(CLEAR)"

ffclean: fclean
	@rm -rf MLX42
	@echo "$(CYAN)cub3D ffclean$(CLEAR)"

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

SOURCES_FIXED_TESTS =	fixed_test_additon.c		fixed_test_subtraction.c	float_test_devision_fi.c \
						fixed_test_additon_fi.c		fixed_testing.h			float_test_multiplication.c \
						fixed_test_devision.c		float_test_additon.c		float_test_subtraction.c \
						fixed_test_devision_fi.c	float_test_additon_fi.c \
						fixed_test_multiplication.c	float_test_devision.c

DIR_FIXED_TESTS = tests/fixed_tests/

SOURCES_FIXED_TESTS := $(foreach file,$(SOURCES_FIXED_TESTS),$(DIR_FIXED_TESTS)$(file))

# FIXED_TESTS_OBJECT_DIR = $(DIR_FIXED_TESTS)obj
# OBJECTS_FIXED_TESTS	=	$(SOURCES_FIXED_TESTS:%.c=$(FIXED_TESTS_OBJECT_DIR)/%.o)

# fixed_tests_obj_comp: $(SOURCES_FIXED_TESTS)
# 	@mkdir $(FIXED_TESTS_OBJECT_DIR)
# 	$
# 	@$(CC) $(CFLAGS) -c $(SOURCES_FIXED_TESTS) -o $(OBJECTS_FIXED_TESTS) $(INCLUDES)

fixed_tests: clean mlx $(OBJECTS) $(MAIN_OB)
	@cd libft && make
	$(foreach file, $(SOURCES_FIXED_TESTS), $(CC) $(FLAGS_SPEED) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(file) && sleep 1 && ./a.out)

# testing: SOURCES += testing.c
# testing: mlx $(OBJECTS) testing.o
# 	@$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(MLX_FLAGS)
# 	@echo "$(GREEN) testing compiled!$(CLEAR)"

