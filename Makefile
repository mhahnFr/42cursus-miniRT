
##		HEADER			##

HEAD_FILES	= abs.h arraylist.h hex_str_to_unsigned_int.h matrix_new.h matrix.h minirt.h \
			  point.h vector.h vertex.h

HDR         = $(addprefix include/,$(HEAD_FILES))
##		UTILITIES		##

UTILS_FILES = arraylist.c arraylist_array.c arraylist_array_unsafe.c arraylist_get.c \
			  arraylist_insert.c arraylist_remove.c arraylist_reset_indices.c        \
			  arraylist_size.c hex_str_to_unsigned_int.c point.c point_copy.c        \
			  vertex.c vertex_cast.c vertex_copy.c
MATH_FILES  = abs.c matrix.c matrix_calculations.c matrix_destroy.c matrix_filler.c   \
			  matrix_new.c vector.c vector_add_sub.c vector_delete.c vector_division.c\
			  vector_multiplications.c

UTILS_SRC   = $(addprefix utils/,$(UTILS_FILES)) $(addprefix utils/math/,$(MATH_FILES))
##		SOURCES			##


SRC_FOLDER	= src/

M_SRC    = builder.c calculator.c lexer.c main.c
#BUILDER_SRC	= 

SRC         = $(UTILS_SRC) $(addprefix src/,$(M_SRC)) #$(addprefix src/builder/, $(BUILDER_SRC))
##		OBJECTS			##

OBJ_FOLDER	= obj/

M_OBJ	= $(addprefix $(OBJ_FOLDER), $(M_SRC))
#BUILDER_OBJ	= $(addprefix obj/builder/, $(BUILDER_SRC))

OBJ         =  $(M_OBJ:.c=.o) #$(BUILDER_OBJ:.c=.o)
##		COMPILER		##
NAME        = miniRT

CFLAGS      = -Wall -Werror -Wextra -g
INC         = -Iinclude -Imlx -Ilibft
LDFLAGS     = -Lmlx -lmlx -Llibft -lft -framework OpenGL -framework AppKit

MLX         = ./mlx/libmlx.a
LIBFT       = ./libft/libft.a

all: $(NAME)

$(NAME): $(LIBFT) $(MLX) obj $(OBJ)
	$(CC) $(LDFLAGS) -o $(NAME) $(OBJ)

$(OBJ_FOLDER)%.o: $(SRC_FOLDER)%.c $(HDR)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

obj: 
	mkdir obj/ obj/builder

$(MLX):
	make -C mlx CFLAGS="-D GL_SILENCE_DEPRECATION -Wno-unused-variable -Wno-unused-parameter"

$(LIBFT):
	make -C libft

clean:
	- $(RM) $(OBJ)
	- make -C mlx clean
	- make -C libft clean

fclean: clean
	- $(RM) $(NAME)
	- $(RM) -r $(OBJ_FOLDER)
	- make -C libft fclean
	- find . -name \*~ -print -delete

re: fclean all

.PHONY : all clean fclean re 
