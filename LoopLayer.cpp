/*
 *  LoopLayer.cpp
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

#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include "LoopLayer.h"
#include "Unary.h"

#define foreach BOOST_FOREACH

LoopLayerParser::LoopLayerParser (const FunctionNode &_node) : node (_node) {}

const std::string &LoopLayerParser::getMessage() { return message; }

bool LoopLayerParser::Parse (LoopLayer *layer)
{
  if (!(node.name == "LoopLayer"))
  {
    boost::format f("%s:%i: Expected LoopLayer, got %s");
    f % node.filename % node.lineno % node.name;
    message = f.str();
    return false;
  }
  if (!(node.args.size() == 1))
  {
    boost::format f("%s:%i: LoopLayer expects a single argument");
    f % node.filename % node.lineno;
    message = f.str();
    return false;
  }
  ArgNode arg = node.args.front();
  if (NameNode *n = boost::get<NameNode> (&arg))
    layer->name = n->name;
  else
  {
    boost::format f("%s:%i: LoopLayer expects name as argument");
    f % node.filename % node.lineno;
    message = f.str();
    return false;
  }
  foreach (FunctionNode n, node.block)
  {
    if ((node.name == "BeatSynch")
     or (node.name == "Interval")
     or (node.name == "UpdateRate")
     or (node.name == "Volume"))
    {
      UnaryParser parser (node);
      Expression e;
      if (!parser.Parse (&e))
      {
        message = parser.getMessage();
        return false;
      }
      // TODO: Some things can't happen twice
      if (node.name == "BeatSynch") layer->beatsynch = e;
      else if (node.name == "Interval") layer->interval = e;
      else if (node.name == "UpdateRate") layer->updaterate = e;
      else if (node.name == "Volume") layer->volume = e;
    }
    // else if Update, Variable, Wave
    else
    {
      boost::format f("%s:%i: Expected BeatSynch, Interval, Update, UpdateRate, Variable, Volume, or Wave; got %s");
      f % node.filename % node.lineno % node.name;
      message = f.str();
      return false;
    }
  }
  // TODO: Sanity checking here
  
  return true;
}
