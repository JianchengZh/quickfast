// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#include "Encoder.h"
#include <Codecs/DataDestination.h>
#include <Messages/Message.h>
#include <Codecs/PresenceMap.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/FieldInstruction.h>

using namespace ::QuickFAST;
using namespace ::QuickFAST::Codecs;

Encoder::Encoder(Codecs::TemplateRegistryPtr registry)
: Context(registry)
{
}

void
Encoder::encodeMessage(
  DataDestination & destination,
  template_id_t templateId,
  const Messages::Message & message)
{
  encodeSegment(destination, templateId, message);
  destination.endMessage();
}

void
Encoder::encodeSegment(
  DataDestination & destination,
  template_id_t templateId,
  const Messages::FieldSet & fieldSet)
{
  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  Codecs::TemplateCPtr templatePtr;
  if(getTemplateRegistry()->getTemplate(getTemplateId(), templatePtr))
  {
    DestinationBufferPtr header = destination.startBuffer();
    destination.startBuffer();
    encodeSegmentBody(destination, pmap, templatePtr, fieldSet);
    destination.selectBuffer(header);
    pmap.encode(destination);
    FieldInstruction::encodeUnsignedInteger(destination, getWorkingBuffer(), getTemplateId());
  }
  else
  {
    throw EncodingError("[ERR D9] Unknown template ID.");
  }
}

void
Encoder::encodeGroup(
  DataDestination & destination,
  Codecs::SegmentBodyCPtr group,
  const Messages::FieldSet & fieldSet)
{
  DestinationBufferPtr previousBuffer = destination.getBuffer();
  Codecs::PresenceMap pmap(getTemplateRegistry()->presenceMapBits());
  DestinationBufferPtr header;
  if(getTemplateRegistry()->presenceMapBits() != 0)
  {
   header = destination.startBuffer();
  }
  // start a buffer for the body of the group
  destination.startBuffer();
  encodeSegmentBody(destination, pmap, group, fieldSet);
  if(header) // if we expected a pmap
  {
    destination.selectBuffer(header);
    pmap.encode(destination);
  }
  destination.selectBuffer(previousBuffer);
}

#if 0
void
Encoder::encodeStaticTemplateRef(
  DataSource & destination,
  Codecs::PresenceMap & pmap,
  const std::string & templateName,
  const std::string & templateNamespace,
  const Messages::FieldSet & fieldSet)
{
  Codecs::TemplateCPtr templatePtr;
  if(!templateRegistry_->findNamedTemplate(
    templateName,
    templateNamespace,
    templatePtr))
  {
    throw EncodingError("[ERR D9] Unknown template name for static templateref.");
  }

  if(!encodeSegmentBody(destination, pmap, templatePtr, fieldSet))
  {
    throw EncodingError("Unexpected end of file during templateref decoding.");
  }
}

#endif

void
Encoder::encodeSegmentBody(
  DataDestination & destination,
  Codecs::PresenceMap & pmap,
  Codecs::SegmentBodyCPtr segment,
  const Messages::FieldSet & fieldSet)
{
  size_t instructionCount = segment->size();
  for( size_t nField = 0; nField < instructionCount; ++nField)
  {
    Codecs::FieldInstructionCPtr instruction;
    if(segment->getInstruction(nField, instruction))
    {
      instruction->encode(destination, pmap, *this, fieldSet);
    }
  }
}