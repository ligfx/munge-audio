#include "AST.h"
#include <iostream>
#include "MNGLexer.h"
#include "MNGParser.h"
#include "tests/CppUnitLite2/CppUnitLite2.h"
#include "tests/CppUnitLite2/TestResultStdErr.h"

using namespace std;
using namespace boost;

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
  std::list<FunctionNode> tree;
  CHECK (parser.Parse(&tree));
}

// TODO: CHECK_NOT_EQUAL

TEST (ParserExpectedFunctionTopLevel)
{
  MNGLexer lexer ("(");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (!parser.Parse(&tree));
  CHECK (parser.getMessage().find ("Expected function") != std::string::npos);
}

TEST (ParserExpectedFunctionBlockLevel)
{
  MNGLexer lexer ("Update { 6 }");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (!parser.Parse(&tree));
  CHECK (parser.getMessage().find ("Expected function") != std::string::npos);
}

TEST (ParserExpectedArgListBlockOrOperator)
{
  MNGLexer lexer ("Add )");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (!parser.Parse(&tree));
  CHECK (parser.getMessage().find ("Expected argument list, block, or operator") != std::string::npos);
}

TEST (ParserExpectedCommaBetweenArguments)
{
  MNGLexer lexer ("Add ( 6 7 )");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (!parser.Parse(&tree));
  CHECK (parser.getMessage().find ("Expected comma between arguments") != std::string::npos);
}

TEST (ParserExpectedNumberVariableNameOrFunction)
{
  MNGLexer lexer ("Add = ,");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (!parser.Parse(&tree));
  CHECK (parser.getMessage().find ("Expected number, variable name, or function") != std::string::npos);
}

TEST (ParserLexFailUnrecognizedCharacter)
{
  MNGLexer lexer ("Add &");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (!parser.Parse(&tree));
  CHECK (parser.getMessage().find ("Unrecognized character") != std::string::npos);
}

TEST (ParserTreeMakesSense)
{
  MNGLexer lexer ("Add (6, 7) { Main (Hi) P = -8.7 }");
  MNGParser parser (&lexer, "test.cpp");
  list<FunctionNode> tree;
  CHECK (parser.Parse (&tree));
  
  CHECK_EQUAL ((unsigned int)1, tree.size());
  {
    FunctionNode first = *tree.begin();
    CHECK_EQUAL ("Add", first.name);
    CHECK_EQUAL ((unsigned int)2, first.args.size());
    {
      NumberNode a = get<NumberNode>(*first.args.begin());
      CHECK_EQUAL (6, a.number);
        
      NumberNode b = get<NumberNode>(*(++first.args.begin()));
      CHECK_EQUAL (7, b.number);
    }
    CHECK_EQUAL ((unsigned int)2, first.block.size());
    {
      FunctionNode c = *first.block.begin();
      CHECK_EQUAL ("Main", c.name);
      CHECK_EQUAL ((unsigned int)1, c.args.size());
      {
        CHECK_EQUAL ("Hi", get<NameNode>(*c.args.begin()).name);
      }
      
      FunctionNode d = *(++first.block.begin());
      CHECK_EQUAL ("=", d.name);
      CHECK_EQUAL ((unsigned int)2, d.args.size());
      {
        NameNode e = get<NameNode>(*d.args.begin());
        CHECK_EQUAL ("P", e.name);
        
        NumberNode f = get<NumberNode>(*(++d.args.begin()));
        CHECK_CLOSE (-8.7, f.number, 0.00001);
      }
    }
  }
}

TEST (PrettyPrintingProducesSameParseTreeAndEqualityCheckingWorks)
{
  MNGLexer lexer ("Add (6, 7) { Main (Hi) P = -0.87 Voice { Volume (0) } }");
  MNGParser parser (&lexer, "test.cpp");
  list<FunctionNode> firsttree;
  CHECK (parser.Parse (&firsttree));
  
  stringstream s;
  s << firsttree;
  string pp = s.str();
  const char *script = pp.c_str();
  
  lexer = MNGLexer (script);
  parser = MNGParser (&lexer, "test.cpp");
  list<FunctionNode> secondtree;
  CHECK (parser.Parse (&secondtree));
  
  CHECK (equal(firsttree.begin(), firsttree.end(), secondtree.begin()));
}
