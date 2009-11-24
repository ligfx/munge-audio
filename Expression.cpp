/*
 *  Expression.cpp
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
#include "Expression.h"

using namespace boost;
using namespace std;

IRandomGenerator::~IRandomGenerator () {}

float expression_add (float left, float right) { return left + right; }
float expression_cosinewave (float, float) { assert (false); }
float expression_divide (float left, float right) { return left / right; } // TODO: Correct?
float expression_multiply (float left, float right) { return left * right; }
float expression_random (float, float) { assert (false); }
float expression_sinewave (float, float) { assert (false); }
float expression_subtract (float left, float right) { return left - right; } // TODO: Correct?

Expression::Expression () {}

float Expression::getValue
  (map<string, float> variables,
   IRandomGenerator *random)
{
  if (ConstantExpression *constant = get<ConstantExpression> (&expression))
  {
    return constant->number;
  }
  else if (VariableExpression *variable = get<VariableExpression> (&expression))
  {
    map<string, float>::const_iterator it = variables.find (variable->name);
    if (it == variables.end())
    {
      // TODO: Some sort of error mechanism...
      //       Should we check this statically
      assert (false);
    }
    return (*it).second;
  }
  else assert (false);
}

ExpressionParser::ExpressionParser (const ArgNode &_node) : node (_node) {}

bool ExpressionParser::CheckAbsenceOfBlock (const FunctionNode &node)
{
  if (node.block.empty()) return true;
  
  format f ("%s does not take a block");
  f % node.name;
  message = f.str();
  return false;
}

bool ExpressionParser::CheckTwoParameters (const FunctionNode &node)
{
  if (node.args.size() == 2) return true;
  
  format f ("%s expects two arguments");
  f % node.name;
  message = f.str();
  return false;
}

bool ExpressionParser::Parse (Expression *expr)
{ 
  if (const FunctionNode *f = get<FunctionNode> (&node))
  {
    if ((f->name == "Add")
     or (f->name == "CosineWave")
     or (f->name == "Divide")
     or (f->name == "Multiply")
     or (f->name == "Random")
     or (f->name == "SineWave")
     or (f->name == "Subtract"))
    {
      if (!CheckAbsenceOfBlock (*f)) return false;
      if (!CheckTwoParameters (*f)) return false;
      
      shared_ptr<Expression> left (new Expression);
      ArgNode leftarg = f->args.front();
      ExpressionParser leftparser (leftarg);
      if (!leftparser.Parse (left.get()))
      {
        message = leftparser.getMessage();
        return false;
      }
      
      shared_ptr<Expression> right (new Expression);
      ArgNode rightarg = *++f->args.begin();
      ExpressionParser rightparser (rightarg);
      if (!rightparser.Parse (right.get()))
      {
        message = rightparser.getMessage();
        return false;
      }
      
      FunctionExpression func;
      func.left = left;
      func.right = right;
      
      if (f->name == "Add") func.function = &expression_add;
      else if (f->name == "CosineWave") func.function = &expression_cosinewave;
      else if (f->name == "Divide") func.function = &expression_divide;
      else if (f->name == "Multiply") func.function = &expression_multiply;
      else if (f->name == "Random") func.function = &expression_random;
      else if (f->name == "SineWave") func.function = &expression_sinewave;
      else if (f->name == "Subtract") func.function = &expression_subtract;
      
      expr->expression = func;
      return true;
    }
    else
    {
    boost::format s (
      "Expected constant, variable name, Add, CosineWave, Divide, Multiply, "
      "Random, SineWave, or Subtract; got %s");
    s % f->name;
    message = s.str();
    return false;
    }
  }
  else if (const NameNode *n = get<NameNode> (&node))
  {
    VariableExpression variable;
    variable.name = n->name;
    expr->expression = variable;
    return true;
  }
  else if (const NumberNode *n = get<NumberNode> (&node))
  {
    ConstantExpression constant;
    constant.number = n->number;
    expr->expression = constant;
    return true;
  }
  else assert (false);
}

const std::string &ExpressionParser::getMessage () { return message; }
