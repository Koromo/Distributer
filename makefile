distgen: $(wildcard src/*.c) $(wildcard src/*.h)
	clang $^
	mv a.out distgen

.PHONY: clean
clean:
	rm src/*.h.gch
