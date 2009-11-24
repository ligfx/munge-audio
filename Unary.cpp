/*
 *  Unary.cpp
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
#include "Unary.h"

UnaryParser::UnaryParser (const FunctionNode &_node) : node (_node) {}

bool UnaryParser::CheckAbsenceOfBlock (const FunctionNode &node)
{
  if (node.block.empty()) return true;
  
  boost::format f ("%s:%i: %s does not take a block");
  f % node.filename % node.lineno % node.name;
  message = f.str();
  return false;
}

bool UnaryParser::CheckSingleParameter (const FunctionNode &node)
{
  if (node.args.size() == 1) return true;
  
  boost::format f ("%s:%i: %s expects a single argument");
  f % node.filename % node.lineno % node.name;
  message = f.str();
  return false;
}

bool UnaryParser::Parse (Expression *expr)
{ 
  if ((node.name == "BeatLength")
   or (node.name == "BeatSynch")
   or (node.name == "Delay")
   or (node.name == "FadeIn")
   or (node.name == "FadeOut")
   or (node.name == "Interval")
   or (node.name == "Pan")
   or (node.name == "TempoDelay")
   or (node.name == "UpdateRate")
   or (node.name == "Volume"))
  {
    if (!CheckAbsenceOfBlock (node)) return false;
    if (!CheckSingleParameter (node)) return false;
    
    ArgNode arg = node.args.front();
    ExpressionParser parser (arg);
    if (!parser.Parse (expr))
    {
      message = parser.getMessage();
      return false;
    }
    else return true;
  }
  else
  {
    boost::format f (
      "%s:%i: Expected BeatLength, BeatSynch, Delay, FadeIn, FadeOut, Interval, "
      "Pan, TempoDelay, UpdateRate, or Volume; got %s");
    f % node.filename % node.lineno % node.name;
    message = f.str();
    return false;
  }
}

const std::string &UnaryParser::getMessage () { return message; }
