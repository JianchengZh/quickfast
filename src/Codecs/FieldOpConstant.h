// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#ifdef _MSC_VER
# pragma once
#endif
#ifndef FIELDOPCONSTANT_H
#define FIELDOPCONSTANT_H
#include <Codecs/FieldOp.h>
namespace QuickFAST{
  namespace Codecs{
    /// @brief Dispatch the &lt;constant> operation to the appropriate method in a FieldInstruction.
    class QuickFAST_Export FieldOpConstant
      : public FieldOp
    {
    public:
      virtual bool usesPresenceMap(bool mandatory)const;
      virtual bool usesDictionary() const;

      virtual void decode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataSource & source,
        Codecs::PresenceMap & pmap,
        Codecs::Decoder & decoder,
        Messages::ValueMessageBuilder & fieldSet) const;

      virtual void encode(
        const Codecs::FieldInstruction & instruction,
        Codecs::DataDestination & destination,
        Codecs::PresenceMap & pmap,
        Codecs::Encoder & encoder,
        const Messages::MessageAccessor & fieldSet) const;

      virtual void setDefaultValue(
        Codecs::FieldInstruction & instruction) const;
    };
  }
}
#endif // FIELDOPCONSTANT_H
