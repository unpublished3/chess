rhino:
	clang++ -o rhino.o rhino.cpp -L ./include/bitboard.h

clean:
	rm -rf rhino.o