CC= cc
FLAGS_SPEED = -fsanitize=address -g 
# -Ofast -march=native -flto

#
#-O3
CFLAGS= $(FLAGS_SPEED)
#-Wextra -Wall -Werror
LIBFT 	=	libft/libft.a
INCLUDES=-I./includes -I./MLX42/include/MLX42
MLX=MLX42/build/libmlx42.a
MLX_FLAGS_LINUX=-Iinclude -ldl -lglfw -pthread -lm
MLX_FLAGS_MAC= -framework Cocoa $(MLX) -framework OpenGL -framework IOKit -Iinclude -lglfw
#-L"/Users/$(USER)/.brew/opt/glfw/lib/"
MLX_FLAGS_LINUX =-I MLX42/include/MLX42 $(MLX) -Iinclude -ldl -lglfw -pthread -lm
OS := $(shell uname)
ifeq ($(OS), Darwin)
	MLX_FLAGS = $(MLX_FLAGS_MAC)
else
	MLX_FLAGS = $(MLX_FLAGS_LINUX)
endif


NAME=cub3D
SOURCES= \
	main.c \
	utils/fps.c \
	utils/fixed_point/fixed_point_float_conv.c \
	stuff.c \
	draw.c \
	vec3.c \
	utils1.c \
	triangle.c \
	obj_parser/obj_parser.c \
	matrix/init_matrix.c \
	matrix/matrix_math1.c \
	matrix/matrix_mult.c \
	fill_triangle.c \
	fill_triangle2.c \
	old.c \
	clipping_z.c \
	clipping_xy.c \
	key_handlers.c \
	menu/menu.c \
	menu/menu_open_close.c \
	menu/menu_public.c \
	menu/menu_hooks.c \
	init.c \
	utils/cleanup.c \
	arrays.c
#init_mesh.c 

VPATH = includes:MLX42/include/MLX42/:utils:utils/fixed_point:obj_parser:matrix:menu

#	to_replace.c \

OBJ_DIR=o_files/
#OBJECTS=$(SOURCES:.c=.o)
TMP=$(notdir $(SOURCES))
OBJECTS=$(TMP:%.c=$(OBJ_DIR)%.o)
	

GREEN	=	\033[0;32m
YELLOW	=	\033[33m
CYAN	=	\033[0;36m
CLEAR	=	\033[0m

.PHONY: clone_mlx42 all clean fclean ffclean

all: fast

debug: mlx $(OBJECTS)
	@cd libft && make
	@$(CC) $(CFLAGS) $(OBJECTS) $(LIBFT) $(INCLUDES) -o $(NAME) $(MLX_FLAGS)
	@echo "$(GREEN)$(NAME) compiled!$(CLEAR)"

fast: fclean
	make debug CFLAGS="-march=native -Ofast -DNDEBUG=1 -mavx"
#make CFLAGS="-fsanitize=address"
#CFLAGS="-march=native -Ofast -DNDEBUG=1 -mavx"

prof: fclean
	make CFLAGS="-march=native -Ofast -mavx2 -DNDEBUG=1 -g -pg" CC=gcc

asm: fclean $(OBJECTS)
	cd libft && make
	$(CC) $(CFLAGS) -S $(OBJECTS) $(LIBFT) $(INCLUDES) -o $(NAME).asm $(MLX_FLAGS)
	echo "$(GREEN)$(NAME).asm generated!$(CLEAR)"

bs:
	echo $(OBJECTS)

$(OBJ_DIR):
	mkdir $(OBJ_DIR)

$(OBJ_DIR)%.o: %.c mlx $(OBJ_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

#clean: SOURCES += main.c
clean:
	@rm -f $(OBJECTS)
	@rm -rf $(FIXED_TESTS_OBJECT_DIR)
	@cd libft && make clean
	@echo "$(CYAN)object files cleaned$(CLEAR)"

fclean: clean
	@rm -f $(NAME) test
	@cd libft && make fclean
	@echo "$(CYAN)cub3D fclean$(CLEAR)"

ffclean: fclean
	rm -rf MLX42
	echo "$(CYAN)cub3D ffclean$(CLEAR)"

re: fclean all

rre: ffclean all

stack_usage: mlx
	$(CC) -fstack-usage -o stack_usage/stackusage $(SOURCES) $(MLX) -lglfw -lm $(LIBFT)

# && cmake --build build -j4;
#&& make -C build -j4; 

mlx: clone_mlx
	@if [ ! -e $(MLX) ]; then \
		cd MLX42 && cmake -DDEBUG=1 -B build && cmake --build build -j4; \
		echo "$(GREEN) lib_MLX compiled!$(CLEAR)"; \
	fi

clone_mlx:
	@if [ ! -d "MLX42" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi

testing: mlx main.o $(OBJECTS) 
	@cd libft && make
	@$(CC) $(CFLAGS) $(OBJECTS) main.o testing.c $(LIBFT) $(INCLUDES) -o test $(MLX_FLAGS)
	@echo "$(GREEN)$(NAME) compiled!$(CLEAR)"


# $(CC) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out
# $(foreach file, $(SOURCES_FIXED_TESTS), $(shell $(CC) $(FLAGS_SPEED) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(file) && sleep 1 && ./a.out >tmp && rm tmp) )
# @echo '--------------------------------'  >>time_logs.log #footer

# fixed_tests: clean mlx $(OBJECTS)
# 	 $(foreach file,$(SOURCES_FIXED_TESTS),$(shell $(CC) $(FLAGS_SPEED) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) fixed_speed_testing_tests.c $(file) && sleep 1 && ./a.out;))
# 	$(foreach file, $(SOURCES_FIXED_TESTS), $(CC) $(FLAGS_SPEED) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) fixed_speed_testing_tests.c $(file) && sleep 1 && ./a.out)


# testing: SOURCES += testing.c
# testing: mlx $(OBJECTS) testing.o
# 	@$(CC) $(CFLAGS) $(OBJECTS) $(INCLUDES) $(MLX_FLAGS)
# 	@echo "$(GREEN) testing compiled!$(CLEAR)"


SOURCES_FIXED_TESTS = \
	fixed_test_additon.c		float_test_additon.c \
	fixed_test_additon_fi.c		float_test_additon_fi.c \
	fixed_test_subtraction.c	float_test_subtraction.c \
	fixed_test_multiplication.c	float_test_multiplication.c \
	fixed_test_devision.c		float_test_devision.c \
	fixed_test_devision_fi.c	float_test_devision_fi.c
DIR_FIXED_TESTS = tests/fixed_tests/
HEADER_FIXED_TESTS = $(DIR_FIXED_TESTS)fixed_testing.h
SOURCES_FIXED_TESTS := $(addprefix $(DIR_FIXED_TESTS),$(SOURCES_FIXED_TESTS))

ADDITIONAL_FLAGS_FIXED_TEST =

# -funroll-all-loops

SOURCES_FIXED_TESTS = \
	fixed_test_additon.c		float_test_additon.c \
	fixed_test_additon_fi.c		float_test_additon_fi.c \
	fixed_test_subtraction.c	float_test_subtraction.c \
	fixed_test_multiplication.c	float_test_multiplication.c \
	fixed_test_devision.c		float_test_devision.c \
	fixed_test_devision_fi.c	float_test_devision_fi.c \
	ref_int_int_add.c


DIR_FIXED_TESTS = tests/fixed_tests/

HEADER_FIXED_TESTS = $(DIR_FIXED_TESTS)fixed_testing.h

SOURCES_FIXED_TESTS := $(foreach file,$(SOURCES_FIXED_TESTS),$(DIR_FIXED_TESTS)$(file))

ADDITIONAL_FLAGS_FIXED_TEST = -march=native
#-funroll-all-loops

fixed_tests: clean mlx $(OBJECTS)
	@cd libft && make
	@echo '---------------------------------------'  >>time_logs.log && \
	date "+%m/%d: %H:%M" >>time_logs.log && \
	echo -------------"test 0-------------" >>time_logs.log && \
	echo speed flags: >>time_logs.log && \
	$(CC) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done && \
	echo -------------"test 1-------------" >>time_logs.log && \
	echo speed flags: >>time_logs.log && \
	$(CC) -DFT_INLINE_TEST_FT=1 $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) -DFT_INLINE_TEST_FT=1 $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) -DFT_INLINE_TEST_FT=1  $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done && \
	echo -------------"test 2-------------" >>time_logs.log && \
	echo speed flags: $(ADDITIONAL_FLAGS_FIXED_TEST) >>time_logs.log && \
	$(CC) -DFT_INLINE_TEST_FT=1 $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) -DFT_INLINE_TEST_FT=1 $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) -DFT_INLINE_TEST_FT=1 $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done && \
	echo -------------"test 3-------------" >>time_logs.log && \
	echo speed flags: -O3 $(ADDITIONAL_FLAGS_FIXED_TEST) >>time_logs.log && \
	$(CC) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) -O3 $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) -O3 $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done && \
	echo -------------"test 4-------------" >>time_logs.log && \
	echo speed flags: -Ofast $(ADDITIONAL_FLAGS_FIXED_TEST) >>time_logs.log && \
	$(CC) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) -Ofast $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) -Ofast $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done; \
	echo -------------"test 5-------------" >>time_logs.log && \
	echo speed flags: -O3 $(ADDITIONAL_FLAGS_FIXED_TEST) >>time_logs.log && \
	$(CC) -DFT_INLINE_TEST_FT=1 $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) -DFT_INLINE_TEST_FT=1 -O3 $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) -DFT_INLINE_TEST_FT=1 -O3 $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done && \
	echo -------------"test 6-------------" >>time_logs.log && \
	echo speed flags: -Ofast $(ADDITIONAL_FLAGS_FIXED_TEST) >>time_logs.log && \
	$(CC) -DFT_INLINE_TEST_FT=1 $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)init.c && ./a.out && rm a.out ; \
	for test in $(SOURCES_FIXED_TESTS); \
	do \
		$(CC) -DFT_INLINE_TEST_FT=1 -Ofast $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
		$(CC) -DFT_INLINE_TEST_FT=1 -Ofast $(ADDITIONAL_FLAGS_FIXED_TEST) $(OBJECTS) $(LIBFT) $(INCLUDES) $(MLX_FLAGS) $(HEADER_FIXED_TESTS) $(DIR_FIXED_TESTS)fixed_speed_testing_tests.c $$test && sleep 1 && ./a.out && rm a.out && rm -f $(DIR_FIXED_TESTS)fixed_testing.h.gch; \
	done && \
	echo '---------------------------------------'  >>time_logs.log;

