// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionByteVector.h"
#include <Messages/FieldByteVector.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionByteVector::FieldInstructionByteVector(
        Messages::FieldRegistry & fieldRegistry,
        const std::string & name,
        const std::string & fieldNamespace,
        const std::string & type,
        const std::string & typeNamespace)
  : FieldInstructionBlob(fieldRegistry, name, fieldNamespace, type, typeNamespace)
{
  initialValue_ = Messages::FieldByteVector::createNull();
}

#if 0
FieldInstructionByteVector::FieldInstructionByteVector()
{
  initialValue_ = Messages::FieldByteVector::createNull();
}
#endif

FieldInstructionByteVector::~FieldInstructionByteVector()
{
}


Messages::FieldCPtr
FieldInstructionByteVector::createField(const uchar * buffer, size_t length)const
{
  return Messages::FieldCPtr(Messages::FieldByteVector::create(buffer, length));
}

Messages::FieldCPtr
FieldInstructionByteVector::createField(const std::string value)const
{
  return Messages::FieldCPtr(Messages::FieldByteVector::create(value));
}

Messages::FieldCPtr
FieldInstructionByteVector::createEmptyField()const
{
  return Messages::FieldCPtr(Messages::FieldByteVector::createNull());
}

