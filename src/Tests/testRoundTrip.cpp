// Copyright (c) 2009, Object Computing, Inc.
// All rights reserved.
// See the file license.txt for licensing information.
#include <Common/QuickFASTPch.h>

#define BOOST_TEST_NO_MAIN QuickFASTTest
#include <boost/test/unit_test.hpp>

#include <Codecs/XMLTemplateParser.h>
#include <Codecs/TemplateRegistry.h>
#include <Codecs/Encoder.h>
#include <Codecs/Decoder.h>
#include <Codecs/DataDestinationString.h>
#include <Codecs/DataSourceString.h>

#include <Messages/Message.h>
#include <Messages/FieldIdentity.h>
#include <Messages/FieldInt32.h>
#include <Messages/FieldUInt32.h>
#include <Messages/FieldInt64.h>
#include <Messages/FieldUInt64.h>
#include <Messages/FieldAscii.h>
#include <Messages/FieldByteVector.h>
#include <Messages/FieldDecimal.h>
#include <Messages/FieldGroup.h>
#include <Messages/FieldSequence.h>
#include <Messages/FieldUtf8.h>
#include <Messages/Sequence.h>

#include <Common/Exceptions.h>

using namespace QuickFAST;

namespace{
  void validateMessage1(Messages::Message & message)
  {
    BOOST_CHECK_EQUAL(message.getApplicationType(), "instrumentreferencedata");
    Messages::FieldCPtr value;

    //msg.addField(index_timestamp, Messages::FieldUInt32::create(1));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//timestamp", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 1);

    //msg.addField(index_srcId, Messages::FieldUInt32::create(2));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//srcId", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2);

    //msg.addField(index_seqNum, Messages::FieldUInt32::create(3));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//seqNum", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 3);

    //msg.addField(index_isix, Messages::FieldUInt32::create(4));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//isix", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 4);

    //msg.addField(index_isin, Messages::FieldAscii::create("isin"));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//isin", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "isin");

    //msg.addField(index_exchId, Messages::FieldAscii::create("exchId"));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//exchId", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "exchId");

    //msg.addField(index_instGrp, Messages::FieldAscii::create("instGrp"));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//instGrp", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "instGrp");

    //msg.addField(index_instTypCod, Messages::FieldAscii::create("instTypCod"));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//instTypCod", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "instTypCod");

    //msg.addField(index_currCode, Messages::FieldAscii::create("currCode"));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//currCode", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "currCode");

    //msg.addField(index_ticSiz, Messages::FieldDecimal::create(Decimal(123, -1)));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//ticSiz", value));
    BOOST_CHECK_EQUAL(value->toDecimal(), Decimal(123, -1));
    //msg.addField(index_setId, Messages::FieldUInt32::create(5));
    BOOST_REQUIRE(message.getField("/instrumentreferencedata//setId", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 5);

    BOOST_REQUIRE(message.getField("/instrumentreferencedata//MDFeedTypes", value));
    const Messages::SequenceCPtr & mdft = value->toSequence();
    BOOST_CHECK_EQUAL(mdft->size(), 2);
    Messages::FieldSetCPtr seqEntry = (*mdft)[0];

    //entry->addField(index_streamType, Messages::FieldAscii::create("streamType"));
    BOOST_REQUIRE( seqEntry->getField("///streamType", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamType");

    //entry->addField(index_streamService, Messages::FieldAscii::create("streamService"));
    BOOST_REQUIRE( seqEntry->getField("///streamService", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamService");

    //entry->addField(index_inetAddr, Messages::FieldAscii::create("inetAddr.com"));
    BOOST_REQUIRE( seqEntry->getField("///inetAddr", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "inetAddr.com");

    //entry->addField(index_port, Messages::FieldUInt32::create(2222));
    BOOST_REQUIRE( seqEntry->getField("///port", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2222);

    //entry->addField(index_mktDepth, Messages::FieldUInt32::create(10));
    BOOST_REQUIRE( seqEntry->getField("///mktDepth", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 10);

    // optional field omitted
    BOOST_REQUIRE(!seqEntry->getField("///mdBookType", value));

    seqEntry = (*mdft)[1];

    //entry->addField(index_streamType, Messages::FieldAscii::create("streamType2"));
    BOOST_REQUIRE( seqEntry->getField("///streamType", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamType2");

    //entry->addField(index_streamService, Messages::FieldAscii::create("streamService2"));
    BOOST_REQUIRE( seqEntry->getField("///streamService", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "streamService2");

    //entry->addField(index_inetAddr, Messages::FieldAscii::create("inetAddr.org"));
    BOOST_REQUIRE( seqEntry->getField("///inetAddr", value));
    BOOST_CHECK_EQUAL(value->toAscii(), "inetAddr.org");

    //entry->addField(index_port, Messages::FieldUInt32::create(2224));
    BOOST_REQUIRE( seqEntry->getField("///port", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 2224);
    // optional field omitted
    BOOST_REQUIRE(!seqEntry->getField("///mktDepth", value));
    //entry->addField(index_mdBookType, Messages::FieldUInt32::create(3));
    BOOST_REQUIRE(seqEntry->getField("///mdBookType", value));
    BOOST_CHECK_EQUAL(value->toUInt32(), 3);
  }
}

BOOST_AUTO_TEST_CASE(testRoundTripSequenceNoPMAP)
{
  // Template from Issue #1
  std::string theTemplate =
    "<templates>"
    "  <template name=\"InstrumentReferenceData\" id=\"3\">"
    "    <typeRef name=\"instrumentreferencedata\"/>"
    "    <uInt32 name=\"timestamp\" id=\"52\">"
    "      <delta/>"
    "    </uInt32>"
    "    <uInt32 name=\"srcId\" id=\"50\">"
    "      <copy/>"
    "    </uInt32>"
    "    <uInt32 name=\"seqNum\" id=\"34\">"
    "      <increment value=\"1\"/>"
    "    </uInt32>"
    "    <uInt32 name=\"isix\" id=\"48\">"
    "      <delta/>"
    "    </uInt32>"
    "    <string name=\"isin\" id=\"455\">"
    "      <delta/>"
    "    </string>"
    "    <string name=\"exchId\" id=\"207\">"
    "      <copy/>"
    "    </string>"
    "    <string name=\"instGrp\" id=\"1151\">"
    "      <copy/>"
    "    </string>"
    "    <string name=\"instTypCod\" id=\"461\">"
    "      <copy/>"
    "    </string>"
    "    <string name=\"currCode\" id=\"15\">"
    "      <copy/>"
    "    </string>"
    "    <decimal name=\"ticSiz\" id=\"969\">"
    "      <delta/>"
    "    </decimal>"
    "    <uInt32 name=\"setId\" id=\"TBD\">"
    "      <copy/>"
    "    </uInt32>"
    "    <sequence name=\"MDFeedTypes\">"
    "      <length name=\"noOfStreams\" id=\"1141\"/>"
    "      <string name=\"streamType\" id=\"1022\"/>"
    "      <string name=\"streamService\"/>"
    "      <string name=\"inetAddr\">"
    "        <delta/>"
    "      </string>"
    "      <uInt32 name=\"port\" id=\"TBD\">"
    "        <delta/>"
    "      </uInt32>"
    "      <uInt32 name=\"mktDepth\" id=\"264\" presence=\"optional\"/>"
    "      <uInt32 name=\"mdBookType\" id=\"1021\" presence=\"optional\"/>"
    "    </sequence>"
    "  </template>"
    "</templates>";

  Codecs::XMLTemplateParser parser;
  std::stringstream templateStream(theTemplate);
  Codecs::TemplateRegistryPtr templateRegistry =
    parser.parse(templateStream);

  BOOST_CHECK(templateRegistry);
  BOOST_CHECK_EQUAL(templateRegistry->maxFieldCount(), 12);

  Messages::FieldRegistry fieldRegistry;

  Messages::FieldRegistry::Index  index_timestamp = fieldRegistry.addFieldIdentity("timestamp", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_srcId = fieldRegistry.addFieldIdentity("srcId", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_seqNum = fieldRegistry.addFieldIdentity("seqNum", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_isix = fieldRegistry.addFieldIdentity("isix", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_isin = fieldRegistry.addFieldIdentity("isin", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_exchId = fieldRegistry.addFieldIdentity("exchId", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_instGrp = fieldRegistry.addFieldIdentity("instGrp", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_instTypCod = fieldRegistry.addFieldIdentity("instTypCod", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_currCode = fieldRegistry.addFieldIdentity("currCode", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_ticSiz = fieldRegistry.addFieldIdentity("ticSiz", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_setId = fieldRegistry.addFieldIdentity("setId", "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_MDFeedTypes = fieldRegistry.addFieldIdentity("MDFeedTypes", "", "instrumentreferencedata","");
// We don't need an index for the length field.
//  Messages::FieldRegistry::Index  index_noOfStreams = fieldRegistry.addFieldIdentity("noOfStreams");
  Messages::FieldRegistry::Index  index_streamType = fieldRegistry.addFieldIdentity("streamType"); // @TODO: , "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_streamService = fieldRegistry.addFieldIdentity("streamService"); // @TODO: , "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_inetAddr = fieldRegistry.addFieldIdentity("inetAddr"); // @TODO: , "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_port = fieldRegistry.addFieldIdentity("port"); // @TODO: , "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_mktDepth = fieldRegistry.addFieldIdentity("mktDepth"); // @TODO: , "", "instrumentreferencedata","");
  Messages::FieldRegistry::Index  index_mdBookType = fieldRegistry.addFieldIdentity("mdBookType"); // @TODO: , "", "instrumentreferencedata","");

  Messages::Message msg(
    fieldRegistry,
    templateRegistry->maxFieldCount());

//   <uInt32 name=\"timestamp\" id=\"52\"><delta/></uInt32>"
  msg.addField(
    fieldRegistry,
    index_timestamp, Messages::FieldUInt32::create(1));
//   <uInt32 name=\"srcId\" id=\"50\"><copy/></uInt32>"
  msg.addField(
    fieldRegistry,
    index_srcId, Messages::FieldUInt32::create(2));
//   <uInt32 name=\"seqNum\" id=\"34\"><increment value=\"1\"/></uInt32>"
  msg.addField(
    fieldRegistry,
    index_seqNum, Messages::FieldUInt32::create(3));
//   <uInt32 name=\"isix\" id=\"48\"><delta/></uInt32>"
  msg.addField(
    fieldRegistry,
    index_isix, Messages::FieldUInt32::create(4));
//   <string name=\"isin\" id=\"455\"><delta/></string>"
  msg.addField(
    fieldRegistry,
    index_isin, Messages::FieldAscii::create("isin"));
//   <string name=\"exchId\" id=\"207\"><copy/></string>"
  msg.addField(
    fieldRegistry,
    index_exchId, Messages::FieldAscii::create("exchId"));
//   <string name=\"instGrp\" id=\"1151\"><copy/></string>"
  msg.addField(
    fieldRegistry,
    index_instGrp, Messages::FieldAscii::create("instGrp"));
//   <string name=\"instTypCod\" id=\"461\"><copy/></string>"
  msg.addField(
    fieldRegistry,
    index_instTypCod, Messages::FieldAscii::create("instTypCod"));
//   <string name=\"currCode\" id=\"15\"><copy/></string>"
  msg.addField(
    fieldRegistry,
    index_currCode, Messages::FieldAscii::create("currCode"));
//   <decimal name=\"ticSiz\" id=\"969\"><delta/></decimal>"
  msg.addField(
    fieldRegistry,
    index_ticSiz, Messages::FieldDecimal::create(Decimal(123, -1)));
//   <uInt32 name=\"setId\" id=\"TBD\"><copy/></uInt32>"
  msg.addField(
    fieldRegistry,
    index_setId, Messages::FieldUInt32::create(5));

//   <sequence name=\"MDFeedTypes\">"
//     <length name=\"noOfStreams\" id=\"1141\"/>"
  Messages::SequencePtr sequence_MDFeedTypes(new Messages::Sequence);
  Messages::FieldSetPtr entry(new Messages::FieldSet(
    fieldRegistry,
    6)); // todo Hardcoded 6?

//     <string name=\"streamType\" id=\"1022\"/>"
  entry->addField(
    fieldRegistry,
    index_streamType, Messages::FieldAscii::create("streamType"));
//     <string name=\"streamService\"/>"
  entry->addField(
    fieldRegistry,
    index_streamService, Messages::FieldAscii::create("streamService"));
//     <string name=\"inetAddr\"><delta/></string>"
  entry->addField(
    fieldRegistry,
    index_inetAddr, Messages::FieldAscii::create("inetAddr.com"));
//     <uInt32 name=\"port\" id=\"TBD\"><delta/></uInt32>"
  entry->addField(
    fieldRegistry,
    index_port, Messages::FieldUInt32::create(2222));
//     <uInt32 name=\"mktDepth\" id=\"264\" presence=\"optional\"/>"
  entry->addField(
    fieldRegistry,
    index_mktDepth, Messages::FieldUInt32::create(10));
//     <uInt32 name=\"mdBookType\" id=\"1021\" presence=\"optional\"/>"
// optional field omitted

  sequence_MDFeedTypes->addEntry(entry);

  entry.reset(new Messages::FieldSet(
    fieldRegistry,
    6));
//     <string name=\"streamType\" id=\"1022\"/>"
  entry->addField(
    fieldRegistry,
    index_streamType, Messages::FieldAscii::create("streamType2"));
//     <string name=\"streamService\"/>"
  entry->addField(
    fieldRegistry,
    index_streamService, Messages::FieldAscii::create("streamService2"));
//     <string name=\"inetAddr\"><delta/></string>"
  entry->addField(
    fieldRegistry,
    index_inetAddr, Messages::FieldAscii::create("inetAddr.org"));
//     <uInt32 name=\"port\" id=\"TBD\"><delta/></uInt32>"
  entry->addField(
    fieldRegistry,
    index_port, Messages::FieldUInt32::create(2224));
//     <uInt32 name=\"mktDepth\" id=\"264\" presence=\"optional\"/>"
// optional field omitted
//     <uInt32 name=\"mdBookType\" id=\"1021\" presence=\"optional\"/>"
  entry->addField(
    fieldRegistry,
    index_mdBookType, Messages::FieldUInt32::create(3));

  sequence_MDFeedTypes->addEntry(entry);

  msg.addField(
    fieldRegistry,
    index_MDFeedTypes, Messages::FieldSequence::create(sequence_MDFeedTypes));

  Codecs::Encoder encoder(templateRegistry);
  Codecs::DataDestinationString destination;
  template_id_t templId = 3; // from the XML above
  encoder.encodeMessage(destination, templId, msg);
  const std::string & fastString = destination.getValue();

  Codecs::Decoder decoder(templateRegistry);
  Codecs::DataSourceString source(fastString);
  Messages::Message msgOut(
    fieldRegistry,
    templateRegistry->maxFieldCount());
  decoder.decodeMessage(source, msgOut);

  validateMessage1(msgOut);
  Messages::Message messageCopy(msgOut);
  validateMessage1(messageCopy);


  // wanna see it again?
  encoder.encodeMessage(destination, templId, msgOut);
  const std::string reencoded = destination.getValue();

  BOOST_CHECK(fastString == reencoded);
}
