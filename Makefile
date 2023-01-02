CC = gcc
INCLUDE_PATHS = -IC:/SDL2/include/SDL2
LIBRARY_PATHS = -LC:/SDL2/lib
COMPILER_FLAGS = -Wall 
LINKER_FLAGS = -lmingw32 -lSDL2main -lSDL2
build:
	$(CC) ./src/*.c -o ScratchRenderingEngine.exe $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
run:
	./ScratchRenderingEngine.exe
build-and-run:
	$(CC) ./src/*.c -o ScratchRenderingEngine.exe $(COMPILER_FLAGS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS)
	./ScratchRenderingEngine.exe
clean:
	rm ScratchRenderingEngine.exe
