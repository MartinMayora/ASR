OBJETOS = build/DFT.o build/wav_to_vector.o #build/RRN.o


CXX = g++
OPCIONES = -g -Wall -fopenmp

ejec: $(OBJETOS) main.cpp | build
	$(CXX) $(OPCIONES) $(OBJETOS) main.cpp -o build/ejec
	
build/DFT.o: include/DFT.h src/DFT.cpp | build
	$(CXX) $(OPCIONES) -c src/DFT.cpp -o build/DFT.o

build/wav_to_vector.o: include/wav_to_vector.h src/wav_to_vector.cpp | build
	$(CXX) $(OPCIONES) -c src/wav_to_vector.cpp -o build/wav_to_vector.o

build:
	mkdir -p build

clean:
	rm -rf build

rebuild:
	make clean
	make

run:
	make clean
	make
	./build/ejec
#build/RRN.o: include/RRN.h src/RRN.cpp | RRN
#	$(CXX) $(OPCIONES) -c src/RRN.cpp -o build/RRN.o