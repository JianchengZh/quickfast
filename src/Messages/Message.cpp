// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "Message.h"
#include <Messages/Field.h>
#include <Common/Exceptions.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Messages;

Message::Message(const FieldRegistry & fieldRegistry, size_t expectedNumberOfFields)
: FieldSet(fieldRegistry, expectedNumberOfFields)
{
  applicationType_ = "";
}

