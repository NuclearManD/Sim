SDL_FLAGS = -lSDL2main -lSDL2 -lSDL2_image
IKE_INCLUDE = ike/include
IKE_O = ike/out/

all: ikelib
	mkdir -p build
	cd build
	gcc ../src/*.c -Iinclude -I../$(IKE_INCLUDE) $(SDL_FLAGS) -lm
	gcc *.o ../$(IKE_O)*.o -o sim
	cd ..
	mkdir -p bin
	mv build/sim bin/
ikelib:
	mkdir -p $(IKE_O)
	cd $(IKE_O)
	gcc -c ../src/*.c -I../include
	cd ../..
clean:
	rm -rf build
	rm -rf $(IKE_O)
fclean: clean
	rm -rf bin
re: fclean, all
