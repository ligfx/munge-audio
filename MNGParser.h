/*
 *  MNGParser.h
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

#ifndef MNG_PARSER_H
#define MNG_PARSER_H

#include <string>

/**
  Parser for MNG (.mng) files
  
  Grammar:
  
  mng := function*

  function := name args
  function := name block
  function := name args block
  function := name operator arg

  block := lbrace function* rbrace

  args := lparen (arg comma)* rparen
  
  arg := name | function | number
  
  name := \w(\w\d)*
  number := -?\d+\.?\d*
  
  operator := "="
  comma := ","
  lparen := "("
  rparen := ")"
  lbrace := "{"
  rbrace := "}"
  
*/
class MNGParser
{
  public:
    /**
      @param lexer
        MNGLexer used as input to parser
      @param filename
        Name of file being parsed (used in error messsages)
    */
    MNGParser (MNGLexer*, std::string filename);
    virtual ~MNGParser ();
    
    virtual bool Parse ();
    
    /**
      Error message, if parser failed
    */
    virtual std::string getMessage ();
  
  protected:
    bool ParseTopLevel();
    bool ParseFunction();
    bool ParseUnnamedFunction();
    bool ParseArg();
    bool ParseOperator();
    bool ParseComma();
    bool ParseNumber();
    bool ParseString();
    
    bool LexFail();
    bool expected(std::string);
  
  protected:
    MNGLexer *lexer;
    std::string message;
    std::string filename;
};

#endif // MNG_PARSER_H
