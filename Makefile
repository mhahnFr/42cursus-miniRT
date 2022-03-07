UTILS_FILES = arraylist.c arraylist_array.c arraylist_array_unsafe.c arraylist_get.c \
			  arraylist_insert.c arraylist_remove.c arraylist_reset_indices.c        \
			  arraylist_size.c hex_str_to_unsigned_int.c point.c point_copy.c

MATH_FILES  = abs.c matrix.c matrix_calculations.c matrix_destroy.c matrix_filler.c   \
			  matrix_new.c vector.c vector_add_sub.c vector_delete.c vector_division.c\
			  vector_multiplications.c

HEAD_FILES	= abs.h arraylist.h hex_str_to_unsigned_int.h matrix_new.h matrix.h minirt.h \
			  point.h vector.h

UTILS_SRC   = $(addprefix utils/,$(UTILS_FILES)) $(addprefix utils/math/,$(MATH_FILES))

HDR		    = $(addprefix include/,$(HEAD_FILES))

SRC         = $(UTILS_SRC)

OBJ         = $(SRC:.c=.o)

CFLAGS      = -Wall -Werror -Wextra -g

INC			= -Iinclude

LDFLAGS     = -Lmlx -lmlx -framework OpenGL -framework AppKit

NAME        = miniRT

MLX			= ./mlx/libmlx.a

LIBFT		= ./libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) include/libft.h $(MLX) $(OBJ)
	$(CC) $(LDFLAGS) -o $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

$(MLX):
	make -C mlx CFLAGS="-D GL_SILENCE_DEPRECATION -Wno-unused-variable -Wno-unused-parameter"

include/libft.h : $(LIBFT)

$(LIBFT):
	make -C libft
	cp ./libft/libft.h ./include

clean:
	$(RM) $(OBJ)
	make -C mlx clean
	make -C libft clean

fclean: clean
	$(RM) $(NAME)
	make -C libft fclean
	$(RM) ./include/libft.h

re: fclean all

.PHONY : all clean fclean re 
