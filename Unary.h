/*
 *  Unary.h
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

#ifndef UNARY_H
#define UNARY_H

#include "AST.h"

class Expression;

class UnaryParser
{
public:
  UnaryParser (const FunctionNode&);
  
  bool Parse (Expression*);
  const std::string &getMessage ();
  
protected:
  bool CheckAbsenceOfBlock (const FunctionNode&);
  bool CheckSingleParameter (const FunctionNode&);
  
protected:
  std::string message;
  const FunctionNode &node;
};

#endif // UNARY_H
