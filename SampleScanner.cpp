/*
 *  SampleScanner.cpp
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
 
#include <iostream>
#include "SampleScanner.h"

using namespace std;
 
SampleScanner::~SampleScanner () {}
 
list<string> SampleScanner::getNames () { return names; }
 
void SampleScanner::visit (const FunctionNode &f)
{
  // SampleScanner
  // If it's a well-formed Wave node, grab the sample name
  if ((f.name == "Wave")
  and (f.args.size() == 1))
  {
    ArgNode first = *f.args.begin();
    NameNode *f = boost::get<NameNode>(&first);
    if (f)
    // Only names we haven't seen yet!
    if (find (names.begin(), names.end(), f->name) == names.end())
        names.push_back (f->name);
  }
  
  // If it's not well-formed, or not even a Wave node, just act like normal
  ASTVisitor::visit (f);
}
