/*
 * MyGenericProtocol.h
 *
 *  Created on: 05.09.2021
 *      Author: butch
 */

#include <string.h>
#include <cstdint>
#include <math.h>

#ifndef MYGENERICPROTOCOL_H_
#define MYGENERICPROTOCOL_H_

#define DATA_PACKET_LENGTH 30
#define DATA_PACKET_MARKER_VALUE_BEGIN 0
#define DATA_PACKET_MARKER_VALUE_END 255

// protocol: start marker (1 byte) | header (2 byte) | payload (26 byte) | end marker (1 byte)


#define PROTOCOL_BITS_MODE 2
#define PROTOCOL_BITS_MODE_VALUE_MIN 1
#define PROTOCOL_BITS_MSG_NO 4
#define PROTOCOL_BITS_FRAG_COUNT 5
#define PROTOCOL_BITS_FRAG_COUNT_VALUE_MIN 1
#define PROTOCOL_BITS_FRAG_NO 5
#define PROTOCOL_BITS_FRAG_NO_VALUE_MIN 1





#define RC_ERROR_VALUE_OUT_OF_BOUNDS 2
#define RC_VALUE_ROLLOVER 3


class MyGenericProtocol {
	public:
		MyGenericProtocol();
		virtual ~MyGenericProtocol();
		uint8_t getMode();

		void setHeader(uint8_t mode, uint8_t msgNo, uint8_t fragCount, uint8_t fragNo); // wrapper to set header at once. NO HEADER GENEREATION !!!
		uint16_t getHeader();
		uint8_t  generateHeader();						// generate the header out of mode, msgNo, fragCount, fragNo
		void     decodeHeader();						// decode the header. Fills the variables _mode, _msgNo, _fragCount, _fragNo
		uint8_t  setMode(uint8_t mode);					// set mode

		uint8_t  getMsgNo();							// get msgNo
		uint8_t  setMsgNo(uint8_t msgNo);				// set msgNo explicitly

		uint8_t  setMsgNoToNextId(); 					// increase msgNo to next Id.

		uint8_t  getFragCount();						// get fragCount
		uint8_t  setFragCount(uint8_t fragCount); 		// set the count of fragments of this message

		uint8_t  getFragNo();							// getFragNo
		uint8_t  setFragNo(uint8_t fragNo); 			// sets the current fragment No
		void     resetFragNo();							// resets the fragNo
		uint8_t  setFragNoToNextId(); 					// increases the fragment no next id
		uint8_t* getPayload();

		void     generateDataPacket(uint8_t *payload);	// generates complete data packet (including start, header, payload, end)
		uint8_t* getDataPacket();						// returns a pointer to the data packet




	private:
		uint8_t   _dataPacket[DATA_PACKET_LENGTH];
		uint8_t   _msgNo;
		uint8_t   _mode;
		uint8_t   _fragCount;
		uint8_t   _fragNo;
		uint16_t  _header;
};

#endif /* MYGENERICPROTOCOL_H_ */
