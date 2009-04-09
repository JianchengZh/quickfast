// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldIdentity.h"

using namespace QuickFAST;
using namespace Messages;

void
FieldIdentity::qualifyName(
  std::string & qualifiedName,
  const std::string & localName,
  const std::string & fieldNamespace,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace
  )
{
  if(!applicationTypeNamespace.empty())
  {
    qualifiedName = applicationTypeNamespace;
  }
  else
  {
    qualifiedName.clear();
  }
  qualifiedName += '/';
  if(!applicationType.empty())
  {
    qualifiedName += applicationType;
  }
  qualifiedName += '/';
  if(!fieldNamespace.empty())
  {
    qualifiedName += fieldNamespace;
  }
  qualifiedName += '/';
  qualifiedName += localName;
}
