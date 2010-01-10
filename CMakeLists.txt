cmake_minimum_required(VERSION 2.8)

project (creatures-munge)

set (src "${creatures-munge_SOURCE_DIR}")
set (bin "${creatures-munge_BINARY_DIR}")

set (CMAKE_MODULE_PATH ${src}/cmake)

find_package (GoogleTest REQUIRED)
find_package (GoogleMock REQUIRED)
find_package (Boost COMPONENTS filesystem REQUIRED)

add_definitions ("-W -Wall")

include_directories (
  ${src}
  ${Boost_INCLUDE_DIR}
  ${GTEST_INCLUDE_DIR}
  ${GMOCK_INCLUDE_DIR})

add_executable ( munge-extract
	munge-extract.cpp
	File.cpp
	MNGLexer.cpp
	MNGParser.cpp
	AST.cpp
	SampleScanner.cpp )
target_link_libraries (munge-extract boost_filesystem-mt)
	
add_executable ( test
	test.cpp
	test-engine.cpp
	MNGLexer.cpp
	MNGParser.cpp
	AST.cpp
	SampleScanner.cpp
	Expression.cpp
	Unary.cpp
	LoopLayer.cpp )
target_link_libraries ( test
  ${GTEST_LIBRARIES}
  ${GMOCK_LIBRARIES}
  ${GMOCK_MAIN_LIBRARIES} )
	
add_custom_command (
	OUTPUT ${bin}/MNGLexer.cpp
	COMMAND re2c -b -o ${bin}/MNGLexer.cpp ${src}/MNGLexer.re2c
	DEPENDS ${src}/MNGLexer.re2c )
	
