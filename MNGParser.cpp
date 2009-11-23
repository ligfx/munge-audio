/*
 *  MNGParser.cpp
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

#include <boost/format.hpp>
#include <iostream>
#include "MNGLexer.h"
#include "MNGParser.h"

using namespace std;
using boost::shared_ptr;
using boost::variant;

// Convention: Use this only for the token defs
typedef MNGLexer Lx;

MNGParser::MNGParser (MNGLexer *lexer, std::string filename)
{
  this->filename = filename;
  this->lexer = lexer;
}

MNGParser::~MNGParser () {}

string MNGParser::getMessage ()
{
  return message;
}

/*
  Important:
  Each 'block' of code assumes that lexer->next() has already been called.
  Hence, the call to next() before the loop. A well-mannered block will
  call next() when it is done with its token.
  
  Rationale:
  Besides being simpler to code (at least for me), take a look at the 'values'
  loop at the end. It calls next() one too many times. Therefore, the
  beginning of the next iteration has to deal with the lexer already having a
  token loaded. This is accomplished simply by loading a token _before_ the
  loop starts, and then loading new tokens at the _end_ of each parsing block.
*/

bool MNGParser::Parse(list<FunctionNode> *tree)
{
  assert (tree);
  return MNGParser::ParseTopLevel(tree);
} 
 
bool MNGParser::ParseTopLevel(list<FunctionNode> *tree)
{
  if (!lexer->next()) return LexFail();
  while (lexer->token() != Lx::EOI)
  {
    FunctionNode function;
    if (!ParseFunction(&function)) return false; // Expectation handled inside
    tree->push_back (function);
  }
  return true;
}

bool MNGParser::ParseFunction(FunctionNode *node)
{
  // Parse name
  string name;
  if (!ParseString(&name)) return expected ("function");
  node->name = name;
  
  return ParseUnnamedFunction(node);
}

bool MNGParser::ParseUnnamedFunction(FunctionNode *node)
{
  bool function = false;
 
  // Now we can parse args, args block, block, or operator arg
  
  // If it starts with a parentheses, it's an arglist
  if (lexer->token() == Lx::LPAREN) {
    if (!lexer->next()) return LexFail(); // Done with LPAREN
    
    function = true;
    while (lexer->token() != Lx::RPAREN)
    {
      variant<FunctionNode, NameNode, NumberNode> n;
      if (!ParseArg (&n)) return false; // Expectation handled inside
      if (lexer->token() != Lx::RPAREN) if (!ParseComma()) return expected("comma between arguments");
      node->args.push_back (n);
    }
    
    if (!lexer->next()) return LexFail(); // Done with RPAREN
  }
  
  // If it starts with a curly brace, it's a block
  if (lexer->token() == Lx::LBRACE) {
    if (!lexer->next()) return LexFail(); // Done with LBRACE
    
    function = true;
    while (lexer->token() != Lx::RBRACE)
    {
      FunctionNode n;
      if (!ParseFunction(&n)) return false; // Expectation handled inside
      node->block.push_back (n);
    }
    
    if (!lexer->next()) return LexFail(); // Done with RBRACE
  }
  
  // If it didn't have either args or a block, then it must be an operand
  if (!function)
  {
    string op;
    variant<FunctionNode, NameNode, NumberNode> n;
    if (!ParseOperator(&op)) return expected("argument list, block, or operator");
    if (!ParseArg(&n)) return false; // Expectation handled inside
    
    node->args.push_back (NameNode (node->name));
    node->args.push_back (n);
    node->name = op;
  }
  
  return true;
}

bool MNGParser::ParseArg(variant<FunctionNode, NameNode, NumberNode> *node)
{
  // An arg can be a string, a number, or a  function (without a block, but that's
  // a bit hard to test)
  if (lexer->token() == Lx::NUMBER)
  {
    float f;
    if (!ParseNumber(&f)) return false; // Expectation unneeded
    *node = NumberNode (f);
    return true;
  }
  
  // Problem: Both names *and* functions start with strings =/
  // Solution: have method parse_unnamed_function (name)
  if (lexer->token() == Lx::STRING)
  {
    string name;
    if (!ParseString(&name)) return false; // Expectation unneeded
    FunctionNode f;
    switch (lexer->token())
    {
      // If it looks like a function after the string, then it's a function!
      // Note: The lexer needs to define a token called OPERATOR
      case Lx::LPAREN:
      case Lx::LBRACE:
      case Lx::OPERATOR:
        f.name = name;
        if (!ParseUnnamedFunction(&f)) return false; // Expectation handled inside
        *node = f;
        return true;
        
      // Otherwise, it's just a name, and we're good
      default:
        *node = NameNode (name);
        return true;
    }
  }
  
  // Not a string (/function) or number? Woops!
  return expected ("number, variable name, or function");
}

bool MNGParser::ParseOperator(string *op) // Expectation handled outside
{
  if (lexer->token() != Lx::OPERATOR) return false;
  // std::cout << "Operator: " << lexer->getString() << std::endl;
  *op = lexer->getString ();
  if (!lexer->next()) return LexFail(); // Done with OPERATOR
  
  return true;
}

bool MNGParser::ParseComma() // Expectation handled above
{
  if (lexer->token() != Lx::COMMA) return false;
  if (!lexer->next()) return LexFail(); // Done with COMMA
  
  return true;
}

bool MNGParser::ParseNumber(float *f) // Expectation handled above
{
  if (lexer->token() != Lx::NUMBER) return false;
  // std::cout << "Number: " << lexer->getNumber() << std::endl;
  *f = lexer->getNumber();
  if (!lexer->next()) return LexFail(); // Done with NUMBER
  
  return true;
}

bool MNGParser::ParseString(string *s) // Expectation handled above
{
  if (lexer->token() != Lx::STRING) return false;
  // std::cout << "String: " << lexer->getString() << std::endl;
  *s = lexer->getString();
  if (!lexer->next()) return LexFail(); // Done with STRING
  
  return true;
}

bool MNGParser::expected(std::string expects)
{
  if (!message.empty()) return false;
  
  boost::format f ("%s:%i:Expected %s, instead got %s");
  f % filename % lexer->getLineNumber() % expects;

  Lx::Token token = lexer->token();
  switch (token)
  {
  case Lx::STRING:
    f % "name"; break;
  case Lx::NUMBER:
    f % "number"; break;
  case Lx::COMMA:
    f % "comma"; break;
  case Lx::OPERATOR:
    f % "operator"; break;
  case Lx::LPAREN:
    f % "opening parentheses"; break;
  case Lx::RPAREN:
    f % "closing parentheses"; break;
  case Lx::LBRACE:
    f % "opening curly-brace"; break;
  case Lx::RBRACE:
    f % "closing curly-brace"; break;
  case Lx::EOI:
    f % "end of input"; break;
  default:
    assert (false);
  }

  message = f.str();

  return false;
}

bool MNGParser::LexFail()
{
  boost::format f("%s:%i:%s");
  f % filename % lexer->getLineNumber();
  
  std::string lexmsg = lexer->getMessage();
  if (lexmsg.empty()) f % "Lex error";
  else f % lexmsg;
  
  message = f.str();
  
  return false;
}
