/*
 *  AST.h
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

#ifndef AST_H
#define AST_H

#include <boost/shared_ptr.hpp>
#include <boost/variant.hpp>
#include <list>

struct Node;
struct FunctionNode;
struct NameNode;
struct NumberNode;

struct Node
{
  unsigned int lineno;
  std::string filename;
};

struct FunctionNode : public Node
{
  std::string name;
  std::list<boost::variant<FunctionNode, NameNode, NumberNode> > args;
  std::list<FunctionNode> block;
};

struct NameNode : public Node
{
  NameNode (std::string n) : name (n) {}
  std::string name;
};

struct NumberNode : public Node
{
  NumberNode (float f) : number (f) {}
  float number;
};

#endif // AST_H
