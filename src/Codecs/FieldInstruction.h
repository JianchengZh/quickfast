// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDINSTRUCTION_H
#define FIELDINSTRUCTION_H

#include "FieldInstruction_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Common/Types.h>
#include <Common/Constants.h>
#include <Common/WorkingBuffer.h>
#include <Common/Exceptions.h>
#include <Messages/FieldSet_fwd.h>
#include <Messages/Message_fwd.h>
#include <Messages/DecodedFields_fwd.h>
#include <Messages/FieldIdentity.h>
#include <Messages/FieldRegistry.h>
#include <Codecs/SchemaElement.h>
#include <Codecs/DataSource.h>
#include <Codecs/Context.h>
#include <Codecs/FieldOp.h>
#include <Codecs/PresenceMap.h>

#include <Common/Profiler.h>

namespace QuickFAST{
  namespace Codecs{
    /// @brief Instruction: an element of a Template.
    ///
    /// Specializations: TemplateRef, Group, Sequence,
    /// String(Ascii), String(unicode), ByteVector
    /// UINT64, INT64, UINT32, INT32, Decimal
    ///
    /// Note typeref is not a FieldInstruction, it's a setApplicationType()
    /// call on the containing SegmentBody
    ///
    /// <pre>
    /// Instruction Attribute           method to call
    /// =====================           ==============
    /// name="name" [required]          constructor(name, namespace)
    /// ns="namespace" [optional]
    ///
    /// Presence=mandatory|optional     setPresence();
    /// dictionary=(template|type|global|"user")
    ///                                 setDictionaryName();
    /// key="token"                     setKey();
    /// id="id"                         setId();
    ///
    /// Embedded Operations
    ///  i.e.: constant, default, copy, increment, delta, tail, "nop")
    ///  Value=literal (for constant, default, copy)
    ///                                 setFieldOp(FieldOPXXXX, value);
    ///
    /// </pre>

    class QuickFAST_Export FieldInstruction
      : public SchemaElement
    {
    protected:
      /// @brief Derived class can constuct given a name and namespace
      /// @param name is the localname for this field
      /// @param fieldNamespace qualifies localname
      FieldInstruction(
        Messages::FieldRegistry & fieldRegistry,
        const std::string & name,
        const std::string & fieldNamespace,
        const std::string & type,
        const std::string & typeNamespace);

      /// @brief Derived class can also construct an anonymous Field Instruction
//      FieldInstruction();

    public:
      /// @brief a typical virtual destructor.
      virtual ~FieldInstruction();

      /// @brief set the id of this field
      /// @param id is the id for this field.
      void setId(const field_id_t & id)
      {
        fieldRegistry_.get(fieldIndex_).setId(id);
//        fieldRegistry_.get(fieldIndex_).setId(id);
      }

#if 0
      /// @brief set the name of this field instruction after construction
      /// @param name is the localname for this field
      void setName(const std::string & name)
      {
        fieldRegistry_.get(fieldIndex_).setName(name);
      }

      /// @brief set the namespace of this field instruction after construction
      /// @param fieldNamespace qualifies localname
      void setNamespace(const std::string & fieldNamespace)
      {
        fieldRegistry_.get(fieldIndex_).setNamespace(fieldNamespace);
      }
#endif

      /// @brief Indicate that the field is mandatory in the application record.
      /// Default if not specified is true.
      /// @param mandatory true for presence="mandatory"; false for presence="optional"
      virtual void setPresence(bool mandatory);

      /// @brief Set a field operation
      ///
      /// Assigns the appropriate dispatching object to this field instruction.
      void setFieldOp(FieldOpPtr fieldOp);

      /// @brief Get the currently used field operation
      bool getFieldOp(FieldOpCPtr & fieldOp) const;

      virtual void setApplicationType(const std::string & type, const std::string & ns)
      {
        applicationType_ = type;
        applicationTypeNamespace_ = ns;
        if(applicationTypeNamespace_.empty())
        {
          qualifiedApplicationType_ = applicationType_;
        }
        else
        {
          qualifiedApplicationType_ = applicationTypeNamespace_ + "." + applicationType_;
        }
      }

      /// @brief get the Application Type assigned to this field instruction
      /// @returns the type assigned to this Fiel
      const std::string & getApplicationType() const
      {
        return qualifiedApplicationType_;
      }

      /// @brief Retrieve the field's name.
      /// @returns the field name.
      const std::string & getName() const
      {
        return fieldRegistry_.get(fieldIndex_).name();
      }

      /// @brief Retrieve the field's id
      /// @returns the fields id
      const field_id_t &
        getId()const
      {
        return fieldRegistry_.get(fieldIndex_).id();
      }

      /// @brief Retrieve the field's identity.
      /// @returns the field identity.
      const Messages::FieldIdentity & getIdentity()const
      {
        return fieldRegistry_.get(fieldIndex_);
      }

      /// @brief Is the field mandatory in the application record?
      /// @returns true if the field is mandatory.
      bool isMandatory()const
      {
        return mandatory_;
      }

      Messages::FieldHandle getFieldIndex() const
      {
        return fieldIndex_;
      }


      /// @brief Implement the dictionary= attribute.
      ///
      /// Defines an dictionary to be used for this element.
      /// Not applicable to all field instructions so the default implementation
      /// throws a TemplateDefinition error.
      /// @param name is the dictionary= attribute of the segment-defining element
      virtual void setDictionaryName(const std::string & name);


      /// @brief set the segment body which defines the contents of a sequence or group.
      ///
      /// Does not apply to other field instructions so the default implementation
      /// throws a TemplateDefinition error.
      /// @param segment is a set of field instructions that define the element
      virtual void setSegmentBody(Codecs::SegmentBodyPtr segment);

      /// @brief How many presence map bits does this field instruction use.
      ///
      /// Note: a call to setOp() or setPresence() invalidates the results of this method.
      /// @returns the maximum number of presence map bits this field might use (0, 1, or 2)
      virtual size_t presenceMapBitsRequired() const;

      /// @brief How many fields will this field instruction produce
      ///
      /// Includes merged groups and templateRefs. If not merged, then the answer is 1
      /// @brief parent SegmentBody is needed to check for merge
      virtual size_t fieldCount(const SegmentBody & parent)const;

      /// @brief Assign a dictionary index to this field and any subfields.
      /// @param indexer assigns the index.
      /// @param dictionaryName is the parent's dictionary name (inherited unless overridden)
      /// @param typeName is the application type for this instruction
      /// @param typeNamespace is the namespace to qualify the application type.
      virtual void buildIndexes(
        DictionaryIndexer & indexer,
        const std::string & dictionaryName,
        const std::string & typeName,
        const std::string & typeNamespace);

      /// @brief Decode the field from a data source.
      ///
      /// @param[in] source supplies the data
      /// @param[in] pmap identifies fields present in the stream
      /// @param decoder provides context for decoding
      /// @param[out] fieldSet receives the decoded fields
      bool decode(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet)const
      {
        return fieldOp_->decode(*this, source, pmap, decoder, fieldSet);
      }

      /// @brief Decode the field from a data source.
      ///
      /// @param destination receives the encoded data
      /// @param pmap is set to indicate which fields are explicitly represented in the encoded data stream
      /// @param encoder provides the context for encoding
      /// @param fieldSet supplies the data to be encoded
      void encode(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet)const
      {
        return fieldOp_->encode(*this, destination, pmap, encoder, fieldSet);
      }

      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueNop();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueConstant();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueDefault();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueCopy();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueDelta();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueIncrement();
      /// Set default value based on operator type (dispatch via FieldOp)
      virtual void setDefaultValueTail();

      ///////////////////
      // Decoding support

      /// @brief Decode when no operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeNop(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const = 0;

      /// @brief Decode when &lt;constant> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeConstant(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      /// @brief Decode when &lt;default> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeDefault(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      /// @brief Decode when &lt;copy> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeCopy(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      /// @brief Decode when &lt;delta> operation is specified.
      /// @see decode()
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeDelta(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      /// @brief Decode when &lt;increment> operation is specified.
      /// @see decode()
      /// Not pure because not all types support increment operator.
      /// Default implementation throws bad operation
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeIncrement(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      /// @brief Decode when &lt;tail> operation is specified.
      /// @see decode()
      /// Not pure because not all types support tail operator.
      /// Default implementation throws bad operation
      /// @param[in] source for the FAST data
      /// @param[in] pmap indicating field presence
      /// @param[in] decoder driving this process
      /// @param[out] fieldSet
      virtual bool decodeTail(
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::DecodedFields & fieldSet) const;

      ///////////////////
      // Encoding support

      /// @brief Encode when no operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeNop(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const = 0;

      /// @brief Encode when &lt;constant> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeConstant(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      /// @brief Encode when &lt;default> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeDefault(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      /// @brief Encode when &lt;copy> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeCopy(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      /// @brief Encode when &lt;delta> operation is specified.
      /// @see encode()
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeDelta(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      /// @brief Encode when &lt;increment> operation is specified.
      /// @see encode()
      /// Default implementation throws bad operation
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeIncrement(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      /// @brief Encode when &lt;tail> operation is specified.
      /// @see encode()
      /// Default implementation throws bad operation
      /// @param destination
      /// @param pmap for fields being encoded
      /// @param encoder driving the encoding process
      /// @param fieldSet
      virtual void encodeTail(
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::FieldSet & fieldSet) const;

      /////////////////////////
      // Basic Decoding methods
      // Developer Note:   These are here to
      // support all derived fielddefs.
      // They are static to make them more generally useful (testable)


      /// @brief Encode a string that's nullable, but not null.
      /// @param destination to which the string will be written.
      /// @param value to be written.
      static void encodeNullableAscii(DataDestination & destination, const std::string & value);

      /// @brief Encode a string.
      /// @param destination to which the string will be written.
      /// @param value to be written.
      static void encodeAscii(DataDestination & destination, const std::string & value);

      /// @brief Helper routine to encode a blob represented as a string; into a destination
      /// @param destination to which the data will be written
      /// @param value to be written to the destination
      static void encodeBlobData(DataDestination & destination, const std::string & value);

      /// @brief Encode signed integer
      ///
      /// Works with all signed integer types.  Variable byte length encoding
      /// assures the most compact result.
      static void encodeSignedInteger(DataDestination & destination, WorkingBuffer & buffer, int64 value);

      /// @brief Encode signed integer
      ///
      /// Works with all signed integer types.  Variable byte length encoding
      /// assures the most compact result.
      /// @param destination receives encoded results
      /// @param buffer provides working space
      /// @param value is the data to be encoded
      static void encodeUnsignedInteger(DataDestination & destination, WorkingBuffer & buffer, uint64 value);

      /// @brief basic decoding for signed integer types.
      ///
      /// Ignores presence map, field operators, etc.  All it does
      /// is decode an integer from the data source./
      /// Used to decode Int32 and Int64 fields.
      /// Also used for exponent and mantissa of Decimal fields
      /// @param[in] source supplies the data
      /// @param context in which the decoding occurs.
      /// @param[out] value returns the result
      /// @param[in] allowOversize ignores one bit of overflow to cope
      ///            with out-of-range deltas (see section 6.3.7.1 of the Fast Specification v1x1)
      /// @returns true if successful; false if EOF
      /// @throws OverflowError if the decoded value doesn't fit the supplied type
      template<typename IntType>
      static void decodeSignedInteger(Codecs::DataSource & source, Codecs::Context & context, IntType & value, bool allowOversize = false);

      /// @brief basic decoding for signed integer types.
      ///
      /// Ignores presence map, field operators, etc.  All it does
      /// is decode an integer from the data source.
      /// Used to decode UInt32 and UInt64 fields.
      /// Also used for length portion of BitVector and Utf8 fields
      /// @param[in] source supplies the data
      /// @param context in which the decoding occurs.
      /// @param[out] value returns the result
      /// @returns true if successful; false if EOF
      /// @throws OverflowError if the decoded value doesn't fit the supplied type
      template<typename UnsignedIntType>
      static void decodeUnsignedInteger(Codecs::DataSource & source, Codecs::Context & context, UnsignedIntType & value);

      /// @brief Check nullable signed or unsigned integer field for null value
      ///
      /// Fixes value to reverse the effect of null encoding.
      /// @param value the value as returned by decodeSignedInteger() or decodeUnsignedInteger()
      /// @returns true if this is a null field
      template<typename IntType>
      static bool checkNullInteger(IntType & value);

      /// @brief Basic decoding for ASCII strings
      /// @param[in] source supplies the data
      /// @param buffer to receive the string
      static bool decodeAscii(
        Codecs::DataSource & source,
        WorkingBuffer & buffer);

      /// @brief Check for zero preamble to encode empty string
      ///
      /// If the string is nullable use checkNullAscii() first.
      /// @param buffer as returned by decodeAscii()
      static bool checkEmptyAscii(WorkingBuffer & buffer);

      /// @brief Check for zero preamble to encode null string
      ///
      /// Must be called before checkEmptyAscii() for nullable strings.
      /// @param buffer as returned by decodeAscii()
      static bool checkNullAscii(WorkingBuffer & buffer);

      /// @brief Helper method to find the longest match at the beginning of two strings
      /// @param previous one of the strings
      /// @param value the other string
      /// @returns a count of bytes that match exactly at the beginning of the strings
      static size_t longestMatchingPrefix(
        const std::string & previous,
        const std::string & value);

      /// @brief Helper method to find the longest match at the end of two strings
      /// @param previous one of the strings
      /// @param value the other string
      /// @returns a count of bytes that match exactly at the end of the strings
      static size_t longestMatchingSuffix(
        const std::string & previous,
        const std::string & value);


      /// @brief Basic decoding for ByteVectors and Utf8 strings
      ///
      /// Assumes byte vector of approriate length is present in input stream.
      /// @param[in] source supplies the data
      /// @param[out] buffer receives the result
      /// @param[in] length expected
      /// @throws EncodingError if not enough data is available
      static void decodeByteVector(
        Codecs::DataSource & source,
        WorkingBuffer & buffer,
        size_t length);

    private:
      /// @brief Interpret initial or default value attribute.
      /// @param value for this instruction as specified in XML
      virtual void interpretValue(const std::string & value) = 0;

      /// @brief Determine how many presence map bits might be needed.
      ///
      /// Note the actual count is dynamic for Decimal fields so we need a max.
      /// @returns 1 or 2 depending on the field type.
      virtual size_t maxPresenceMapBits() const;

    private:
      FieldInstruction(const FieldInstruction &);
      FieldInstruction & operator = (const FieldInstruction &);

    protected:
      /// Identify information for the fields to be Xcoded by this instruction
//      Messages::FieldIdentityPtr identity_;
      Messages::FieldRegistry & fieldRegistry_;
      Messages::FieldHandle fieldIndex_;

      /// Is this field required in the application message?
      bool mandatory_;
      /// Application type associated with this instruction
      std::string applicationType_;
      /// Namespace in which the application type is defined.
      std::string applicationTypeNamespace_;
      /// Cached applicationTypeNamespace.applicationType
      std::string qualifiedApplicationType_;
      /// Pointer to field operator dispatcher.
      FieldOpPtr fieldOp_;
    };

    ///////////////////////////////
    // Template method definitions

    template<typename IntType>
    void
    FieldInstruction::decodeSignedInteger(Codecs::DataSource & source, Codecs::Context & context, IntType & value, bool oversize)
    {
      PROFILE_POINT("decodeSignedInteger");
      uchar byte;
      if(!source.getByte(byte))
      {
        context.reportFatal("", "Unexpected end of data decoding integer");
      }

      value = 0;
      // extend the sign bit
      if((byte & signBit) != 0)
      {
        value = IntType(-1);
      }
      // Assume an 8 bit byte;
      // Check the seven data bits and the sign bit to make sure no significant
      // information is lost.
      size_t shift = sizeof(IntType) * byteSize - (dataShift + 1);
      IntType overflowMask(IntType(-1) << shift);
      IntType overflowCheck(value << shift);
      // if we're allowing oversize values ignore a sign bit overflow
      if(oversize)
      {
        overflowMask <<= 1;
        overflowCheck <<= 1;
      }

      while((byte & stopBit) == 0)
      {
        if((value & overflowMask) != overflowCheck){
          context.reportError("[ERR D2]", "Integer Field overflow.");
        }
        value <<= dataShift;
        value |= byte;
        if(!source.getByte(byte))
        {
          context.reportFatal("", "Integer Field overflow.");
        }
      }
      // include the last byte (the one with the stop bit)
      if((value & overflowMask) != overflowCheck)
      {
        context.reportError("[ERR D2]", "Integer Field overflow.");
      }
      value <<= dataShift;
      value |= (byte & dataBits);
    }

    template<typename IntType>
    bool
    FieldInstruction::checkNullInteger(IntType & value)
    {
      bool isNull = (value == 0);
      if(value > 0)
      {
        value -= 1;
      }
      return isNull;
    }

    template<typename UnsignedIntType>
    void
    FieldInstruction::decodeUnsignedInteger(Codecs::DataSource & source, Codecs::Context & context, UnsignedIntType & value)
    {
      PROFILE_POINT("decodeUnsignedInteger");
      uchar byte;
      if(!source.getByte(byte))
      {
        context.reportFatal("", "Unexpected end of data decoding unsigned integer");
      }

      value = 0;

      // Assume an 8 bit byte;
      // Check the seven data bitsbit to make sure no significant
      // information is lost.
      unsigned short shift = sizeof(UnsignedIntType) * byteSize - dataShift;
      UnsignedIntType overflowMask(UnsignedIntType(-1) << shift);
      UnsignedIntType overflowCheck(value << shift);

      while((byte & stopBit) == 0)
      {
        if((value & overflowMask) != overflowCheck)
        {
          context.reportError("[ERR D2]", "Unsigned Integer Field overflow.");
        }
        value <<= dataShift;
        value |= byte;
        if(!source.getByte(byte))
        {
          context.reportFatal("", "End of file without stop bit decoding unsigned integer.");
        }
      }
      if((value & overflowMask) != overflowCheck)
      {
        context.reportError("[ERR D2]", "Integer Field overflow.");
      }
      value <<= dataShift;
      value |= (byte & dataBits);
    }
  }
}
#endif // FIELDINSTRUCTION_H
