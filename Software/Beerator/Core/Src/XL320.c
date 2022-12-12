/*
 * XL320.c
 *
 *  Created on: Dec 1, 2022
 *      Author: nassi
 */

#include "XL320.h"
#include "main.h"
#include <stdlib.h>
#include <stdio.h>

static uint8_t bufRX[BUFFER_LENGTH] = {0};

#define XL320_GET_LO_BYTE(bytes) ((uint8_t)(((uint16_t)(bytes)) & 0xFF)) // Low order byte
#define XL320_GET_HO_BYTE(bytes) ((uint8_t)((((uint16_t)(bytes)) >> 8) & 0xFF)) // High order byte

unsigned short crc_table[256] = {
		0x0000, 0x8005, 0x800F, 0x000A, 0x801B, 0x001E, 0x0014, 0x8011,
		0x8033, 0x0036, 0x003C, 0x8039, 0x0028, 0x802D, 0x8027, 0x0022,
		0x8063, 0x0066, 0x006C, 0x8069, 0x0078, 0x807D, 0x8077, 0x0072,
		0x0050, 0x8055, 0x805F, 0x005A, 0x804B, 0x004E, 0x0044, 0x8041,
		0x80C3, 0x00C6, 0x00CC, 0x80C9, 0x00D8, 0x80DD, 0x80D7, 0x00D2,
		0x00F0, 0x80F5, 0x80FF, 0x00FA, 0x80EB, 0x00EE, 0x00E4, 0x80E1,
		0x00A0, 0x80A5, 0x80AF, 0x00AA, 0x80BB, 0x00BE, 0x00B4, 0x80B1,
		0x8093, 0x0096, 0x009C, 0x8099, 0x0088, 0x808D, 0x8087, 0x0082,
		0x8183, 0x0186, 0x018C, 0x8189, 0x0198, 0x819D, 0x8197, 0x0192,
		0x01B0, 0x81B5, 0x81BF, 0x01BA, 0x81AB, 0x01AE, 0x01A4, 0x81A1,
		0x01E0, 0x81E5, 0x81EF, 0x01EA, 0x81FB, 0x01FE, 0x01F4, 0x81F1,
		0x81D3, 0x01D6, 0x01DC, 0x81D9, 0x01C8, 0x81CD, 0x81C7, 0x01C2,
		0x0140, 0x8145, 0x814F, 0x014A, 0x815B, 0x015E, 0x0154, 0x8151,
		0x8173, 0x0176, 0x017C, 0x8179, 0x0168, 0x816D, 0x8167, 0x0162,
		0x8123, 0x0126, 0x012C, 0x8129, 0x0138, 0x813D, 0x8137, 0x0132,
		0x0110, 0x8115, 0x811F, 0x011A, 0x810B, 0x010E, 0x0104, 0x8101,
		0x8303, 0x0306, 0x030C, 0x8309, 0x0318, 0x831D, 0x8317, 0x0312,
		0x0330, 0x8335, 0x833F, 0x033A, 0x832B, 0x032E, 0x0324, 0x8321,
		0x0360, 0x8365, 0x836F, 0x036A, 0x837B, 0x037E, 0x0374, 0x8371,
		0x8353, 0x0356, 0x035C, 0x8359, 0x0348, 0x834D, 0x8347, 0x0342,
		0x03C0, 0x83C5, 0x83CF, 0x03CA, 0x83DB, 0x03DE, 0x03D4, 0x83D1,
		0x83F3, 0x03F6, 0x03FC, 0x83F9, 0x03E8, 0x83ED, 0x83E7, 0x03E2,
		0x83A3, 0x03A6, 0x03AC, 0x83A9, 0x03B8, 0x83BD, 0x83B7, 0x03B2,
		0x0390, 0x8395, 0x839F, 0x039A, 0x838B, 0x038E, 0x0384, 0x8381,
		0x0280, 0x8285, 0x828F, 0x028A, 0x829B, 0x029E, 0x0294, 0x8291,
		0x82B3, 0x02B6, 0x02BC, 0x82B9, 0x02A8, 0x82AD, 0x82A7, 0x02A2,
		0x82E3, 0x02E6, 0x02EC, 0x82E9, 0x02F8, 0x82FD, 0x82F7, 0x02F2,
		0x02D0, 0x82D5, 0x82DF, 0x02DA, 0x82CB, 0x02CE, 0x02C4, 0x82C1,
		0x8243, 0x0246, 0x024C, 0x8249, 0x0258, 0x825D, 0x8257, 0x0252,
		0x0270, 0x8275, 0x827F, 0x027A, 0x826B, 0x026E, 0x0264, 0x8261,
		0x0220, 0x8225, 0x822F, 0x022A, 0x823B, 0x023E, 0x0234, 0x8231,
		0x8213, 0x0216, 0x021C, 0x8219, 0x0208, 0x820D, 0x8207, 0x0202
};



uint16_t XL320_CRC(uint16_t crc_accum, uint8_t *data_blk_ptr, uint16_t data_blk_size)
{
	uint16_t i, j;
	for (j = 0; j < data_blk_size; j++)
	{
		i = ((uint16_t)(crc_accum >> 8) ^ data_blk_ptr[j]) & 0xFF;
		crc_accum = (crc_accum << 8) ^ crc_table[i];
	}
	return crc_accum;
}



uint8_t XL320_send_packet(h_XL320_t * XL320, uint8_t id, XL320_instructions inst, uint8_t *params, uint16_t params_length,uint8_t Rx_packet_length)
{
	uint32_t packet_length = packet_length_init + params_length;
	uint8_t packet[packet_length];
	packet[0] = 0xFF; /* Header 1. */
	packet[1] = 0xFF; /* Hedaer 2. */
	packet[2] = 0xFD; /* Hedaer 3. */
	packet[3] = 0x00; /* Reserved. */
	packet[4] = id; /* Packet ID. */

	/* Length = Parameter length + 3. */

	packet[5] = XL320_GET_LO_BYTE(params_length + 3);  /* Length 1 (Low-order byte). */
	packet[6] = XL320_GET_HO_BYTE(params_length + 3); /* Lenget 2 (High-order byte). */

	/* Instruction */
	packet[7] = (uint8_t)inst;

	for (uint16_t i = 0; i < params_length; i++)
	{
		packet[8 + i] = params[i];
	}

	uint16_t crc=XL320_CRC(0, packet, packet_length - 2);
	packet[packet_length-2]=XL320_GET_LO_BYTE(crc);
	packet[packet_length-1]=XL320_GET_HO_BYTE(crc);
	printf("\r\n Send Packet \r\n");
	XL320_Display_Packet(packet,packet_length);


	HAL_HalfDuplex_EnableTransmitter(XL320->uart);
	if (HAL_UART_Transmit(XL320->uart, packet, packet_length, 0xFFFF)==HAL_OK)
	{
		HAL_HalfDuplex_EnableReceiver(XL320->uart);
		HAL_UART_Receive_IT(XL320->uart, bufRX, Rx_packet_length);
		printf("\r\nData transmitted successfully\r\n");

		return SEND_SERVO_Ok;
	}
	else
	{
		printf("Data is not transmitted\r\n");
		return SEND_SERVO_ERROR;
	}
}


uint8_t XL320_ping(h_XL320_t * XL320, uint8_t id,uint16_t *model_number,uint8_t *firmware_version)
{

	statusINFO rxINFO;
	uint8_t returnedDATA[64];



	XL320_Clear_rxBUF();
	if(XL320_send_packet(XL320, id, ping, NULL, 0, ping_statut_packet_length)==SEND_SERVO_Ok)
	{
		printf("\r\nReceived Ping Packet \r\n");
		XL320_Display_Packet(bufRX, ping_statut_packet_length);
		if(XL320_check_status(bufRX,rxINFO,returnedDATA)== SERVO_Ok )
		{

			*model_number = returnedDATA[0]+(returnedDATA[1]<<8);
			*firmware_version = returnedDATA[2];
            return PING_SERVO_OK;
		}
		else{
			return PING_SERVO_ERROR;
		}


	}
	else{
		return SEND_SERVO_ERROR;
	}



}

uint8_t XL320_check_status(uint8_t *receiveDATA,statusINFO rxINFO,uint8_t *returnedDATA )
{
	if(receiveDATA[7] == 0x55)
	{
		rxINFO.id = receiveDATA[4];
		rxINFO.error = receiveDATA[8];
		rxINFO.length = receiveDATA[5] + ((uint16_t)(receiveDATA[6] << 8) & 0xFF00);
		rxINFO.params_length = rxINFO.length - 3 ;

		for(uint16_t i=0 ; i < rxINFO.params_length ; i++ )
		{
			returnedDATA[i] = receiveDATA[i + 9];
		}

		return SERVO_Ok;
	}
	return SERVO_ERROR ;
}

void XL320_Clear_rxBUF()
{
	for(uint8_t i=0;i<BUFFER_LENGTH;i++)
	{
		bufRX[i] = 0;

	}
}

void XL320_Display_Packet(uint8_t * packet,uint8_t size)
{
	uint8_t i=0;
	char buffer[50];
	for (i=0;i<size;i++)
	{
		if (i==size-1)
		{
			sprintf(buffer," 0x%02x\r\n",packet[i]);
		}
		else if (i==0)
		{
			sprintf(buffer,"0x%02x |",packet[i]);
		}
		else
		{
			sprintf(buffer," 0x%02x |",packet[i]);
		}
		printf(buffer);
	}
}

uint8_t XL320_read(h_XL320_t * XL320, uint8_t id, uint16_t address, uint16_t data_length, statusINFO rxinfo, uint8_t *returnedDATA)
{
	uint8_t params[4];


	/* Parameter 1~2: Starting address. */
	params[0] = XL320_GET_LO_BYTE(address);
	params[1] = XL320_GET_HO_BYTE(address);

	/* Parameter 2~3: Data length. */
	params[2] = XL320_GET_LO_BYTE(data_length);
	params[3] = XL320_GET_HO_BYTE(data_length);

	XL320_Clear_rxBUF();
	if(XL320_send_packet(XL320,id, read, params, 4, 15)== SEND_SERVO_Ok)
	{
		if(XL320_check_status(bufRX,rxinfo,returnedDATA)== SERVO_Ok )
		{
			return READ_SERVO_Ok;
		}
		else {
			return READ_SERVO_ERROR;

		}
	}
	else
		return SEND_SERVO_ERROR;


	return 1 ;

}

uint16_t XL320_read_position(h_XL320_t * XL320, uint8_t id)
{
	uint16_t address = XL320_PRESENT_POSITION;
	uint8_t return_data[2];
	statusINFO rxinfo;

	if(XL320_read(XL320, id, address, 2, rxinfo,return_data)== READ_SERVO_Ok)
	{
		uint16_t position = return_data[0] + ((return_data[1] << 8) & 0xFF00);
		return position;
	}
	else
		return READ_SERVO_ERROR;
}

uint16_t XL320_read_present_position(h_XL320_t * XL320, uint8_t id)
{
	uint16_t address = XL320_PRESENT_POSITION;
	uint8_t return_data[2];
	statusINFO rxinfo;

	if(XL320_read(XL320, id, address, 2, rxinfo,return_data)== READ_SERVO_Ok)
	{
		uint16_t position = return_data[0] + ((return_data[1] << 8) & 0xFF00);
		return position;
	}
	else
		return READ_SERVO_ERROR;
}

uint8_t XL320_write(h_XL320_t * XL320, uint8_t id, uint16_t address, uint8_t *data, uint16_t data_length)
{
	uint32_t params_length = data_length + 2 ; // 2 byte pour l'adresse
	uint8_t params[params_length];

	/* Parameter 1~2: Starting address. */
	params[0] = XL320_GET_LO_BYTE(address);
	params[1] = XL320_GET_HO_BYTE(address);

	/* Parameter 3~X: Data. */
	for (uint16_t i = 0; i < data_length; i++)
	{
		params[2 + i] = data[i];
	}
	printf("\r\n Write packet \r\n");
	XL320_Clear_rxBUF();
	if(XL320_send_packet(XL320, id, write, params, params_length,11)==SEND_SERVO_Ok)
	{
		printf("\r\n Received Write packet \r\n");
		XL320_Display_Packet(bufRX,11);
		return WRITE_SERVO_Ok;
	}
	else
		return WRITE_SERVO_ERROR;




}

uint8_t XL320_set_torque_enable(h_XL320_t * XL320, uint8_t id, uint8_t enable)
{
	uint16_t address = XL320_TORQUE_ENABLE;
	uint8_t data = enable ;
	if(XL320_write(XL320, id, address, &data, 1)==WRITE_SERVO_Ok)
	{
		return TORQUE_SERVO_Ok;
	}
	else
		return TORQUE_SERVO_ERROR;

}

uint8_t XL320_set_speed_position(h_XL320_t * XL320,uint8_t id, uint16_t speed)
{
	uint16_t address = XL320_MOVING_SPEED;
	uint8_t data[2];
	data[0] = (uint8_t)(speed & 0xFF);
	data[1] = (uint8_t)((speed >> 8) & 0xFF);

	if(XL320_write(XL320, id, address, data, 2)== WRITE_SERVO_Ok)
	{
		return SPEED_SERVO_Ok;
	}
	else
		return SPEED_SERVO_ERROR;
}

uint8_t XL320_set_goal_position(h_XL320_t * XL320, uint8_t id, uint16_t position)
{
	uint16_t address = XL320_GOAL_POSITION;
	uint8_t data[2];
	data[0] = (uint8_t)(position & 0xFF);
	data[1] = (uint8_t)((position >> 8) & 0xFF);

	if(XL320_write(XL320, id, address, data, 2)== WRITE_SERVO_Ok)
	{
		return GOAL_SERVO_Ok;
	}
	else
		return GOAL_SERVO_ERROR;
}


