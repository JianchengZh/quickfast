// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldRegistry.h"
#include <Common/Exceptions.h>

//#include <Codecs/Field.h>
//#include <Codecs/DictionaryIndexer.h>

using namespace QuickFAST;
using namespace QuickFAST::Messages;


FieldRegistry::Index
FieldRegistry::addFieldIdentity(
  const std::string & localName,
  const std::string & fieldNamespace,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace)
{
  Index index = identities_.size();
  identities_.push_back(FieldIdentity(localName, fieldNamespace, applicationType, applicationTypeNamespace));
  std::string qualifiedName;
  qualifyName(qualifiedName, localName, fieldNamespace, applicationType, applicationTypeNamespace);
  indexes_[qualifiedName] = index;
  return index;
}

FieldRegistry::Index
FieldRegistry::findIndex(
  const std::string & localName,
  const std::string & fieldNamespace,
  const std::string & applicationType,
  const std::string & applicationTypeNamespace) const
{
  std::string qualifiedName;
  qualifyName(qualifiedName, localName, fieldNamespace, applicationType, applicationTypeNamespace);
  return findIndexQualified(qualifiedName);
}

FieldRegistry::Index
FieldRegistry::findIndexQualified(
  const std::string & qualifiedName) const
{
  Index index = UNKNOWN;
  FieldNameToIndex::const_iterator it = indexes_.find(qualifiedName);
  if(it != indexes_.end())
  {
    index = it->second;
  }
  return index;
}

FieldIdentity &
FieldRegistry::get(FieldRegistry::Index index)
{
  throw ::QuickFAST::FieldNotPresent("Internal error: Field Identity index out of range.");
}

const FieldIdentity &
FieldRegistry::get(FieldRegistry::Index index) const
{
  throw ::QuickFAST::FieldNotPresent("Internal error: Field Identity index out of range.");
}



