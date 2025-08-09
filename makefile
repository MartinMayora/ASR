OBJETOS = build/audioProcessing.o build/wav_to_vector.o build/RRN.o


CXX = g++
OPCIONES = -g -Wall -fopenmp

ejec: $(OBJETOS) main.cpp | build
	$(CXX) $(OPCIONES) $(OBJETOS) main.cpp -o build/ejec
	
build/audioProcessing.o: include/audioProcessing.h src/audioProcessing.cpp | build
	$(CXX) $(OPCIONES) -c src/audioProcessing.cpp -o build/audioProcessing.o

build/wav_to_vector.o: include/wav_to_vector.h src/wav_to_vector.cpp | build
	$(CXX) $(OPCIONES) -c src/wav_to_vector.cpp -o build/wav_to_vector.o

build/RNN.o: include/RNN.h src/RNN.cpp | build
	$(CXX) $(OPCIONES) -c src/RNN.cpp -o build/RNN.o
	
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