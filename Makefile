CC= cc
CFLAGS=
MLX_INC=libmlx42.a -Iinclude -ldl -lglfw -pthread -lm
NAME=cub3D
SOURCES=main.c
OBJECTS=$(SOURCES:.c=.o)

.PHONY: clone_mlx42

all: $(OBJECTS)
	$(CC) $(CFLAGS) $(MLX_INC) $(OBJECTS) -o $(NAME)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

ffclean: fclean
	rm -rf MLX42

mlx: clone_mlx
	cd MLX42 && cmake -B build
	cd MLX42 && cmake --build build -j4

clone_mlx:
	@if [ ! -d "MLX42" ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git; \
	fi


