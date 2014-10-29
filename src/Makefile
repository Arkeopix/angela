NAME	=	pamela.so

GCC	=	gcc

SRC	=	pamela.c

OBJ	=	$(SRC:.c=.o)

CFLAGS	=	-fPIC -ggdb

LIBS	=	-lpam

BINDIR	=	../lib

$(NAME):	$(OBJ)
		$(GCC) $(OBJ) -o $(NAME) -shared $(LIBS)

all:		$(NAME)

clean:
		rm -vf $(OBJ)

fclean:		clean
		rm -vf $(NAME)

re:		fclean all
