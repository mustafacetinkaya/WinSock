#pragma once
#include <iostream>
#include <winsock2.h>
#include <thread>
#include "relations_class.h"
typedef unsigned char byte;


/* packet = 3 + 1 + 1 + [data_size] + 2  = 7 + [data_size]
/
/  3 bytes for the start sequence,
/		1 byte for type,
/			1 byte for data length,
/				data_size bytes for packet data,
/					 2 bytes for the checksum.
*/

enum enum_header {
	psu_header = 1,
	snp_header,
	mcu_header,
	spu_header
};

#pragma pack(push, 1)	// Packed: 1 Byte

struct packet {
	byte header[3];		// MCU - PSU - SNP - SPU
	byte packet_type;	// Type of command
	byte data_size;		// 
	byte *data;
	byte checksum[2];
};

#pragma pack(pop)		// End of pack

enum enum_packet_for {
	psu = 1,
	snp,
	mcu,
	spu,
	undefined
};

enum enum_command_type {

	/* MCU Commands */
	SET_POINT_X = 65,
	SET_POINT_Y,
	SET_POINT_Z,
	SET_POINT_ROLL,
	SET_POINT_PITCH,
	SET_POINT_YAW,
	STOP_ALL, 

	/* SPU Commands */
	GET_IMU_DATA = 60,
	IMU_DATA = 63
};