SRCDIR	=	./src

all:
	make -C $(SRCDIR)

clean:
	make -C $(SRCDIR) clean

fclean:
	make -C $(SRCDIR) fclean
