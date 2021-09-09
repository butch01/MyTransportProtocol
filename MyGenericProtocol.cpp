/*
 * MyGenericProtocol.cpp
 *
 *  Created on: 05.09.2021
 *      Author: butch
 */

#include "MyGenericProtocol.h"




MyGenericProtocol::MyGenericProtocol() {
	// initialize _dataPacket with zero
	memset(_dataPacket, 0, sizeof(_dataPacket));
	_msgNo 			= 0;
	_mode 			= 0;
	_fragCount		= 0;
	_fragNo			= 0;

}

MyGenericProtocol::~MyGenericProtocol() {
	// TODO Auto-generated destructor stub
}

uint8_t MyGenericProtocol::getMode()
{


	return 0;
}


/**
 * sets the mode.
 */
uint8_t MyGenericProtocol::setMode(uint8_t mode)
{
	bool rc = 0;
	if (mode >= PROTOCOL_BITS_MODE_VALUE_MIN && (double) mode < pow( (long) 2, (long) PROTOCOL_BITS_MODE))
	{
		_mode = mode;
	}
	else
	{
		rc = RC_ERROR_VALUE_OUT_OF_BOUNDS;
	}
	return rc;
}


/**
 * sets the msgNo.
 * msgNo can be 0
 * msgNo will be truncated to the last PROTOCOL_BITS_MSG_NO (lsb).
 * handles rollover
 */
uint8_t MyGenericProtocol::setMsgNo(uint8_t msgNo)
{
	bool rc = 0;
	uint8_t valueComparator = 0;
	uint8_t oldMsgNo = _msgNo; 			// remember it to check for rollover

	// prepare the comparator for bitwise and
	for (unsigned char i=0; i<PROTOCOL_BITS_MSG_NO; i++ )
	{
		valueComparator |= 1UL << i;
	}

	// bitwise and of valueComparator and msgNo (to handle the rollover and truncate to allowed bit count)
	_msgNo = msgNo & valueComparator;

	// check if we have a rollover of msgNo
	if (msgNo < oldMsgNo)
	{
		rc = RC_VALUE_ROLLOVER;
	}
	return rc;
}


/**
 * increases the message number by one.
 * Rollover is handled by subcalled method.
 */
uint8_t MyGenericProtocol::setMsgNoToNextId()
{
	return setMsgNo(_msgNo +1);
}


/**
 * fragCount must be bigger than 0
 * fragCount must be less than
 * number of fragments
 */
uint8_t MyGenericProtocol::setFragCount(uint8_t fragCount)
{
	uint8_t rc = 0;

	// check for value range
	if (fragCount >= PROTOCOL_BITS_FRAG_COUNT_VALUE_MIN && fragCount < pow( (long) 2, (long) PROTOCOL_BITS_FRAG_COUNT))
	{
		_fragCount = fragCount;
	}
	else
	{
		rc = RC_ERROR_VALUE_OUT_OF_BOUNDS;
	}
	return rc;
}


/**
 * sets the fragNo
 * checks against bit bounds
 * checks that fragNo is no bigger than fragCount
 */
uint8_t MyGenericProtocol::setFragNo(uint8_t fragNo)
{
	uint8_t rc = 0;
	// check for value range
	if (fragNo >= PROTOCOL_BITS_FRAG_NO_VALUE_MIN && fragNo < pow( (long) 2, (long) PROTOCOL_BITS_FRAG_NO) && fragNo <= _fragCount)
	{
		_fragNo = fragNo;
	}
	else
	{
		rc = RC_ERROR_VALUE_OUT_OF_BOUNDS;
	}
	return rc;
}


/**
 * increases the fragNo by 1.
 * checks against bit bounds
 * checks that fragNo is no bigger than fragCount
 */
uint8_t MyGenericProtocol::setFragNoToNextId()
{
	return setFragNo(_fragNo +1);
}


/**
 * reset the fragNo to PROTOCOL_BITS_FRAG_NO_VALUE_MIN
 */
void MyGenericProtocol::resetFragNo()
{
	_fragNo = PROTOCOL_BITS_FRAG_NO_VALUE_MIN;
}


/**
 * wrapper for setting the header at once.
 * NO HEADER GENERATION !!!
 */
void MyGenericProtocol::setHeader(uint8_t mode, uint8_t msgNo, uint8_t fragCount, uint8_t fragNo)
{
	setMode(mode);
	setMsgNo(msgNo);
	setFragCount(fragCount);
	setFragNo(fragNo);
}


uint8_t MyGenericProtocol::generateHeader()
{
	uint8_t rc = 0;
	_header = 0; // init header (2 bytes)

	_header = _mode;
	_header = _header << PROTOCOL_BITS_MSG_NO; // bit shifting left

	_header = _header + _msgNo;
	_header = _header << PROTOCOL_BITS_FRAG_COUNT; // bit shifting

	_header = _header + _fragCount;
	_header = _header << PROTOCOL_BITS_FRAG_NO ;	// bit shifting

	_header = _header + _fragNo;
	// no bit shifting at the last element

	return rc;
}


/**
 * returns the header
 */
uint16_t MyGenericProtocol::getHeader()
{
	return _header;
}

/**
 * decodes the _header variable. Result is stored
 * in _mode, _msgNo, _fragCount, _fragNo
 */
void  MyGenericProtocol::decodeHeader()
{
	uint16_t comparator = (pow ((long)2, (long) PROTOCOL_BITS_FRAG_NO))-1;
	_fragNo = _header & comparator;

	comparator = (pow ((long)2, (long) PROTOCOL_BITS_FRAG_COUNT))-1;
	_fragCount = _header >> PROTOCOL_BITS_FRAG_NO & comparator;

	comparator = (pow ((long)2, (long) PROTOCOL_BITS_MSG_NO))-1;
	_msgNo = _header >> (PROTOCOL_BITS_FRAG_NO + PROTOCOL_BITS_FRAG_COUNT ) & comparator;

	comparator = (pow ((long)2, (long) PROTOCOL_BITS_MODE))-1;
	_mode = _header >> (PROTOCOL_BITS_FRAG_NO + PROTOCOL_BITS_FRAG_COUNT + PROTOCOL_BITS_MSG_NO ) & comparator;
}

/**
 * generates the data packet
 */
void MyGenericProtocol::generateDataPacket(uint8_t *payload)
{
	_dataPacket[0] = DATA_PACKET_MARKER_VALUE_BEGIN;
	_dataPacket[1] = _header >> 8;
	_dataPacket[2] = _header & 255;

	memcpy(_dataPacket+3, payload, 26);
	_dataPacket[DATA_PACKET_LENGTH -1] = DATA_PACKET_MARKER_VALUE_END;


}


/**
 * returns a pointer to the complete dataPacket
 */
uint8_t* MyGenericProtocol::getDataPacket()
{
	return _dataPacket;
}

/**
 * getter for _msgNo
 */
uint8_t MyGenericProtocol::getMsgNo()
{
	return _msgNo;
}

/**
 * getter for _fragCount
 */
uint8_t MyGenericProtocol::getFragCount()
{
	return _fragCount;
}

/**
 * getter for _fragNo
 */
uint8_t MyGenericProtocol::getFragNo()
{
	return _fragNo;
}
