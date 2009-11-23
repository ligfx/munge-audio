#include "MNGLexer.h"
#include "tests/CppUnitLite2/CppUnitLite2.h"
#include "tests/CppUnitLite2/TestResultStdErr.h"

int main()
{     
    TestResultStdErr result;
    TestRegistry::Instance().Run(result);
    return (result.FailureCount());
}

struct LexerFixture
{
  LexerFixture ()
    : lexer ("Track (Hi) { Volume (-6.7) Hi = Add (6, -7.80} //fg%4") {}
  
  MNGLexer lexer;
};

TEST_F (LexerFixture, LexerGivesCorrectTokens)
{
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
