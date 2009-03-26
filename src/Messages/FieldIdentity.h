// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDIDENTITY_H
#define FIELDIDENTITY_H
#include "FieldIdentity_fwd.h"
#include <Common/Types.h>
#include <Messages/Field_fwd.h>

namespace QuickFAST{
  namespace Messages{
    /// @brief Identify a field within a FieldSet
    ///
    /// Keeping the field's identity separate from it's type and value allows
    /// immutable fields to be shared among different field containers (dictionaries & field sets, &tc.)
    class QuickFAST_Export FieldIdentity
    {
    public:
      /// @brief Construct the FieldIdentity
      /// @param name the localname for the field
      /// @param fieldNamespace the namespace in which the localname is defined
      /// @param applicationType the record type containing this field
      /// @param applicatoinTypenamespace the namespace in which the application type is defined
      explicit FieldIdentity(
        const std::string & name,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "")
        : localName_(name)
        , fieldNamespace_(fieldNamespace)
        , applicationType_(applicationType)
        , applicationTypeNamespace_(applicationTypeNamespace)
//        , mandatory_(true)
        , refcount_(0)
      {
        qualifyName(qualifiedName_, localName_, fieldNamespace_, applicationType_, applicationTypeNamespace_);
      }

      /// @brief Construct an anonomous FieldIdentity
      FieldIdentity();

//    private:
      /// @brief private constructor prevents stack or static allocations, or embedding
      ~FieldIdentity()
      {
      }
    public:

/*
      /// @brief set name after construction
      /// @param name the localname for the field
      void setName(const std::string & name)
      {
        localName_ = name;
        qualifyName(qualifiedName_, localName_, fieldNamespace_, applicationType_, applicationTypeNamespace_);
      }

      /// @brief Set Namespace after construction
      /// @param fieldNamespace the namespace in which the localname is defined
      void setNamespace(const std::string & fieldNamespace)
      {
        fieldNamespace_ = fieldNamespace;
        qualifyName(qualifiedName_, localName_, fieldNamespace_, applicationType_, applicationTypeNamespace_);
      }

      /// @brief set name after construction
      /// @param name the localname for the field
      void setApplicationType(const std::string & type)
      {
        applicationType_ = type;
        qualifyName(qualifiedName_, localName_, fieldNamespace_, applicationType_, applicationTypeNamespace_);
      }

      /// @brief Set Namespace after construction
      /// @param fieldNamespace the namespace in which the localname is defined
      void setApplicationTypeNamespace(const std::string & applicationTypeNamespace)
      {
        applicationTypeNamespace_ = applicationTypeNamespace;
        qualifyName(qualifiedName_, localName_, fieldNamespace_, applicationType_, applicationTypeNamespace_);
      }
*/

      /// @brief Copy construct the FieldIdentity
      /// @param rhs is the FieldIdentity from which to copy
      FieldIdentity(const FieldIdentity & rhs)
        : localName_(rhs.localName_)
        , fieldNamespace_(rhs.fieldNamespace_)
        , qualifiedName_(rhs.qualifiedName_)
        , id_(rhs.id_)
//        , mandatory_(rhs.mandatory_)
      {
      }

      /// @brief Set the fields ID (not terribly useful)
      /// @param id to be stored
      void setId(const field_id_t & id)
      {
        id_ = id;
      }

      /// @brief get the fully qualified name of the field.
      /// @returns the name qualified by the namespace (from the cached value)
      const std::string & name()const
      {
        return qualifiedName_;
      }

      /// @brief get the localname of a field
      /// @returns the unqualified name
      const std::string & getLocalName()const
      {
        return localName_;
      }

      /// @brief get the namespace of a field
      /// @returns the namespace
      const std::string & getNamespace()const
      {
        return fieldNamespace_;
      }
      const std::string & getTypeName() const
      {
        return applicationType_;
      }

      const std::string & getTypeNamespace()const
      {
        return applicationTypeNamespace_;
      }


      /// @brief get the field id
      /// @returns the id assigned to this field.
      const field_id_t & id()const
      {
        return id_;
      }
/*
      /// @brief set the mandatory flag for this field.
      /// @param mandatory is true if the field must appear in the application data type
      void setMandatory(bool mandatory)
      {
        int todo_Find_a_new_home;
        mandatory_ = mandatory;
      }

      /// @brief access the mandatory flag
      /// @returns the value as set by setMandatory
      bool mandatory()const
      {
        return mandatory_;
      }
*/
      static void qualifyName(
        std::string & qualifiedName,
        const std::string & localName,
        const std::string & fieldNamespace = "",
        const std::string & applicationType = "",
        const std::string & applicationTypeNamespace = "");

    private:
      std::string localName_;
      std::string fieldNamespace_;
      std::string applicationType_;
      std::string applicationTypeNamespace_;

      std::string qualifiedName_; // cached for performance
      field_id_t id_;
//      bool mandatory_;
    private:
      friend void QuickFAST_Export intrusive_ptr_add_ref(const FieldIdentity * ptr);
      friend void QuickFAST_Export intrusive_ptr_release(const FieldIdentity * ptr);
      friend void QuickFAST_Export intrusive_ptr_add_ref(FieldIdentity * ptr);
      friend void QuickFAST_Export intrusive_ptr_release(FieldIdentity * ptr);
      void freeFieldIdentity()const
      {
        delete this;
      }

      mutable unsigned long refcount_;
    };

    inline
    void QuickFAST_Export
    intrusive_ptr_add_ref(const Messages::FieldIdentity * ptr)
    {
      ++ptr->refcount_;
    }

    inline
    void QuickFAST_Export
    intrusive_ptr_release(const Messages::FieldIdentity * ptr)
    {
      if(--ptr->refcount_ == 0)
      {
        ptr->freeFieldIdentity();
      }
    }

    inline
    void QuickFAST_Export
    intrusive_ptr_add_ref(Messages::FieldIdentity * ptr)
    {
      ++ptr->refcount_;
    }

    inline
    void QuickFAST_Export
    intrusive_ptr_release(Messages::FieldIdentity * ptr)
    {
      if(--ptr->refcount_ == 0)
      {
        ptr->freeFieldIdentity();
      }
    }
  }
}
#endif // FIELDIDENTITY_H
