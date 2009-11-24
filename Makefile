# vim: set noexpandtab:

COMPILER = g++
CCFLAGS = -g -ansi -Wall -Wextra -DLINUX
.SUFFIXES: .o .cpp

munge-extract: munge-extract.o File.o MNGLexer.o MNGParser.o AST.o SampleScanner.o
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

test: test.o test-engine.o tests/cppunitlite.a MNGLexer.o MNGParser.o AST.o SampleScanner.o Expression.o Unary.o
	$(COMPILER) $(CCFLAGS) -o $@ $^

cppunitlite_objects = $(patsubst %.cpp, tests/CppUnitLite2/%.o, $(cppunitlite_src))
tests/cppunitlite.a: $(cppunitlite_objects)
	ar cr tests/cppunitlite.a $^

%.o : %.cpp
	$(COMPILER) $(CCFLAGS) -c $< -o $(patsubst %.cpp, %.o, $<)

clean:
	rm munge-extract MNGLexer.cpp MNGParser.o MNGLexer.o test.o test-engine.o SampleScanner.o Expression.o Unary.o AST.o munge-extract.o File.o $(cppunitlite_objects) tests/cppunitlite.a test
