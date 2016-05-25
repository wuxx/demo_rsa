OBJ=demo_rsa

#CFLAGS = -m32
.PHONY: clean

all: clean
	gcc $(CFLAGS) $(OBJ).c -o $(OBJ)

test:

clean:	
	rm -f $(OBJ)
