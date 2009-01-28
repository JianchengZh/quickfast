// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef MESSAGECONSUMER_H
#define MESSAGECONSUMER_H
#include "MessageConsumer_fwd.h"
#include <Common/QuickFAST_Export.h>
#include <Messages/Message_fwd.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief interface to be implemented by a consumer of decoded messages.
    class QuickFAST_Export MessageConsumer
    {
    public:
      virtual ~MessageConsumer(){}
      /// @brief Accept a decoded message
      /// @param message is the decoded message, valid for the life of this call.
      /// @returns true if decoding should continue; false to stop decoding
      virtual bool consumeMessage(Messages::Message & message) = 0;
    };
  }
}
#endif /* MESSAGECONSUMER_H */