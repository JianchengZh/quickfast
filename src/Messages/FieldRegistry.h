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

#include <Messages/FieldSet_fwd.h>
#include <Messages/FieldIdentity.h>

namespace QuickFAST{
  namespace Messages{

    /// @brief this class exists solely to allow FieldRegistry to store FieldIdentities
    /// in a vector without requiring public constructor/destructors for FieldIdentity
    class FieldRegistryEntry : public FieldIdentity
    {
    public:
      explicit FieldRegistryEntry(
        const std::string & name,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "")
        : FieldIdentity(name, fieldNamespace, applicationType, applicationTypeNamespace)
      {
      }

      /// @brief private constructor prevents stack or static allocations, or embedding
      ~FieldRegistryEntry()
      {
      }
    };

    /// @brief A registry of known field identities.
    ///
    class QuickFAST_Export FieldRegistry
    {
    public:

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
      FieldHandle addFieldIdentity(
        const std::string & localName,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "");

      FieldHandle findHandle(
        const std::string & localName,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "") const;

      FieldHandle findHandleQualified(
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

      FieldIdentity & get(FieldHandle index);
      const FieldIdentity & get(FieldHandle index) const;

      size_t size()const
      {
        return identities_.size();
      }

    private:
      /// @brief fast access via indexing
      typedef std::vector<FieldRegistryEntry> FieldIdentityVector;
      /// @brief qualified name to index
      typedef std::map<std::string, size_t> FieldNameToIndex;

      FieldIdentityVector identities_;
      FieldNameToIndex indexes_;
    };
  }
}
#endif // FIELDREGISTRY_H
