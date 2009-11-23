# vim: set noexpandtab:

COMPILER = g++
CCFLAGS = -g -ansi -Wall -DLINUX
.SUFFIXES: .o .cpp

munge-extract: munge-extract.o file.o
	$(COMPILER) $(CCFLAGS) -o $@ $^ -lboost_filesystem-mt -lboost_system-mt

MNGLexer.cpp: MNGLexer.re2c
	re2c -b -o $@ $^

cppunitlite_src = \
	ExceptionHandler.cpp \
	Test.cpp \
  Failure.cpp \
	TestException.cpp \
	TestRegistry.cpp \
	TestResult.cpp \
	TestResultStdErr.cpp \
	Linux/SignalHandler.cpp

cppunitlite_objects = $(patsubst %.cpp, tests/CppUnitLite2/%.o, $(cppunitlite_src))
cppunitlite.a: $(cppunitlite_objects)
	ar cr $(lib).a $(objects)

%.o : %.cpp
	$(COMPILER) $(CCFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)

clean:
	rm munge-extract MNGLexer.cpp munge-extract.o file.o $(cppunitlite_objects) cppunitlite.a
