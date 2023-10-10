a.out: cardpile.o solitaire.o main.o
	g++ -o main cardpile.o solitaire.o main.o
	rm *.o

cardpile.o: cardpile.cpp
	g++ -c -o cardpile.o cardpile.cpp

solitaire.o: solitaire.cpp
	g++ -c -o solitaire.o solitaire.cpp

main.o: main.cpp
	g++ -c -o main.o main.cpp

clean:
	rm main *.o