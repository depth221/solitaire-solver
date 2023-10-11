a.out: cardpile.o solitaire.o main.o
	g++ -std=c++11 -o main cardpile.o solitaire.o main.o
	rm *.o

cardpile.o: cardpile.cpp
	g++ -std=c++11 -c -o cardpile.o cardpile.cpp

solitaire.o: solitaire.cpp
	g++ -std=c++11 -c -o solitaire.o solitaire.cpp

main.o: main.cpp
	g++ -std=c++11 -c -o main.o main.cpp

clean:
	rm main *.o