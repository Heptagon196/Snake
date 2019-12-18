.SILENT:
FLAG = -Dlinux
CC = gcc
DEPENDENCIES = ./main.o ./ConsoleIO.o ./List.o ./Menu.o ./Snake.o ./Block.o ./Rank.o ./InputBox.o ./Config.o
ifdef MAKE_RELEASE
	FLAG += -O2
endif
ifdef MAKE_DEBUG
	FLAG += -DDEBUG -g
endif
ifdef MAKE_STATIC
	FLAG += -static
endif

default:
	make release

snake: $(DEPENDENCIES)
	echo "[10/10] Building snake"
	$(CC) $(DEPENDENCIES) $(FLAG) -o snake

./main.o: ./main.c
	echo "[1/10] Compiling main.c"
	$(CC) -c $(FLAG) $< -o ./main.o

./ConsoleIO.o: ./ConsoleIO.c
	echo "[2/10] Compiling ConsoleIO.c"
	$(CC) -c $(FLAG) $< -o ./ConsoleIO.o

./List.o: ./List.c
	echo "[3/10] Compiling List.c"
	$(CC) -c $(FLAG) $< -o ./List.o

./Menu.o: ./Menu.c
	echo "[4/10] Compiling Menu.c"
	$(CC) -c $(FLAG) $< -o ./Menu.o

./Snake.o: ./Snake.c
	echo "[5/10] Compiling Snake.c"
	$(CC) -c $(FLAG) $< -o ./Snake.o

./Block.o: ./Block.c
	echo "[6/10] Compiling Block.c"
	$(CC) -c $(FLAG) $< -o ./Block.o

./Rank.o: ./Rank.c
	echo "[7/10] Compiling Rank.c"
	$(CC) -c $(FLAG) $< -o ./Rank.o

./InputBox.o: ./InputBox.c
	echo "[8/10] Compiling InputBox.c"
	$(CC) -c $(FLAG) $< -o ./InputBox.o

./Config.o: ./Config.c
	echo "[8/10] Compiling Config.c"
	$(CC) -c $(FLAG) $< -o ./Config.o

clean:
	echo "Cleaning files"
	rm -rf $(DEPENDENCIES)
install:
	echo "Installing snake to /usr/local/bin"
	cp ./snake /usr/local/bin/
debug:
	echo "Building a debug version"
	env MAKE_DEBUG=true make snake
static:
	echo "Building a statically linked version"
	env MAKE_STATIC=true make snake
release:
	echo "Building a release version"
	env MAKE_RELEASE=true make snake
