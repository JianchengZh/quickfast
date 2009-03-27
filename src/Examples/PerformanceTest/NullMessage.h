// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef NULLMESSAGE_H
#define NULLMESSAGE_H
#include <Messages/DecodedFields.h>
#include <Messages/FieldRegistry.h>
#include <Common/Types.h>
namespace QuickFAST{
  namespace Examples{
    /// @brief Internal representation of a Null Message
    /// @todo: consider typedef FieldSet Message
    class NullMessage : public Messages::DecodedFields
    {
      NullMessage();
    public:
      /// @brief Construct an empty NullMessage
      NullMessage(size_t expectedNumberOfFields);

      /// @brief Copy construct NullMessage
      NullMessage(const NullMessage & rhs);

      virtual void clear(size_t capacity = 0);
      virtual void reserve(size_t capacity);
      virtual size_t size()const;
      virtual void addField(const Messages::FieldIdentityCPtr & identity, const Messages::FieldCPtr & value);
      virtual void addField(
        const Messages::FieldRegistry & registry,
        Messages::FieldRegistry::Index index,
        const Messages::FieldCPtr & value);

      virtual bool getIdentity(const std::string &name, Messages::FieldIdentityCPtr & identity) const;
      virtual bool getIdentity(
        const std::string &name,
        const Messages::FieldIdentity *& identity) const;
      virtual void setApplicationType(const std::string & type, const std::string & ns);
      virtual const std::string & getApplicationType()const;
      virtual const std::string & getApplicationTypeNs()const;
      virtual Messages::DecodedFields * createdNestedFields(size_t size)const;
    private:
      int size_;
      std::string applicationType_;
      std::string applicationTypeNamespace_;

    };
  }
}
#include <Codecs/MessageConsumer.h>
namespace QuickFAST{
  namespace Examples{
    class NullMessageConsumer : public Codecs::MessageConsumer
    {
    public:
      NullMessageConsumer();
      virtual ~NullMessageConsumer();
      virtual bool consumeMessage(Examples::NullMessage & message);
      virtual bool consumeMessage(Messages::Message & message);
      virtual bool wantLog(unsigned short level);
      virtual bool logMessage(unsigned short level, const std::string & logMessage);
      virtual bool reportDecodingError(const std::string & errorMessage);
      virtual bool reportCommunicationError(const std::string & errorMessage);
      virtual void decodingStopped();

      /// @brief get the count
      /// @returns the number of calls to consumeMessage()
      size_t getMesssageCount()const
      {
        return messageCount_;
      }
    private:
      size_t messageCount_;
    };
  }
}
#endif // NULLMESSAGE_H
