CC = gcc
#CC = x86_64-w64-mingw32-gcc
INCLUDE_PATHS = -IC:/SDL2/include/SDL2
LIBRARY_PATHS = -LC:/SDL2/lib
COMPILER_FLAGS = -Wall 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
build:
	$(CC) $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) ./src/*.c -o ScratchRenderingEngine.exe

run:
	./ScratchRenderingEngine.exe

clean:
	rm ScratchRenderingEngine.exe
