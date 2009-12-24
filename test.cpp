/*
 *  test.cpp
 *  creatures-munge
 *
 *  Copyright (c) 2009 Michael Maltese. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 */

#include <gtest/gtest.h>

#include "AST.h"
#include <iostream>
#include "MNGLexer.h"
#include "MNGParser.h"
#include "SampleScanner.h"

using namespace std;
using namespace boost;

TEST (LexerTest, GivesCorrectTokens)
{
  MNGLexer lexer ("Track (Hi) { Volume (-6.7) Hi = Add (6, -7.80} //fg%4");

  lexer.next();
  EXPECT_EQ (MNGLexer::STRING, lexer.token());
  EXPECT_EQ ("Track", lexer.getString());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::LPAREN, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::STRING, lexer.token());
  EXPECT_EQ ("Hi", lexer.getString());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::RPAREN, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::LBRACE, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::STRING, lexer.token());
  EXPECT_EQ ("Volume", lexer.getString());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::LPAREN, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::NUMBER, lexer.token());
  EXPECT_FLOAT_EQ (-6.7, lexer.getNumber());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::RPAREN, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::STRING, lexer.token());
  EXPECT_EQ ("Hi", lexer.getString());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::OPERATOR, lexer.token());
  EXPECT_EQ ("=", lexer.getString());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::STRING, lexer.token());
  EXPECT_EQ ("Add", lexer.getString());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::LPAREN, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::NUMBER, lexer.token());
  EXPECT_FLOAT_EQ (6, lexer.getNumber());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::COMMA, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::NUMBER, lexer.token());
  EXPECT_FLOAT_EQ (-7.8, lexer.getNumber());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::RBRACE, lexer.token());
  
  lexer.next();
  EXPECT_EQ (MNGLexer::EOI, lexer.token());
}

TEST (ParserTest, Success)
{
  MNGLexer lexer ("Track (Hi) { Add (P) Glarb { Jack = Random(-1,4) Add (5) } Hit(Nobody) }");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_TRUE (parser.Parse(&tree));
}

TEST (ParserTest, ExpectedFunctionTopLevel)
{
  MNGLexer lexer ("(");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_FALSE (parser.Parse(&tree));
  ASSERT_NE (parser.getMessage().find ("Expected function"), std::string::npos);
}

TEST (ParserTest, ExpectedFunctionBlockLevel)
{
  MNGLexer lexer ("Update { 6 }");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_FALSE (parser.Parse(&tree));
  ASSERT_NE (parser.getMessage().find ("Expected function"), std::string::npos);
}

TEST (ParserTest, ExpectedArgListBlockOrOperator)
{
  MNGLexer lexer ("Add )");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_FALSE (parser.Parse(&tree));
  ASSERT_NE (parser.getMessage().find ("Expected argument list, block, or operator"), std::string::npos);
}

TEST (ParserTest, ExpectedCommaBetweenArguments)
{
  MNGLexer lexer ("Add ( 6 7 )");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_FALSE (parser.Parse(&tree));
  ASSERT_NE (parser.getMessage().find ("Expected comma between arguments"), std::string::npos);
}

TEST (ParserTest, ExpectedNumberVariableNameOrFunction)
{
  MNGLexer lexer ("Add = ,");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_FALSE (parser.Parse(&tree));
  ASSERT_NE(parser.getMessage().find ("Expected number, variable name, or function"), std::string::npos);
}

TEST (ParserTest, LexerFailsOnUnrecognizedCharacter)
{
  MNGLexer lexer ("Add &");
  MNGParser parser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  ASSERT_FALSE (parser.Parse(&tree));
  ASSERT_NE  (parser.getMessage().find ("Unrecognized character"), std::string::npos);
}

TEST (ParserTest, TreeMakesSense)
{
  MNGLexer lexer ("Add (6, 7) { Main (Hi) P = -8.7 }");
  MNGParser parser (&lexer, "test.cpp");
  list<FunctionNode> tree;
  ASSERT_TRUE (parser.Parse (&tree));
  
  EXPECT_EQ ((unsigned int)1, tree.size());
  {
    FunctionNode first = *tree.begin();
    EXPECT_EQ ("Add", first.name);
    EXPECT_EQ ((unsigned int)2, first.args.size());
    {
      NumberNode a = get<NumberNode>(*first.args.begin());
      EXPECT_EQ (6, a.number);
        
      NumberNode b = get<NumberNode>(*(++first.args.begin()));
      EXPECT_EQ (7, b.number);
    }
    EXPECT_EQ ((unsigned int)2, first.block.size());
    {
      FunctionNode c = *first.block.begin();
      EXPECT_EQ ("Main", c.name);
      EXPECT_EQ ((unsigned int)1, c.args.size());
      {
        EXPECT_EQ ("Hi", get<NameNode>(*c.args.begin()).name);
      }
      
      FunctionNode d = *(++first.block.begin());
      EXPECT_EQ ("=", d.name);
      EXPECT_EQ ((unsigned int)2, d.args.size());
      {
        NameNode e = get<NameNode>(*d.args.begin());
        EXPECT_EQ ("P", e.name);
        
        NumberNode f = get<NumberNode>(*(++d.args.begin()));
        EXPECT_FLOAT_EQ (-8.7, f.number);
      }
    }
  }
}

TEST (PrettyPrintingTest, ProducesEqualParseTree)
{
  MNGLexer lexer ("Add (6, 7) { Main (Hi) P = -0.87 Voice { Volume (0) } }");
  MNGParser parser (&lexer, "test.cpp");
  list<FunctionNode> firsttree;
  ASSERT_TRUE (parser.Parse (&firsttree));
  
  stringstream s;
  s << firsttree;
  string pp = s.str();
  const char *script = pp.c_str();
  
  lexer = MNGLexer (script);
  parser = MNGParser (&lexer, "test.cpp");
  list<FunctionNode> secondtree;
  ASSERT_TRUE (parser.Parse (&secondtree));
  
  ASSERT_TRUE (equal(firsttree.begin(), firsttree.end(), secondtree.begin()));
}

TEST (SampleScannerTest, Works)
{
  MNGLexer lexer ("Wave (Blah) Add (Wave(Hmm),6.7) Voice { Wave (BobbyJones) Wave\n(Blah) }");
  MNGParser parser (&lexer, "test.cpp");
  list<FunctionNode> tree;
  ASSERT_TRUE (parser.Parse (&tree));
  
  SampleScanner ss;
  ss.visit (tree);
  
  list<string> names = ss.getNames();
  EXPECT_EQ ((unsigned int)3, names.size());
  list<string>::iterator i = names.begin();
  EXPECT_EQ ("Blah", *i++);
  EXPECT_EQ ("Hmm", *i++);
  EXPECT_EQ ("BobbyJones", *i++);
}
