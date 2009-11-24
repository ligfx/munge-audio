/*
 *  LoopLayer.h
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

#ifndef LOOP_LAYER_H
#define LOOP_LAYER_H

#include "AST.h"
#include <boost/optional.hpp>
#include "Expression.h"
#include <map>
#include <string>

class UpdateBlock {};

class LoopLayer
{
friend class LoopLayerParser;
public:
  

protected:
  std::string name;
  std::string wave;
  boost::optional<UpdateBlock> update;
  std::map<std::string, float> variables;
  boost::optional<Expression> volume;
  boost::optional<Expression> interval;
  boost::optional<Expression> updaterate;
  boost::optional<Expression> beatsynch;
};

class LoopLayerParser
{
public:
  LoopLayerParser (const FunctionNode&);
  
  bool Parse (LoopLayer*);
  const std::string &getMessage();
  
protected:
  std::string message;
  const FunctionNode &node;
};

#endif // LOOP_LAYER_H
