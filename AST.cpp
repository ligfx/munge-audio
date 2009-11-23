/*
 *  AST.cpp
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

#include "AST.h"
#include <boost/foreach.hpp>

#define foreach BOOST_FOREACH
#define times(num) for (int i = 0; i < num; ++i)

using namespace boost;
using namespace std;

// Private declarations
class FunctionArgEqualityTester
  : public static_visitor<bool>
{
public:
  bool operator() (const FunctionNode&, const FunctionNode&);
  bool operator() (const NameNode&, const NameNode&);
  bool operator() (const NumberNode&, const NumberNode&);
  bool operator()
    (const variant<FunctionNode, NameNode, NumberNode>&,
     const variant<FunctionNode, NameNode, NumberNode>&);
};

class PrettyPrinter
  : public boost::static_visitor<>
{
public:
  PrettyPrinter (std::ostream&);

  void operator() (const FunctionNode&);
  void operator() (const NumberNode&);
  void operator() (const NameNode&);
  
protected:
  int indentation;
  std::ostream &out;
};

void PrettyPrint (const std::list<FunctionNode>&, std::ostream &out = std::cout);

// Function definitions

bool FunctionArgEqualityTester::operator()
  (const FunctionNode &a, const FunctionNode &b)
{
  return a == b; 
}

bool FunctionArgEqualityTester::operator()
  (const NameNode &a, const NameNode &b)
{
  return a.name == b.name;
}

bool FunctionArgEqualityTester::operator()
  (const NumberNode &a, const NumberNode &b)
{
  return a.number == b.number;
}

bool FunctionArgEqualityTester::operator()
  (const variant<FunctionNode, NameNode, NumberNode> &a,
   const variant<FunctionNode, NameNode, NumberNode> &b)
{
  return apply_visitor (*this, a, b);
}

bool
operator== (const FunctionNode &a, const FunctionNode &b)
{
  if (!(a.name == b.name)) return false;
  
  if (!equal(a.args.begin(), a.args.end(), b.args.begin(), FunctionArgEqualityTester())) return false;
  return equal(a.block.begin(), a.block.end(), b.block.begin());
}

PrettyPrinter::PrettyPrinter(ostream &_out) : indentation (0), out(_out) {}

void PrettyPrinter::operator() (const FunctionNode &n)
{
  times (indentation) out << "\t";
  
  // Operators are special
  if (n.name == "=")
  {
    apply_visitor (*this, *n.args.begin());
    out << " " << n.name << " ";
    apply_visitor (*this, *++n.args.begin());
    out << endl;
    return;
  }
  
  // Everything else
  out << n.name;
  // Prettyprint args
  if (!n.args.empty())
  {
    out << " (";
    list<variant<FunctionNode, NameNode, NumberNode> >::const_iterator i;
    for (i = n.args.begin(); i != --n.args.end(); ++i)
    {
      apply_visitor (*this, *i);
      out << ", ";
    }
    apply_visitor (*this, *--n.args.end());
    out << ")";
  }
  out << endl;
  // Prettyprint block
  if (!n.block.empty())
  {
    times (indentation) out << "\t";
    out << "{" << endl;
    indentation += 1;
    
    foreach (const FunctionNode n, n.block)
     (*this)(n);
    
    indentation -= 1;
    times (indentation) out << "\t";
    out << "}" << endl;
  }
}

void PrettyPrinter::operator() (const NumberNode &n)
{
  out << n.number;
}

void PrettyPrinter::operator() (const NameNode &n)
{
  out << n.name;
}
  
void PrettyPrint (const list<FunctionNode> &tree, ostream &out)
{
  PrettyPrinter pp (out);
  foreach (const FunctionNode n, tree)
    pp (n);
}

ostream &operator<< (ostream &out, const list<FunctionNode> &tree)
{
  PrettyPrint (tree, out);
  return out;
}
