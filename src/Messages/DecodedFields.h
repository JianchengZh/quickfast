// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef DECODEDFIELDS_H
#define DECODEDFIELDS_H
#include "DecodedFields_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/MessageField.h>
#include <Messages/Field_fwd.h>
namespace QuickFAST{
  namespace Messages{
    /// @brief Internal representation of a set of fields to be encoded or decoded.
    class QuickFAST_Export DecodedFields{
    public:

      /// @brief Virtual destructor
      virtual ~DecodedFields(){};

      /// @brief clear current contents of the field set
      ///
      /// Optionally adjust the capacity.
      /// @param capacity is expected number of fields (0 is no change)
      virtual void clear(size_t capacity = 0)  = 0;

      /// @brief insure the DecodedFields can hold the expected number of fields
      /// @param capacity is expected number of fields.
      virtual void reserve(size_t capacity)  = 0;

      /// @brief Get the count of fields in the set
      ///
      /// Group fields are counted individually.
      /// A Sequence is counted as one field.
      /// @returns the field count.
      virtual size_t size()const  = 0;

      /// @brief Add a field to the set.
      ///
      /// The FieldCPtr is copied, not the actual Field object.
      /// @param identity identifies this field
      /// @param value is the value to be assigned.
//      virtual void addField(const FieldIdentityCPtr & identity, const FieldCPtr & value) = 0;
      virtual void addField(const FieldRegistry & registry, FieldRegistry::Index index, const FieldCPtr & value) = 0;

      /// @brief Get the identity information for the specified field
      /// @param[in] name identifies the desired field
      /// @param[out] identity is the information for the field that was found
      /// @returns true if the field was found
//      virtual bool getIdentity(const std::string &name, FieldIdentityCPtr & identity) const = 0;;
      virtual bool getIdentity(const std::string &name, const FieldIdentity *& identity) const = 0;

      /// @brief identify the application type associated with
      /// this set of fields via typeref.
      virtual void setApplicationType(const std::string & type, const std::string & ns) = 0;

      /// @brief get the application type associated with
      /// this set of fields via typeref.
      virtual const std::string & getApplicationType()const = 0;

      /// @brief get the namespace to qualify application type.
      virtual const std::string & getApplicationTypeNs()const = 0;

      /// @brief create an empty version of this container for nexted fields.
      virtual DecodedFields * createdNestedFields(size_t size)const  = 0;

    private:
    };
  }
}
#endif // DECODEDFIELDS_H
