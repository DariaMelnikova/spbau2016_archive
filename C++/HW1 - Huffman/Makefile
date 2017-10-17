CPP_FLAGS = -Werror -Wall
all: bin/huffman

bin/main.o: src/main.cpp src/file_compress_huffman.cpp src/file_decompress_huffman.cpp
	g++ $(CPP_FLAGS) -c $^ -o bin/main.o

bin/file_compress_huffman.o: src/file_compress_huffman.cpp
	g++ $(CPP_FLAGS) -c .$^ -o bin/file_compress_huffman.o

bin/file_decompress_huffman.o: src/file_decompress_huffman.cpp
	g++ $(CPP_FLAGS) -c .$^ -o bin/file_decompress_huffman.o

bin/huffman: src/file_compress_huffman.cpp src/file_decompress_huffman.cpp src/main.cpp 
	g++ $(CPP_FLAGS) $^ -o bin/huffman 
	

clean: 
	rm -f bin/huffman

.PHONY: clean

