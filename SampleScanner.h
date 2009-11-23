/*
 *  SampleScanner.h
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
 
#ifndef SAMPLE_SCANNER_H
#define SAMPLE_SCANNER_H

#include "AST.h"

class SampleScanner
 : public ASTVisitor
{
public:
  using ASTVisitor::visit;

  virtual ~SampleScanner();
  virtual void visit (const FunctionNode &f);
  
  std::list<std::string> getNames ();
  
protected:
  std::list<std::string> names;
};

#endif
