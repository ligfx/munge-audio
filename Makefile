munge-extract: munge-extract.cpp file.cpp
	g++ -o $@ $^ -g -lboost_filesystem-mt -lboost_system-mt

clean:
	rm munge-extract
