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
#include <boost/variant/static_visitor.hpp>
#include <iostream>
#include <list>

struct Node;
struct FunctionNode;
struct NameNode;
struct NumberNode;
typedef boost::variant<FunctionNode, NameNode, NumberNode> ArgNode;

// The actual nodes an AST is made of
struct Node
{
  Node () : lineno(0) {}
  unsigned int lineno;
  std::string filename;
};

struct FunctionNode : public Node
{
  FunctionNode () : Node() {}
  std::string name;
  std::list<boost::variant<FunctionNode, NameNode, NumberNode> > args;
  std::list<FunctionNode> block;
};

struct NameNode : public Node
{
  NameNode (std::string n) : Node(), name (n) {}
  std::string name;
};

struct NumberNode : public Node
{
  NumberNode (float f) : Node(), number (f) {}
  float number;
};

// Visitor
class ASTVisitor
  : public boost::static_visitor<>
{
public:
  virtual ~ASTVisitor();
  void operator() (const FunctionNode &n);
  void operator() (const NameNode &n);
  void operator() (const NumberNode &n);
  void visit (const ArgNode &a);
  
  virtual void visit (const std::list<FunctionNode>&);
  virtual void visit (const FunctionNode&);
  virtual void visit (const NameNode&);
  virtual void visit (const NumberNode&);
};

// Helpers
bool operator== (const FunctionNode&, const FunctionNode&);

std::ostream &operator<< (std::ostream&, const std::list<FunctionNode>&);

#endif // AST_H
