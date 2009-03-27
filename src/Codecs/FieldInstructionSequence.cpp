// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>
#include "FieldInstructionSequence.h"
#include <Codecs/DataSource.h>
#include <Codecs/Decoder.h>
#include <Codecs/Encoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstructionUInt32.h>
#include <Messages/Message.h>
#include <Messages/FieldSequence.h>
#include <Messages/Sequence.h>
#include <Messages/FieldUInt32.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

FieldInstructionSequence::FieldInstructionSequence(
        Messages::FieldRegistry & fieldRegistry,
        const std::string & name,
        const std::string & fieldNamespace,
        const std::string & type,
        const std::string & typeNamespace)
  : FieldInstruction(fieldRegistry, name, fieldNamespace, type, typeNamespace)
{
}

FieldInstructionSequence::~FieldInstructionSequence()
{
}

bool
FieldInstructionSequence::decodeNop(
  Codecs::DataSource & source,
  Codecs::PresenceMap & pmap,
  Codecs::Decoder & decoder,
  Messages::DecodedFields & fieldSet) const
{
  if(!segment_)
  {
    decoder.reportFatal("[ERR U07]", "SegmentBody not defined for Sequence instruction.");
  }
  size_t length = 0;
  Codecs::FieldInstructionCPtr lengthInstruction;
  Messages::FieldSet lengthSet(decoder.getTemplateRegistry()->fieldRegistry(), 1);
  if(segment_->getLengthInstruction(lengthInstruction))
  {
    source.beginField(lengthInstruction->getIdentity().name());
    if(!lengthInstruction->decode(source, pmap, decoder, lengthSet))
    {
      return false;
    }
  }
  else
  {
    FieldInstructionUInt32 defaultLengthInstruction(fieldRegistry_,"length","", "", "");
    defaultLengthInstruction.setPresence(isMandatory());
    if(!defaultLengthInstruction.decode(source, pmap, decoder, lengthSet))
    {
      return false;
    }
  }

  Messages::FieldSet::const_iterator fld = lengthSet.begin();
  if(fld == lengthSet.end())
  {
    // this optional sequence is not present
    return true;
  }
  length = fld->getField()->toUInt32();

  Messages::SequencePtr sequence(new Messages::Sequence);
  for(size_t nEntry = 0; nEntry < length; ++nEntry)
  {
    if(decoder.getLogOut())
    {
      std::stringstream msg;
      msg << "Sequence entry #" << nEntry << " of " << length << std::ends;
      decoder.logMessage(msg.str());
    }

    Messages::FieldSetPtr entrySet(
      new Messages::FieldSet(
        decoder.getTemplateRegistry()->fieldRegistry(),
        segment_->fieldCount()));
    entrySet->setApplicationType(segment_->getApplicationType(), segment_->getApplicationTypeNamespace());
    decoder.decodeGroup(source, segment_, *entrySet);
    sequence->addEntry(entrySet);
  }
  Messages::FieldCPtr field(Messages::FieldSequence::create(sequence));
  fieldSet.addField(
    fieldRegistry_,
    fieldIndex_,
    field);
  return true;
}

void
FieldInstructionSequence::encodeNop(
  Codecs::DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::Encoder & encoder,
  const Messages::FieldSet & fieldSet) const
{
  if(!segment_)
  {
    encoder.reportFatal("[ERR U07]", "SegmentBody not defined for Sequence instruction.");
  }

  // retrieve the field corresponding to this sequence
  Messages::FieldCPtr field;
      if(fieldSet.getField(fieldRegistry_, fieldIndex_, field))
  {
    Messages::SequenceCPtr sequence = field->toSequence();
    size_t length = sequence->size();

    // todo: performance could be improved here
    Messages::FieldCPtr lengthField(Messages::FieldUInt32::create(length));
    Messages::FieldSet lengthSet(
      encoder.getTemplateRegistry()->fieldRegistry(), 1);

    Codecs::FieldInstructionCPtr lengthInstruction;
    if(segment_->getLengthInstruction(lengthInstruction))
    {
      lengthSet.addField(
        fieldRegistry_,
        lengthInstruction->getFieldIndex(),
        lengthField);
      lengthInstruction->encode(destination, pmap, encoder, lengthSet);
    }
    else
    {
      Messages::FieldRegistry registry;
      FieldInstructionUInt32 lengthInstruction(registry, "length","","","");
      lengthInstruction.setPresence(isMandatory());
      lengthInstruction.encode(destination, pmap, encoder, lengthSet);
    }

    for(size_t pos = 0; pos < length; ++pos)
    {
      const Messages::FieldSetCPtr & entry = (*sequence)[pos];
      encoder.encodeGroup(destination, segment_, *entry);
    }
  }
  else
  {
    // possibility #2: option group not present.
    if(isMandatory())
    {
      encoder.reportFatal("[ERR U09]", "Missing mandatory group.");
    }
    // let our counterparty know it's just not there.
    pmap.setNextField(false);
  }
}

void
FieldInstructionSequence::interpretValue(const std::string & value)
{
  throw TemplateDefinitionError("Value not needed by Sequence instruction.");
}

void
FieldInstructionSequence::buildIndexes(
  DictionaryIndexer & indexer,
  const std::string & dictionaryName,
  const std::string & typeName,
  const std::string & typeNamespace)
{
  segment_->buildIndexes(indexer, dictionaryName, typeName, typeNamespace);
}

