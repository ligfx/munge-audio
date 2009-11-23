# vim: set noexpandtab:

munge-extract: munge-extract.cpp file.cpp
	g++ -o $@ $^ -g -lboost_filesystem-mt -lboost_system-mt

MNGLexer.cpp: MNGLexer.re2c
	re2c -b -o $@ $^

clean:
	rm munge-extract MNGLexer.cpp
