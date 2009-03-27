// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>
#include <Codecs/FieldInstructionAscii.h>
#include <Codecs/FieldInstructionUtf8.h>
#include <Codecs/FieldInstructionByteVector.h>
#include <Codecs/FieldInstructionInt8.h>
#include <Codecs/FieldInstructionUInt8.h>
#include <Codecs/FieldInstructionInt16.h>
#include <Codecs/FieldInstructionUInt16.h>
#include <Codecs/FieldInstructionInt32.h>
#include <Codecs/FieldInstructionUInt32.h>
#include <Codecs/FieldInstructionInt64.h>
#include <Codecs/FieldInstructionUInt64.h>
#include <Codecs/FieldInstructionDecimal.h>
#include <Codecs/FieldInstructionTemplateRef.h>
#include <Codecs/FieldInstructionGroup.h>
#include <Codecs/FieldInstructionSequence.h>
#include <Codecs/FieldOpConstant.h>
#include <Codecs/FieldOpCopy.h>
#include <Codecs/FieldOpDefault.h>
#include <Codecs/FieldOpDelta.h>
#include <Codecs/FieldOpIncrement.h>
#include <Codecs/FieldOpNop.h>
#include <Codecs/FieldOpTail.h>
#include <Codecs/DataSourceString.h>
#include <Codecs/DataSourceStream.h>
#include <Codecs/DataDestinationString.h>
#include <Codecs/Decoder.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/DictionaryIndexer.h>

using namespace QuickFAST;

BOOST_AUTO_TEST_CASE(theFirstTest)
{
  // with any luck, this will be the first test run
#ifdef _MSC_VER
  _CrtSetBreakAlloc(16586*10); // see documentation of this function for debugging memory leaks.
#endif
  BOOST_CHECK(true); // entertain boost test that objects to tests which don't actually test anything.
}

namespace
{
  void testFieldInstructionBaseClass(
    FieldInstruction & instruction,
    size_t expectPresenceMapBits,
    bool acceptsOperator = true)
  {
    BOOST_CHECK(instruction.isMandatory());
    instruction.setPresence(false);
    BOOST_CHECK(!instruction.isMandatory());

    instruction.setId("id");
    BOOST_CHECK_EQUAL(instruction.getId(), "id");

    const Messages::FieldIdentity & identity = instruction.getIdentity();
    BOOST_CHECK(!instruction.isMandatory());
    BOOST_CHECK_EQUAL(identity.id(),"id");
    BOOST_CHECK_EQUAL(identity.name(), "//NS/Name");

    BOOST_CHECK_EQUAL(instruction.presenceMapBitsRequired(), 0);

    if(acceptsOperator)
    {
      FieldOpPtr fieldOp(new FieldOpNop);
      fieldOp->setKey("alternate");
      fieldOp->setDictionaryName("global");
      instruction.setFieldOp(fieldOp);

      instruction.setFieldOp(FieldOpPtr(new FieldOpConstant));
      BOOST_CHECK_EQUAL(instruction.presenceMapBitsRequired(), expectPresenceMapBits);
      instruction.setFieldOp(FieldOpPtr(new FieldOpCopy));
      BOOST_CHECK_EQUAL(instruction.presenceMapBitsRequired(), expectPresenceMapBits);
      instruction.setFieldOp(FieldOpPtr(new FieldOpDefault));
      BOOST_CHECK_EQUAL(instruction.presenceMapBitsRequired(), expectPresenceMapBits);
      instruction.setFieldOp(FieldOpPtr(new FieldOpDelta));
      BOOST_CHECK_EQUAL(instruction.presenceMapBitsRequired(), 0);
    }
  }
}

BOOST_AUTO_TEST_CASE(testFieldInstructionAscii)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionUtf8)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUtf8 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionByteVector)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionByteVector instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionInt8)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt8 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionUInt8)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt8 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionInt16)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt16 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionUInt16)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt16 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionInt32)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionUInt32)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionInt64)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt64 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionUInt64)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt64 instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionDecimal)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 2);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionTemplateRef)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionTemplateRef instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 0, false);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionGroup)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionGroup instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1, false);
}

BOOST_AUTO_TEST_CASE(testFieldInstructionSequence)
{
  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionSequence instruction(fieldRegistry, "Name", "NS", "", "");
  testFieldInstructionBaseClass(instruction, 1, false);
}


BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_1_1)
{
  // <int32 id="1" presence="optional" name="Value"/>
  // Input Value    942755
  // Native Hex     0x0e      0x62      0xa3
  //        Binary  0000 1110 0110 0010 1010 0011
  // FAST Hex       0x39      0x45      0xa4
  //        Binary  0011 1001 0100 0101 1010 0100

  std::string testString("\x39\x45\xa4");
  std::istringstream sourceStream(testString, std::ios::binary);
  Codecs::DataSourceStream source(sourceStream);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");

  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 942755);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_1_2)
{
//Int32 Example � Mandatory Positive Number
//<int32 id="1" presence="mandatory" name="Value"/>
//Input Value     942755
//Native  Hex     0x0e 0x62 0xa3
//     Binary     0000 1110 0110 0010 1010 0011
//FAST    Hex     0x39 0x45 0xa3
//     Binary     00111001 01000101 10100011
  const std::string testString("\x39\x45\xa3");
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Value", "", "", "");
  // Should be default:  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 942755);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

    // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);

}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_1_3)
{
//Int32 Example � Optional Negative Number
//<int32 id="1" presence="optional" name="Value"/>
//Input Value       -942755
//Native  Hex       0xf1 0x9d 0x5d
//     Binary       1111 0001 1001 1101 0101 1101
//FAST    Hex       0x46 0x3a 0xdd
//     Binary       0100 0110 0011 1010 1101 1101
  std::string testString("\x46\x3a\xdd");
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), -942755);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_1_4)
{
//Int32 Example � Mandatory Negative Number
//<int32 id="1" presence="mandatory" name="Value"/>
//Input Value -7942755
//Native Hex  0xff 0x86 0xcd 0x9d
//Binary      1111 1111 1000 0110 1100 1101 1001 1101
//FAST Hex    0x7c 0x1b 0x1b 0x9d
//Binary      0111 1100 0001 1011 0001 1011 1001 1101
  const std::string testString("\x7c\x1b\x1b\x9d");
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), -7942755);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_1_5)
{
//Int32 Example � Mandatory Positive Number with sign-bit extension
//<int32 id="1" presence="mandatory" name="Value"/>
//Input Value   8193
//Native Hex    0x20 0x01
//Binary        0010 0000 0000 0001
//FAST Hex      0x00 0x40 0x81
//Binary        0000 0000 0100 0000 1000 0001
  const char testData[] = "\x00\x40\x81";  // beware embedded nulls
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 8193);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_1_6)
{
//Int32 Example � Mandatory Negative Number with sign-bit extension
//<int32 id="1" presence="mandatory" name="Value"/>
//Input Value   -8193
//Native Hex    0xff 0xdf 0xff
//Binary        1111 1111 1101 1111 1111 1111
//ERROR in SPEC: FAST Hex      0x73 0x3f 0xff
//FAST Hex      0x7F 0x3f 0xff
//Binary        0111 1111 0011 1111 1111 1111

  const char testData[] = "\x7F\x3f\xff";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), -8193);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}


BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_1a)
{
//uInt32 Example � Optional Number
//<uInt32 id="1" presence="optional" name="Value"/>

//Input Value   NULL
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x80
//Binary        1000 0000

  const char testData[] = "\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate nothing (Missing optional field)
  BOOST_CHECK_EQUAL(fieldSet.size(), 0);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_1b)
{
//uInt32 Example � Optional Number
//<uInt32 id="1" presence="optional" name="Value"/>

//Input Value   0
//Native Hex    0x00
//Binary        0000 0000
//FAST Hex      0x81
//Binary        1000 0001

  const char testData[] = "\x81";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_1c)
{
//uInt32 Example � Optional Number
//<uInt32 id="1" presence="optional" name="Value"/>

//Input Value   1
//Native Hex    0x01
//Binary        0000 0001
//FAST Hex      0x82
//Binary        1000 0010

  const char testData[] = "\x82";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_1d)
{
//uInt32 Example � Optional Number
//<uInt32 id="1" presence="optional" name="Value"/>

//Input Value   942755
//Native Hex    0x0e 0x62 0xa3
//Binary        1110 01100010 10100011
//FAST Hex      0x39 0x45 0xa4
//Binary        00111001 01000101 10100100

  const char testData[] = "\x39\x45\xa4";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

    Messages::FieldRegistry fieldRegistry;
Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942755);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_2a)
{
//uInt32 Example � Mandatory Number
//<uInt32 id="1" presence="mandatory" name="Value"/>

//Input Value   0
//Native Hex    0x00
//Binary        0000 0000
//FAST Hex      0x80
//Binary        1000 0000

  const char testData[] = "\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_2b)
{
//uInt32 Example � Mandatory Number
//<uInt32 id="1" presence="mandatory" name="Value"/>

//Input Value   1
//Native Hex    0x01
//Binary        0000 0001
//FAST Hex      0x81
//Binary        1000 0001

  const char testData[] = "\x81";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_2_2c)
{
//uInt32 Example � Optional Number
//<uInt32 id="1" presence="optional" name="Value"/>

//Input Value   942755
//Native Hex    0x0e 0x62 0xa3
//Binary        1110 01100010 10100011
//FAST Hex      0x39 0x45 0xa3
//Binary        00111001 01000101 10100011

  const char testData[] = "\x39\x45\xa3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942755);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_3_1a)
{
// US ASCII string Example - Optional String
// <string id="1" presence="optional" name="Value"/>

//Input Value   NULL
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x80
//Binary        1000 0000

  const char testData[] = "\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 0);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_3_1b)
{
// US ASCII string Example - Optional String
// <string id="1" presence="optional" name="Value"/>

//Input Value   "ABC"
//Native Hex    0x41 0x42 0x43
//Binary        01000001 01000010 01000011
//FAST Hex      0x41 0x42 0xC3
//Binary        01000001 01000002 11000011

  const char testData[] = "AB\xC3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ABC");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_3_1c)
{
// US ASCII string Example - Optional String
// <string id="1" presence="optional" name="Value"/>

//Input Value   ""
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x00 0x80
//Binary        0000 0000 1000 0000

  const char testData[] = "\x00\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Value", "", "", "");
  field.setId("1");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK(pFieldEntry->getField()->toAscii().empty());
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);

}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_3_2a)
{
// US ASCII string Example - Mandatory String
// <string id="1" presence="mandatory" name="Value"/>

//Input Value   "ABC"
//Native Hex    0x41 0x42 0x43
//Binary        0100 0001 0100 0010 0100 0011
//FAST Hex      0x41 0x42 0xC3
//Binary        0100 0001 0100 0002 1100 0011

  const char testData[] = "AB\xC3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ABC");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_3_2b)
{
// US ASCII string Example - Optional String
// <string id="1" presence="optional" name="Value"/>

//Input Value   ""
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x80
//Binary        1000 0000

  const char testData[] = "\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK(pFieldEntry->getField()->toAscii().empty());
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_4_1a)
{
// byteVector Example - Optional byteVector
// <byteVector id="1" presence="optional" name="Value"/>

//Input Value   NULL
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x80
//Binary        1000 0000

  const char testData[] = "\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionByteVector field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate no fields
  BOOST_CHECK_EQUAL(fieldSet.size(), 0);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_4_1b)
{
// byteVector string Example - Optional ByteVector
// <byteVector id="1" presence="optional" name="Value"/>

//Input Value   "ABC"
//Native Hex    0x41 0x42 0x43
//Binary        0100 0001 0100 0010 0100 0011
//FAST Hex      0x84 0x41 0x42 0x43
//Binary        1000 0100 0100 0001 0100 0002 0100 0011

  const char testData[] = "\x84\x41\x42\x43";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionByteVector field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toByteVector(), "ABC");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_4_1c)
{
// byteVector string Example - Optional ByteVector
// <byteVector id="1" presence="optional" name="Value"/>

//Input Value   ""
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x81
//Binary        1000 0001

  const char testData[] = "\x81";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionByteVector field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(pFieldEntry->getField()->toByteVector().empty());
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_4_2a)
{
// byteVector string Example - Mandatory ByteVector
// <byteVector id="1" presence="mandatory" name="Value"/>

//Input Value   "ABC"
//Native Hex    0x41 0x42 0x43
//Binary        0100 0001 0100 0010 0100 0011
//FAST Hex      0x83 0x41 0x42 0x43
//Binary        0100 0001 0100 0002 1100 0011

  const char testData[] = "\x83\x41\x42\x43";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionByteVector field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toByteVector(), "ABC");
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());
  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);


}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_4_2b)
{
// byteVector string Example - Mandatory ByteVector
// <byteVector id="1" presence="mandatory" name="Value"/>

//Input Value   ""
//Native Hex    n/a
//Binary        n/a
//FAST Hex      0x80
//Binary        1000 0000

  const char testData[] = "\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionByteVector field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");
  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::BYTEVECTOR));
  BOOST_CHECK(pFieldEntry->getField()->toByteVector().empty());
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_1)
{
//Decimal Example � Mandatory Positive Decimal
//<decimal id="1" presence="mandatory" name="Value"/>
//Input Value  94275500
//Decomposed Input Value 942755 * 10 ^ 2
//        Exponent    Mantissa
//        2           942755
//FAST
// Hex    0x82        0x39 0x45 0xa3
// Binary 1000 0010   0011 1001 0100 0101 1010 0011
  const char testData[] = "\x82\x39\x45\xa3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(942755, 2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_2)
{
//Decimal Example � Mandatory Positive Decimal with Scaled Mantissa
//<decimal id="1" presence="mandatory" name="Value"/>
//Input Value  94275500
//Decomposed Input Value 942755 * 10 ^ 2
//        Exponent    Mantissa
//        1           9427550
//FAST
// Hex    0x81        0x04 0x3f 0x34 0xDE
// Binary 1000 0001   00000100 00111111 00110100 11011110
  const char testData[] = "\x81\x04\x3F\x34\xDE";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(942755, 2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_3)
{
//Decimal Example � Optional Positive Decimal
//<decimal id="1" presence="optional" name="Value"/>
//Input Value  94275500
//Decomposed Input Value 942755 * 10 ^ 2
//        Exponent    Mantissa
//        2           942755
//FAST
// Hex    0x83        0x39 0x45 0xa3
// Binary 1000 0010   0011 1001 0100 0101 1010 0011
  const char testData[] = "\x83\x39\x45\xa3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.buildIndexes(indexer, "global","", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(942755, 2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_4)
{
//Decimal Example � Mandatory Positive Decimal negative exponent
//<decimal id="1" presence="mandatory" name="Value"/>
//Input Value  9427.55
//Decomposed Input Value 942755 * 10 ^ 2
//        Exponent    Mantissa
//        -2          942755
//FAST
// Hex    0xFE        0x39 0x45 0xa3
// Binary 1111 1110   0011 1001 0100 0101 1010 0011
  const char testData[] = "\xFE\x39\x45\xa3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(942755, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_5)
{
//Decimal Example � Optional Negative Decimal negative exponent
//<decimal id="1" presence="optional" name="Value"/>
//Input Value  -9427.55
//Decomposed Input Value 942755 * 10 ^ 2
//        Exponent    Mantissa
//        -2          -942755
//FAST
// Hex    0xFE        0x46 0x3a 0xdd
// Binary 1111 1110   01000110 00111010 11011101
  const char testData[] = "\xFE\x46\x3A\xDD";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(true);
  field.buildIndexes(indexer, "global","", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(-942755, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_6)
{
//Decimal Example � Optional Positive Decimal with single field operator
//<decimal id="1" presence="optional" name="Value"> <copy/> </decimal>
//Input Value  9427.55
//Decomposed Input Value 942755 * 10 ^ 2
//        Exponent    Mantissa
//        -2          942755
//FAST
// Hex    0xFE        0x39 0x45 0xA3
// Binary 1111 1110   00111001 01000101 10100011
  const char testData[] = "\xFE\x39\x45\xA3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);
  pmap.setNextField(true);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);
  field.setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.buildIndexes(indexer, "global","", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(942755, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_7)
{
//Decimal Example � Optional Positive Decimal with individual field operators
// <decimal id="1" presence="optional" name="Value">
//   <exponent> <copy/> </exponent>
//   <mantissa> <delta/> </mantissa>
// </decimal>
// Input   Exp:Man   Pmap  FAST Exp:Man Hex/Binary
// 9427.55 -2:942755 1     FE:39 45 A3 / 11111110 00111001 01000101 10100011
  const char testData[] = "\xFE\x39\x45\xA3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);
  pmap.setNextField(true);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);

  Codecs::FieldInstructionExponentPtr exponent(new FieldInstructionExponent(
    fieldRegistry, "value|exponent", "", "", ""));
  exponent->setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.setExponentInstruction(exponent);

  Codecs::FieldInstructionMantissaPtr mantissa(new FieldInstructionMantissa(
    fieldRegistry, "value|mantissa", "", "", ""));
  mantissa->setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpDelta));
  field.setMantissaInstruction(mantissa);
  field.buildIndexes(indexer, "global", "", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(942755, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Spec_1x1_Appendix3_1_5_8)
{
    //Decimal Example � Optional Negative Decimal with sign bit extension
    // <decimal id="1" presence="optional" name="Value"/>
    // Input   Exp:Man Pmap  FAST Exp:Man Hex/Binary
    // -8.193  -3:8193 1     FD:7F 3F FF / 0111111 00111111 11111111
    //    ERROR IN SPEC: FD:73 3F FF is wrong!
  const char testData[] = "\xFD\x7F\x3F\xFF";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  BOOST_CHECK_EQUAL(fieldSet.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet.begin();
  BOOST_CHECK(pFieldEntry != fieldSet.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expected(-8193, -3);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expected);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testString);
  BOOST_CHECK(pmap == pmapResult);
}
