rhino:
	clang++ -oFast -o rhino.o rhino.cpp -L ./include/bitboard.h 
	x86_64-w64-mingw32-g++ --static -oFast -o rhino.exe rhino.cpp -L ./include/bitboard.h

debug:
	clang++ -o rhino.o rhino.cpp -L ./include/bitboard.h 
	x86_64-w64-mingw32-g++ --static -o rhino.exe rhino.cpp -L ./include/bitboard.h

clean:
	rm -rf rhino.o rhino.exe