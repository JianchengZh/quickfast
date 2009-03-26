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
#include <Codecs/DictionaryIndexer.h>
#include <Codecs/TemplateRegistry.h>
#include <Tests/FieldInstructionMock.h>
#include <Tests/DataDestinationMock.h>

using namespace QuickFAST;

BOOST_AUTO_TEST_CASE(testFieldOperationDispatch)
{
  // the Field Operator does half of a double dispatch
  // The tests below make sure the dispatch gets to the
  // right place.

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create an "empty" presence map.
  Codecs::PresenceMap pmap(1);

  Tests::FieldInstructionMock field;
  field.setPresence(false);
  // NOP operator is the default
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // create enough infrastructure to call decode
  Codecs::DataSourceString source("");
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);
  Messages::FieldSet fieldSet1(10);

  // and encode
  Tests::DataDestinationMock destination;
  Codecs::Encoder encoder(registry);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeNop(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeNop(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr constantOp(new Codecs::FieldOpConstant);
  constantOp->setValue("1");
  field.setFieldOp(constantOp);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeConstant(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeConstant(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr defaultOp(new Codecs::FieldOpDefault);
  defaultOp->setValue("1");
  field.setFieldOp(defaultOp);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeDefault(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeDefault(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr copyOp(new Codecs::FieldOpCopy);
  copyOp->setValue("1");
  field.setFieldOp(copyOp);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeCopy(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeCopy(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr deltaOp(new Codecs::FieldOpDelta);
  deltaOp->setValue("1");
  field.setFieldOp(deltaOp);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeDelta(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeDelta(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr incrementOp(new Codecs::FieldOpIncrement);
  incrementOp->setValue("1");
  field.setFieldOp(incrementOp);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeIncrement(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeIncrement(), 1);
  BOOST_CHECK(field.isClear());

  Codecs::FieldOpPtr tailOp(new Codecs::FieldOpTail);
  tailOp->setValue("1");
  field.setFieldOp(tailOp);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  BOOST_CHECK_EQUAL(field.readDecodeTail(), 1);
  BOOST_CHECK_EQUAL(field.readInterpretValue(), 1);
  BOOST_CHECK(field.isClear());

  field.encode(destination, pmap, encoder, fieldSet1);
  BOOST_CHECK_EQUAL(field.readEncodeTail(), 1);
  BOOST_CHECK(field.isClear());

}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_1_1)
{
// Constant Operator Example  Mandatory Unsigned Integer
// <uInt32 id="1" presence="mandatory" name="Flag"> <constant value="0"/></uInt32>
// Input   Prior   Encoded   Pmap      FAST Hex/Binary
// 0       N/A     None      No        None
// 99      N/A     Error     Error     Error
// None    N/A     Error     Error     Error

  const char testData[] = "";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  DictionaryIndexer indexer;
  Messages::FieldRegistry fieldRegistry;
  Codecs::PresenceMap pmap(1);
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(true);
  FieldOpPtr fieldOp(new Codecs::FieldOpConstant);
  fieldOp->setValue("0");
  field.setFieldOp(fieldOp);
  field.buildIndexes(indexer, "global", "", "");

  // verify no presence map needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 1 field
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  // the value should be the constant zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // Now reencode the data
  Codecs::PresenceMap pmapResult(1);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);

  // check the error condition: constant value doesn't match
  Messages::FieldSet fieldSet2(10);
  Messages::FieldCPtr dataField = Messages::FieldUInt32::create(99);
  fieldSet2.addField(field.getIdentity(), dataField);
  BOOST_CHECK_THROW(field.encode(destination, pmapResult, encoder, fieldSet2), EncodingError);
  // check the error condition: missing mandatory field
  Messages::FieldSet fieldSet3(10);
  BOOST_CHECK_THROW(field.encode(destination, pmapResult, encoder, fieldSet3), EncodingError);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_1_2)
{
//Constant Operator Example  Optional Unsigned Integer
// <uInt32 id="1" presence="optional" name="Flag"> <constant value="0"/> </uInt32>
// Input  Prior Encoded Pmap FAST Hex/Binary
// 0      N/A   None    1    None
// None   N/A   None    0    None
  const char testData[] = "";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(2);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(false);
  FieldOpPtr fieldOp(new Codecs::FieldOpConstant);
  fieldOp->setValue("0");
  field.setFieldOp(fieldOp);
  field.buildIndexes(indexer, "global", "", "");
  // verify 1 presence map bit needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);

  // test a: pmap = 1
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));

  // should generate 1 field
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  // the value should be the constant zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: pmap = 0
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));

  // Check to be sure no data was generated
  BOOST_CHECK(fieldSet2.begin() == fieldSet2.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_2_1)
{
  // Default Operator Example  Mandatory Unsigned Integer
  // <uInt32 id="1" presence="mandatory" name="Flag"> <default value="0"/> </uInt32>// Input  Prior Encoded Pmap FAST Hex/Binary
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // 0      N/A   None    0     None
  // 1      N/A   1       1     0x81: 1000 0001
  const char testData[] = "\x81";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(2);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(true);
  FieldOpPtr fieldOp(new Codecs::FieldOpDefault);
  fieldOp->setValue("0");
  field.setFieldOp(fieldOp);
  field.buildIndexes(indexer, "global", "", "");
  // verify 1 presence map bit needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // test a: pmap = 0
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));

  // should generate 1 field
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  // the value should be the constant zero
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 0);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // test b: pmap = 1
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));

  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry != fieldSet2.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::INT32));
  // the value should be the explicit 1
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toInt32(), 1);
  ++pFieldEntry;
  BOOST_CHECK(pFieldEntry == fieldSet2.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_2_2)
{
  // Default Operator Example for NULL Optional Unsigned Integer
  // <uInt32 id="1" presence="optional" name="Flag"> <default/> </uInt32>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // none   N/A   None    0     None
  const char testData[] = "";
  Codecs::DataSourceString source(testData);
  // create a dictionary indexer
  DictionaryIndexer indexer;
  Codecs::PresenceMap pmap(1);
  pmap.setNextField(false);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionInt32 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(false);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpDefault));
  field.buildIndexes(indexer, "global", "", "");
  // verify 1 presence map bit needed
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);

  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry == fieldSet1.end());

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_3_1)
{
  // Copy Operator Example  Mandatory String
  // <string id="1" presence="mandatory" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // CME    None  CME     1     0x43 0x4d 0xc5 /01000011 01001101 11000101
  // CME    CME   None    0     none
  // ISE    CME   ISE     1     0x49 0x53 0xC5 /01001001 01010011 11000101


  const char testData[] = "\x43\x4d\xc5\x49\x53\xC5";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpCopy));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "CME");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry != fieldSet2.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "CME");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry != fieldSet3.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ISE");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_3_2)
{
  // Copy Operator Example for NULL  Optional String
  // <string id="1" presence="optional" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // None   None  Null    1     0x80 / 10000000
  // None   Null  None    0     None
  // CME    Null  CME     1     0x43 0x4d 0xc5 / 01000011 01001101 11000101

  const char testData[] = "\x80\x43\x4d\xc5";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(false);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpCopy));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet2.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "CME");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_4_1)
{
  // Increment Operator Example  Mandatory Unsigned Integer
  // <uInt32 id="1" presence="mandatory" name="Flag"> <increment value="1"/> </uInt32>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // 1      1     None    0     None
  // 2      1     None    0     None
  // 4      2     4       1     0x84 / 10000010
  // 5      4     None    0     None

  const char testData[] = "\x84";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(4);
  pmap.setNextField(false);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(true);
  FieldOpPtr fieldOp(new Codecs::FieldOpIncrement);
  fieldOp->setValue("1");
  field.setFieldOp(fieldOp);
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));
  Messages::FieldSet fieldSet4(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet4));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // should generate 4 fields; one in each fieldset
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 1);

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 2);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 4);

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 5);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_1)
{
  // Delta Operator Example  Mandatory Signed Integer
  // <int32 id="1" presence="mandatory" name="Price"> <delta/> </int32>
  // Input  Prior   Encoded PMap  FAST Hex/Binary
  // 942755 0       942755  n/a   0x39 0x45 0xa3 /00111001 01000101 10100011
  // 942750 942755  -5      n/a   0xfb / 11111011
  // 942745 942750  -5      n/a   0xfb / 11111011
  // 942745 942745  0       n/a   0x80 / 10000000

  const char testData[] = "\x39\x45\xa3\xfb\xfb\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionUInt32 field(fieldRegistry, "Price", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpDelta));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));
  Messages::FieldSet fieldSet4(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet4));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942755);

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942750);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942745);

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UINT32));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUInt32(), 942745);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_2)
{
  // Delta Operator Example  Mandatory Decimal
  // <decimal id="1" presence="mandatory" name="Price"> <delta/> </decimal>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //          Exp:Man   Exp:Man     Exp:Man
  // 9427.55  0:0       -2:942755   FE:39 45 A3/
  //                                11111110:00111001 01000101 10100011
  // 9427.51  -2:942755 0:-4        0x80:FC/10000000 11111100
  // 9427.46  -2:942751 0:-5        0x80:FB/10000000 11111011

  const char testData[] = "\xFE\x39\x45\xA3\x80\xFC\x80\xFB";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Price", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpDelta));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  Decimal expectA(942755,-2);
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectA);


  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  Decimal expectB(942751,-2);
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectB);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  Decimal expectC(942746,-2);
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectC);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_3)
{
  // Delta Operator Example  Mandatory Decimal with Initial Value
  // <decimal id="1" presence="mandatory" name="Price"> <delta value="12000"/> </decimal>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //          Exp:Man   Exp:Man     Exp:Man
  // 12100    3:12      -2:1198     FE:09 AE/11111110:00001001 10101110
  // 12150    1:1210    0:5         80:85/10000000:10000101
  // 12200    1:1215    0:5         80:85/10000000:10000101

  const char testData[] = "\xFE\x09\xAE\x80\x85\x80\x85";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Price", "", "", "");
  field.setPresence(true);
  FieldOpPtr fieldOp(new Codecs::FieldOpDelta);
  fieldOp->setValue("12000");
  field.setFieldOp(fieldOp);
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));

  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));

  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  Decimal expectB(12100);
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectB);


  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  Decimal expectC(12150);
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectC);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  Decimal expectD(12200);
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectD);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAppendix_3_2_5_4)
{
  // Delta Operator Example  Mandatory String
  // <string id="1" presence="mandatory" name="Security"> <delta/> </string>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //                    Len:Value   Len/Value
  // GEH6     Empty     0:GEH6      80:47 45 48 B6
  //                                1000000:01000111 01000101 01001000 10110110
  // GEM6     GEH6      2:M6        82:4D B6 / 10000010 01001101 10110110
  // ESM6     GEM6      -2:ES       FD:45 D3 / 11111101 01000101 11010011
  // RSESM6   ESM6      -0:RS       FF:52 D3 / 11111111 01010010 11010011

  const char testData[] = "\x80\x47\x45\x48\xb6\x82\x4D\xB6\xFD\x45\xD3\xFF\x52\xD3";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Security", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpDelta));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));
  Messages::FieldSet fieldSet4(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet4));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "ESM6");

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "RSESM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testAsciiTailMandatory)
{
  //  Not in spec
  // Tail Operator Example  Mandatory String
  // <string id="1" presence="mandatory" name="Security"> <tail/> </string>
  // Input    Prior   PMAP  Encoded     FAST Hex/Binary
  // GEH6     Empty   1     GEH6        47 45 48 B6
  //                                    01000111 01000101 01001000 10110110
  // GEM6     GEH6    1     M6          4D B6 / 01001101 10110110
  // GEM6     GEM6    0     None
  const char testData[] = "\x47\x45\x48\xb6\x4D\xB6";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionAscii field(fieldRegistry, "Security", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpTail));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::ASCII));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toAscii(), "GEM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}


BOOST_AUTO_TEST_CASE(testAppendix_3_2_6) // SPEC ERROR: _3 s/b _1
{
  // Multiple Pmap Slot Example  Optional Positive Decimal with individual field operators

  // <decimal id="1" presence="optional" name="Value">
  //    <exponent> <copy/> </exponent>
  //    <mantissa> <copy/> </mantissa>
  // </decimal>
  //    Input    Exponent      Mantissa     PMAP   FAST Hex/Binary
  //             Value>Encode  Value>Encode        Exponent^Mantissa
  //[1] 9427.55  -2>-2         942755>942755 11    FE:39 45 A3 /
  //                                               11111110:00111001 01000101 10100011
  //[2] 9427.60  -2/None       942760>942760 01    None:39 45 A7 /
  //                                               :00111001 01000101 10100111
  //    None     NULL/NULL     None/None     1     0x80:  / 10000000:
  //
  // Errata.  The spec wrongly says:
  //[1] 9427.55  -2>-2         942755>942755 11    FE:39 45 A4 /
  //[1]                                            11111110:00111001 01000101 10100100
  //[2] 9427.60  -2/None       942760>942760 01    None:39 45 A8 /
  //[2]                                            :00111001 01000101 10101000


  const char testData[] = "\xFE\x39\x45\xA3\x39\x45\xA8\x80";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // create a presence map.
  Codecs::PresenceMap pmap(5);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.rewind();

  Messages::FieldRegistry fieldRegistry;
  Codecs::FieldInstructionDecimal field(fieldRegistry, "Value", "", "", "");
  field.setPresence(false);

  Codecs::FieldInstructionExponentPtr exponent(new FieldInstructionExponent(
    fieldRegistry, "value|exponent", "", "", ""));
  exponent->setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.setExponentInstruction(exponent);

  Codecs::FieldInstructionMantissaPtr mantissa(new FieldInstructionMantissa(fieldRegistry, "value|mantissa", "", "", ""));
  mantissa->setFieldOp(Codecs::FieldOpPtr(new Codecs::FieldOpCopy));
  field.setMantissaInstruction(mantissa);
  field.buildIndexes(indexer, "global", "", "");

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));

  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));

  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expectedA(942755, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectedA);


  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::DECIMAL));
  Decimal expectedB(942760, -2);
  BOOST_CHECK(pFieldEntry->getField()->toDecimal() == expectedB);

  BOOST_CHECK_EQUAL(fieldSet3.size(), 0);

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Utf8_Copy_Mandatory)
{
  // Copy Operator Example  Mandatory Unicode String
  // <string id="1" charset="unicode" presence="mandatory" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // CME    None  3:CME   1     83 43 4d 45 /10000011 01000011 01001101 01000101
  // CME    CME   None    0     none
  // ISE    CME   3:ISE   1     83 49 53 C5 /80000011 01001001 01010011 01000101

  const char testData[] = "\x83\x43\x4d\x45\x83\x49\x53\x45";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUtf8 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpCopy));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "CME");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry != fieldSet2.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "CME");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry != fieldSet3.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "ISE");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Utf8_Copy_optional)
{
  // Copy Operator Example for NULL  Optional Unicode String
  // <string id="1" charset="unicode" presence="optional" name="Flag"> <copy/> </string>
  // Input  Prior Encoded PMap  FAST Hex/Binary
  // None   None  Null    1     80 / 10000000
  // None   Null  None    0     None
  // CME    Null  3:CME   1     84 43 4d 45 / 10000100 01000011 01001101 01000101

  const char testData[] = "\x80\x84\x43\x4d\x45";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.setNextField(true);
  pmap.rewind();

  Codecs::FieldInstructionUtf8 field(fieldRegistry, "Flag", "", "", "");
  field.setPresence(false);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpCopy));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  // Capture results in a field set
  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet2.size(), 0);
  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);

  Messages::FieldSet::const_iterator pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "CME");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(test_Utf8_Delta_Mandatory)
{
  // Delta Operator Example  Mandatory Unicode String
  // <string id="1" charset="unicode" presence="mandatory" name="Security"> <delta/> </string>
  // No PMAP bits are used
  // Input    Prior     Encoded     FAST Hex/Binary
  //                    Len:Value   Len/Value
  // GEH6     Empty     0:4:GEH6    80:84:47 45 48 36
  //                                1000000:80000100:01000111 01000101 01001000 00110110
  // GEM6     GEH6      2:2:M6      82:82:4D 36 / 10000010 10000010 01001101 00110110
  // ESM6     GEM6      -2:2:ES     FD:82:45 53 / 11111101 10000010 01000101 01010011
  // RSESM6   ESM6      -0:2:RS     FF:82:52 53 / 11111111 01010010 01010011

  const char testData[] = "\x80\x84\x47\x45\x48\x36\x82\x82\x4D\x36\xFD\x82\x45\x53\xFF\x82\x52\x53";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(1);

  Codecs::FieldInstructionUtf8 field(fieldRegistry, "Security", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpDelta));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 0);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));
  Messages::FieldSet fieldSet4(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet4));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "ESM6");

  BOOST_CHECK_EQUAL(fieldSet4.size(), 1);
  pFieldEntry = fieldSet4.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "RSESM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(2);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  field.encode(destination, pmapResult, encoder, fieldSet4);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}

BOOST_AUTO_TEST_CASE(testUtf8_Tail_Mandatory)
{
  //  Not in spec
  // Tail Operator Example  Mandatory Unicode String
  // <string id="1" charset="unicode" presence="mandatory" name="Security"> <tail/> </string>
  // Input    Prior   PMAP  Encoded     FAST Hex/Binary
  // GEH6     Empty   1     4:GEH6      84:47 45 48 36
  //                                    10000100:01000111 01000101 01001000 00110110
  // GEM6     GEH6    1     2:M6        82:4D 36 / 80000010:01001101 00110110
  // GEM6     GEM6    0     None
  const char testData[] = "\x84\x47\x45\x48\x36\x82\x4D\x36";
  std::string testString(testData, sizeof(testData)-1);
  Codecs::DataSourceString source(testString);

  // create a dictionary indexer
  DictionaryIndexer indexer;
  // prepare the presence map.
  Codecs::PresenceMap pmap(3);
  pmap.setNextField(true);
  pmap.setNextField(true);
  pmap.setNextField(false);
  pmap.rewind();

  Codecs::FieldInstructionUtf8 field(fieldRegistry, "Security", "", "", "");
  field.setPresence(true);
  field.setFieldOp(FieldOpPtr(new Codecs::FieldOpTail));
  field.buildIndexes(indexer, "global", "", "");
  BOOST_CHECK_EQUAL(field.presenceMapBitsRequired(), 1);

  // We neeed the helper routines in the decoder
  Codecs::TemplateRegistryPtr registry(new Codecs::TemplateRegistry(3,3,indexer.size()));
  Codecs::Decoder decoder(registry);

  Messages::FieldSet fieldSet1(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet1));
  Messages::FieldSet fieldSet2(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet2));
  Messages::FieldSet fieldSet3(10);
  BOOST_REQUIRE(field.decode(source, pmap, decoder, fieldSet3));

  // Was all input consumed?
  uchar byte;
  BOOST_CHECK(!source.getByte(byte));

  // check generated fields
  BOOST_CHECK_EQUAL(fieldSet1.size(), 1);
  Messages::FieldSet::const_iterator pFieldEntry = fieldSet1.begin();
  BOOST_CHECK(pFieldEntry != fieldSet1.end());
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEH6");

  BOOST_CHECK_EQUAL(fieldSet2.size(), 1);
  pFieldEntry = fieldSet2.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEM6");

  BOOST_CHECK_EQUAL(fieldSet3.size(), 1);
  pFieldEntry = fieldSet3.begin();
  BOOST_CHECK(pFieldEntry->getField()->isType(Messages::Field::UTF8));
  BOOST_CHECK_EQUAL(pFieldEntry->getField()->toUtf8(), "GEM6");

  // Now reencode the data
  Codecs::PresenceMap pmapResult(3);
  Codecs::DataDestinationString destination;
  destination.startBuffer();
  Codecs::Encoder encoder(registry);
  field.encode(destination, pmapResult, encoder, fieldSet1);
  field.encode(destination, pmapResult, encoder, fieldSet2);
  field.encode(destination, pmapResult, encoder, fieldSet3);
  destination.endMessage();
  const std::string & result = destination.getValue();
  BOOST_CHECK_EQUAL(result, testData);
  BOOST_CHECK(pmap == pmapResult);
}


