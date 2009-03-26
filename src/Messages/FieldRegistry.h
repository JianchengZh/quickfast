// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif

#ifndef FIELDREGISTRY_H
#define FIELDREGISTRY_H

#include "FieldRegistry_fwd.h"
#include <Common/QuickFAST_Export.h>

#include <Messages/FieldIdentity.h>

namespace QuickFAST{
  namespace Messages{
    /// @brief A registry of known field identities.
    ///
    class QuickFAST_Export FieldRegistry
    {
    public:
      /// @brief Mostly opaque "handle" to the field identity
      typedef size_t Index;
      /// @brief Special value to indicate an unknown field.
      static const Index UNKNOWN = ~0;

      /// @brief Construct and empty registry.
      FieldRegistry()
      {
      }

      /// @brief Virtual destructor.
      virtual ~FieldRegistry()
      {
      }

      /// @brief Add a definition to the registry
      /// @param value smart pointer to the template to be added
      Index addFieldIdentity(
        const std::string & localName,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "");

      Index findIndex(
        const std::string & localName,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "") const;

      Index findIndexQualified(
        const std::string & qualifiedName) const;

      static void qualifyName(
        std::string & qualifiedName,
        const std::string & localName,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "")
      {
        FieldIdentity::qualifyName(qualifiedName, localName, fieldNamespace, applicationType, applicationTypeNamespace);
      }

      FieldIdentity & get(Index index);
      const FieldIdentity & get(Index index) const;

      size_t size()const
      {
        return identities_.size();
      }

    private:
      /// @brief fast access via indexing
      typedef std::vector<FieldIdentity> FieldIdentityVector;
      /// @brief qualified name to index
      typedef std::map<std::string, size_t> FieldNameToIndex;

      FieldIdentityVector identities_;
      FieldNameToIndex indexes_;
    };
  }
}
#endif // FIELDREGISTRY_H
