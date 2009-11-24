/*
 *  Expression.h
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

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "AST.h"
#include <map>

class ConstantExpression;
class Expression;
class FunctionExpression;
class VariableExpression;

class IRandomGenerator
{
public:
  virtual ~IRandomGenerator();
  virtual float next (float left, float right) =0;
};

float expression_add (float, float);
float expression_cosinewave (float, float);
float expression_divide (float, float);
float expression_multiply (float, float);
float expression_random (float, float);
float expression_sinewave (float, float);
float expression_subtract (float, float);

class FunctionExpression
{
  friend class Expression;
  friend class ExpressionParser;
  
  float(*function)(float, float);
  boost::shared_ptr<Expression> left, right;
};
class VariableExpression
{
  friend class Expression;
  friend class ExpressionParser;

  std::string name;
};
class ConstantExpression
{
  friend class Expression;
  friend class ExpressionParser;

  float number;
};


class Expression
{
  friend class ExpressionParser;
public:
  Expression ();
  
  float getValue (std::map<std::string, float> variables, IRandomGenerator*);

protected:
  boost::variant<FunctionExpression, VariableExpression, ConstantExpression> expression;
};

class ExpressionParser
{
public:
  ExpressionParser (const ArgNode&);
  
  bool Parse (Expression*);
  const std::string &getMessage ();
  
protected:
  bool CheckAbsenceOfBlock (const FunctionNode&);
  bool CheckTwoParameters (const FunctionNode&);
  
protected:
  std::string message;
  const ArgNode &node;
};

#endif // TRACK_PLAYER_H
