#include <iostream>
#include "MNGLexer.h"
#include "MNGParser.h"
#include "tests/CppUnitLite2/CppUnitLite2.h"
#include "tests/CppUnitLite2/TestResultStdErr.h"

int main()
{     
    TestResultStdErr result;
    TestRegistry::Instance().Run(result);
    return (result.FailureCount());
}

TEST (LexerGivesCorrectTokens)
{
  MNGLexer lexer ("Track (Hi) { Volume (-6.7) Hi = Add (6, -7.80} //fg%4");

  lexer.next();
  CHECK_EQUAL (MNGLexer::STRING, lexer.token());
  CHECK_EQUAL ("Track", lexer.getString());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::LPAREN, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::STRING, lexer.token());
  CHECK_EQUAL ("Hi", lexer.getString());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::RPAREN, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::LBRACE, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::STRING, lexer.token());
  CHECK_EQUAL ("Volume", lexer.getString());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::LPAREN, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::NUMBER, lexer.token());
  CHECK_CLOSE (-6.7, lexer.getNumber(), 0.00001);
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::RPAREN, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::STRING, lexer.token());
  CHECK_EQUAL ("Hi", lexer.getString());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::OPERATOR, lexer.token());
  CHECK_EQUAL ("=", lexer.getString());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::STRING, lexer.token());
  CHECK_EQUAL ("Add", lexer.getString());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::LPAREN, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::NUMBER, lexer.token());
  CHECK_CLOSE (6, lexer.getNumber(), 0.00001);
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::COMMA, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::NUMBER, lexer.token());
  CHECK_CLOSE (-7.8, lexer.getNumber(), 0.00001);
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::RBRACE, lexer.token());
  
  lexer.next();
  CHECK_EQUAL (MNGLexer::EOI, lexer.token());
}

TEST (BooleanParser)
{
  MNGLexer lexer ("Track (Hi) { Add (P) Glarb { Jack = Random(-1,4) Add (5) } Hit(Nobody) }");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (parser.Parse());
}

// TODO: CHECK_NOT_EQUAL

TEST (ParserExpectedFunctionTopLevel)
{
  MNGLexer lexer ("(");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (!parser.Parse());
  CHECK (parser.getMessage().find ("Expected function") != std::string::npos);
}

TEST (ParserExpectedFunctionBlockLevel)
{
  MNGLexer lexer ("Update { 6 }");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (!parser.Parse());
  CHECK (parser.getMessage().find ("Expected function") != std::string::npos);
}

TEST (ParserExpectedArgListBlockOrOperator)
{
  MNGLexer lexer ("Add )");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (!parser.Parse());
  CHECK (parser.getMessage().find ("Expected argument list, block, or operator") != std::string::npos);
}

TEST (ParserExpectedCommaBetweenArguments)
{
  MNGLexer lexer ("Add ( 6 7 )");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (!parser.Parse());
  CHECK (parser.getMessage().find ("Expected comma between arguments") != std::string::npos);
}

TEST (ParserExpectedNumberVariableNameOrFunction)
{
  MNGLexer lexer ("Add = ,");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (!parser.Parse());
  CHECK (parser.getMessage().find ("Expected number, variable name, or function") != std::string::npos);
}

TEST (ParserLexFailUnrecognizedCharacter)
{
  MNGLexer lexer ("Add &");
  MNGParser parser (&lexer, "test.cpp");
  CHECK (!parser.Parse());
  CHECK (parser.getMessage().find ("Unrecognized character") != std::string::npos);
}
