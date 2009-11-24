/*
 *  test-engine.cpp
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
 
#include "AST.h"
#include "Expression.h"
#include "MNGLexer.h"
#include "MNGParser.h"
#include "tests/CppUnitLite2/CppUnitLite2.h"

using namespace std;

class TestRandomGenerator
  : public IRandomGenerator
{
public:
  TestRandomGenerator ()
  {
    numbers.push_back (5.6);
  }
  virtual float next (float, float)
  {
    float n = numbers.front();
    numbers.pop_front();
    return n;
  }
protected:
  std::list<float> numbers;
};

TEST (ExpressionParserConstant)
{
  ArgNode a = NumberNode(0.6);

  Expression expr;
  ExpressionParser parser(a);
  CHECK (parser.Parse (&expr));
  
  TestRandomGenerator random;
  CHECK_CLOSE (
    0.6,
    expr.getValue (map<string, float>(), &random),
    0.00001);
}

TEST (ExpressionParserVariable)
{
  ArgNode a = NameNode ("MementoMori");
  
  Expression expr;
  ExpressionParser parser(a);
  CHECK (parser.Parse (&expr));
  
  TestRandomGenerator random;
  map<string, float> variables;
  variables.insert (make_pair ("MementoMori", -1.5));
  CHECK_CLOSE (
    -1.5,
    expr.getValue (variables, &random),
    0.00001);
}

TEST (ExpressionParserFunction)
{
  MNGLexer lexer ("Add (1, 2)");
  MNGParser mngparser(&lexer, "test.cpp");
  list<FunctionNode> tree;
  CHECK (mngparser.Parse (&tree));
  
  ArgNode node = tree.front();
  ExpressionParser parser (node);
  Expression expression;
  CHECK (parser.Parse (&expression));
  
  TestRandomGenerator random;
  map<string, float> variables;
  CHECK_CLOSE (
    3,
    expression.getValue (variables, &random),
    0.00001);
}

TEST (ExpressionParserReallyComplicatedFunction)
{
  MNGLexer lexer ("Add (Subtract (10, 5), Multiply (2, Divide (7, 1)))");
  MNGParser mngparser(&lexer, "test.cpp");
  list<FunctionNode> tree;
  CHECK (mngparser.Parse (&tree));
  
  ArgNode node = tree.front();
  ExpressionParser parser (node);
  Expression expression;
  CHECK (parser.Parse (&expression));
  
  TestRandomGenerator random;
  map<string, float> variables;
  CHECK_CLOSE (
    19,
    expression.getValue (variables, &random),
    0.00001);
}

#if 0
#include "Track.h"


TEST (TrackParserFailsOnNotTrack)
{
  FunctionNode f;
  f.name = "Effect";
  TrackParser parser (f);
  Track track;
  CHECK (!parser.Parse (&track));
  CHECK (parser.getMessage().find ("Expected Track") != std::string::npos);
}

TEST (TrackParserFailsOnTrackWithWrongNumberOfArguments)
{
  FunctionNode f;
  f.name = "Track";
  TrackParser parser (f);
  Track track;
  CHECK (!parser.Parse (&track));
  CHECK (parser.getMessage().find ("Wrong number of arguments") != std::string::npos);
}

TEST (TrackParserFailsOnNonNameArgument)
{
  FunctionNode f;
  f.name = "Track";
  f.args.push_back (NumberNode (0));
  TrackParser parser (f);
  Track track;
  CHECK (!parser.Parse (&track));
  CHECK (parser.getMessage().find("Track expects name") != std::string::npos);
}

TEST (TrackReaderFailsOnUnknownAttribute)
{
  MNGLexer lexer ("Track (Hi) { Boosh () }");
  MNGParser mngparser (&lexer, "test.cpp");
  std::list<FunctionNode> tree;
  CHECK (mngparser.Parse (&tree));
  
  FunctionNode top = tree.front();
  TrackParser trackparser (top);
  Track track;
  CHECK (!trackparser.Parse (&track));
  CHECK (trackparser.getMessage().find ("got Boosh") != std::string::npos);
}

#endif

#if 0
TEST (TrackReaderReadsAttributesCorrectly)
{
  MNGLexer lexer ("Track (ChrisPineIsAwesome) { Volume (0.7) BeatLength (0.4) FadeIn (0.1) FadeOut (0.2) }");
  
}

TEST (TrackReaderDealsWithNoAttributesCorrectly)
{
  MNGLexer lexer ("Track (MySisterLovesHeroes) { }");
  
}

TEST (TrackReaderFailsOnMistypedAttribute)
{

}
#endif
