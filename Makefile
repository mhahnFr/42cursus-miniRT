UTILS_FILES = arraylist.c arraylist_array.c arraylist_array_unsafe.c arraylist_get.c \
			  arraylist_insert.c arraylist_remove.c arraylist_reset_indices.c        \
			  arraylist_size.c hex_str_to_unsigned_int.c point.c point_copy.c

MATH_FILES  = abs.c matrix.c matrix_calculations.c matrix_destroy.c matrix_filler.c   \
			  matrix_new.c vector.c vector_add_sub.c vector_delete.c vector_division.c\
			  vector_multiplications.c

UTILS_HDR   = arraylist.h hex_str_to_unsigned_int.h point.h

MATH_HDR    = abs.h matrix.h matrix_new.h vector.h

UTILS_SRC   = $(addprefix utils/,$(UTILS_FILES)) $(addprefix utils/math/,$(MATH_FILES))

UTILS_HDR   = $(addprefix utils/,$(UTILS_HDR)) $(addprefix utils/math,$(MATH_HDR))

SRC         = $(UTILS_SRC)

HDR         = $(UTILS_HDR)

OBJ         = $(SRC:.c=.o)

CFLAGS      = -Wall -Werror -Wextra -g

LDFLAGS     = -I.

NAME        = miniRT

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(LDFLAGS) -o $(OBJ)

%.o: %.c $(HDR)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all
