// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
//

#include "Examples/ExamplesPch.h"
#include "MessageInterpreter.h"
#include <Messages/Message.h>
#include <Messages/Field.h>
#include <Messages/Sequence.h>
#include <Messages/Group.h>

using namespace QuickFAST;
using namespace Examples;

MessageInterpreter::MessageInterpreter(std::ostream & out)
  : out_(out)
  , indent_(0)
  , recordCount_(0)
{
}

MessageInterpreter::~MessageInterpreter()
{
}

bool
MessageInterpreter::consumeMessage(Messages::Message & message)
{
  recordCount_ += 1;
  out_ << "Record #" << recordCount_ << ' ' << std::flush;
  formatMessage(message);
  out_ << std::endl;
  return true;
}

void
MessageInterpreter::formatMessage(Messages::Message & message)
{
  for( Messages::Message::const_iterator it = message.begin();
    it != message.end();
    ++it)
  {
    Messages::FieldIdentityCPtr identity = it->getIdentity();
    Messages::FieldCPtr field = it->getField();
    Messages::Field::FieldType type = field->getType();
    if(type == Messages::Field::SEQUENCE)
    {
      formatSequence(identity, field);
    }
    else if(type == Messages::Field::GROUP)
    {
      formatGroup(field);
    }
    else
    {
      out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
      displayFieldValue(field);
    }
  }
}
void
MessageInterpreter::newline()
{
  out_ << std::endl << std::string(indent_ * 2, ' ');
}

void
MessageInterpreter::formatSequence(
  Messages::FieldIdentityCPtr & identity,
  Messages::FieldCPtr & field)
{
  Messages::SequenceCPtr sequence = field->toSequence();
  size_t count = sequence->size();
  newline();
  out_ << ' ' << identity->name() << '[' << identity->id() << "]=";

  out_ << "Sequence[" << count << ']';
  size_t entryCount = 0;
  ++indent_;
  for(Messages::Sequence::const_iterator it = sequence->begin();
    it != sequence->end();
    ++it)
  {
    newline();
    out_ << '[' << entryCount++ << "]: ";

    Messages::FieldSetCPtr fieldSet = *it;
    for(Messages::FieldSet::const_iterator fsit = fieldSet->begin();
      fsit != fieldSet->end();
      ++fsit)
    {
      // todo: refactor with message decoding
      Messages::FieldIdentityCPtr identity = fsit->getIdentity();
      Messages::FieldCPtr field = fsit->getField();
      Messages::Field::FieldType type = field->getType();
      if(type == Messages::Field::SEQUENCE)
      {
        formatSequence(identity, field);
      }
      else if(type == Messages::Field::GROUP)
      {
        formatGroup(field);
      }
      else
      {
        out_ << ' ' << identity->name() << '[' << identity->id() << "]=";
        displayFieldValue(field);
      }
    }
  }
  --indent_;
  newline();
}

void
MessageInterpreter::formatGroup(Messages::FieldCPtr & field)
{
}


void
MessageInterpreter::displayFieldValue(Messages::FieldCPtr field)
{
  switch(field->getType())
  {
  case Messages::Field::INT32:
    out_ << field->toInt32();
    break;
  case Messages::Field::UINT32:
    out_ << field->toUInt32();
    break;
  case Messages::Field::INT64:
    out_ << field->toInt64();
    break;
  case Messages::Field::UINT64:
    out_ << field->toUInt64();
    break;
  case Messages::Field::DECIMAL:
    {
      std::string value;
      field->toDecimal().toString(value);
      out_ << value;
      break;
    }
  case Messages::Field::ASCII:
    {
      out_ << field->toAscii();
      break;
    }
  case Messages::Field::UTF8:
    {
      out_ << field->toUtf8();
      break;
    }
  case Messages::Field::BYTEVECTOR:
    {
      // todo: we probably should hex dump this
      out_ << field->toByteVector();
      break;
    }
  case Messages::Field::SEQUENCE:
    {
      out_ << "sequence";
      break;
    }
  case Messages::Field::GROUP:
    {
      static const std::string sts("group");
      out_ << "group";
      break;
    }
  default:
    {
      out_ << "unknown field type";
      break;
    }
  }

}

