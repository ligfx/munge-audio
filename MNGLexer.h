/*
 *  MNGLexer.h
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

#ifndef MNG_LEXER_H
#define MNG_LEXER_H

#include <string>

class MNGLexer;

/**
  Lexer for MNG (.mng) files
  
  Lexes a character buffer into tokens:
    STRING = \w(\w\d)*
    NUMBER = \-?d+.?\d*
    COMMA = ','
    OPERATOR = '='
    LPAREN = '('
    RPAREN = ')'
    LBRACE = '{'
    RBRACE = '}'
    EOI = <end-of-input>
*/
class MNGLexer
{
  public:
    enum Token
    {
      INTERN_LEX, // Used internally only
      STRING,
      NUMBER,
      COMMA,
      OPERATOR,
      LPAREN,
      RPAREN,
      LBRACE,
      RBRACE,
      EOI
    };
  
    /**
      @param buffer
        Character buffer to lex
    */
    MNGLexer (const char *buf);
    
    /**
      Lexes the next token
      
      @returns Successful
    */
    virtual bool next ();
    
    /**
      Token from last call to next()
    */
    virtual Token token ();
    
    /**
      @returns Current line number
    */
    virtual unsigned int getLineNumber ();
    
    /**
      Error message, if lexer failed
    */
    virtual std::string getMessage ();
    
    /**
      Value of lexed STRING or OPERATOR token
    */
    virtual std::string getString ();
    
    /**
      Value of lexed NUMBER token
    */
    virtual float getNumber ();
    
  protected:
    const char *mng_parse_p;
    unsigned int lineno;
    Token tok;
    std::string message;
    
    float number;
    std::string str;
};

#endif // MNG_PARSER_H
