//============================================================================
// Name        : cppTest.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include "MyGenericProtocol.h"
#include <bitset>

using namespace std;


int main() {
	uint8_t payload[26] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26};
	// memset(payload, 1, sizeof(payload));

	MyGenericProtocol p;
	p.setHeader(2, 4, 6, 3);
	p.generateHeader();


	p.decodeHeader();
	p.generateDataPacket(payload);

	p.getDataPacket();

	uint8_t *test;
	test = p.getDataPacket();

	printf("received data packet\n");
	for (int i=0; i < 30; i++)
	{
		uint8_t t1 = *(test+i);
//		cout << (int) t1 << endl;
		printf("%3d: %3d   ", i, (int) t1);
		cout <<  std::bitset<8>((int) t1) << endl;
	}


	return 0;
}
