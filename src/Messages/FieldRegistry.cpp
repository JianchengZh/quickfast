// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldRegistry.h"
#include <Common/Exceptions.h>

namespace QuickFAST{
  namespace Messages{
  }
}

using namespace QuickFAST;
using namespace QuickFAST::Messages;


FieldHandle
FieldRegistry::addFieldIdentity(
  const std::string & localName,
  const std::string & fieldNamespace,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace)
{
  FieldHandle index = identities_.size();
  identities_.push_back(FieldRegistryEntry(localName, fieldNamespace, applicationType, applicationTypeNamespace));
  std::string qualifiedName;
  qualifyName(qualifiedName, localName, fieldNamespace, applicationType, applicationTypeNamespace);
  indexes_[qualifiedName] = index;
  return index;
}

FieldHandle
FieldRegistry::findHandle(
  const std::string & localName,
  const std::string & fieldNamespace,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace) const
{
  std::string qualifiedName;
  qualifyName(qualifiedName, localName, fieldNamespace, applicationType, applicationTypeNamespace);
  return findHandleQualified(qualifiedName);
}

FieldHandle
FieldRegistry::findHandleQualified(
  const std::string & qualifiedName) const
{
  FieldHandle index = FieldHandleUnknown;
  FieldNameToIndex::const_iterator it = indexes_.find(qualifiedName);
  if(it != indexes_.end())
  {
    index = it->second;
  }
  return index;
}

FieldIdentity &
FieldRegistry::get(FieldHandle index)
{
  if(index >= identities_.size())
  {
    throw ::QuickFAST::FieldNotPresent("Internal error: Field Identity index out of range.");
  }
  return identities_[index];
}

const FieldIdentity &
FieldRegistry::get(FieldHandle index) const
{
  if(index >= identities_.size())
  {
    throw ::QuickFAST::FieldNotPresent("Internal error: Field Identity index out of range.");
  }
  return identities_[index];
}



