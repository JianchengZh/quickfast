// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldIdentity.h"

#ifdef _MSC_VER
#pragma warning(disable:4355) // disable warning C4355: 'this' : used in base member initializer list
#endif
using namespace QuickFAST;
using namespace Messages;

namespace
{
  std::string anonName(void * address)
  {
    return boost::lexical_cast<std::string>(address);
  }
}

FieldIdentity::FieldIdentity()
  : /*mandatory_(true)
  , */localName_(anonName(this))
  , refcount_(0)
{
  qualifyName(qualifiedName_, localName_, fieldNamespace_, applicationType_, applicationTypeNamespace_);
}

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
