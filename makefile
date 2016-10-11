tablegen: $(wildcard src/*.c) $(wildcard src/*.h)
	clang $^
	rm src/*.h.gch		
	mv a.out tablegen
