/*
 * XL320.h
 *
 *  Created on: Dec 1, 2022
 *      Author: nassi
 */

#include "main.h"
#include "stdio.h"
#include "cmsis_os.h"


#ifndef INC_XL320_H_
#define INC_XL320_H_

#endif /* INC_XL320_H_ */

#define XL320_ID 0x01

#define packet_length_init 10 // taille de l'instruction packet sans la taille des parametres
#define ping_statut_packet_length 14
#define BUFFER_LENGTH 128
#define PARAMS_LENGTH 10

#define CATCHED_LOAD 300
#define CCW_LOAD_MIN_VALUE 1024

#define XL320_PRESENT_POSITION 37
#define XL320_PRESENT_LOAD 41
#define XL320_TORQUE_ENABLE 24
#define XL320_MOVING_SPEED 32
#define XL320_GOAL_POSITION 30

#define XL320_OPEN_ANGLE 500
#define XL320_CLOSE_ANGLE 350

typedef enum
{
	ping = 0x01,
	read = 0x02,
	write = 0x03,

	reg_write = 0x04,
	action = 0x05,

	factory_reset = 0x06,
	reboot = 0x08,
	clear = 0x10,
	control_table_backup = 0x20,

	sync_read = 0x82,
	sync_write = 0x83,
	fast_sync_read = 0x8A,

	bulk_read = 0x92,
	bulk_write = 0x93,
	fast_bulk_read = 0x9A
} XL320_instructions;

typedef enum
{
	XL320_OK = 1,
	XL320_ERROR = -1,
	XL320_NOT_FOUND = -2,

} XL320_error_handler;


typedef struct statusINFO
{
	uint8_t params;
	uint8_t  id;
	uint8_t  error;
	uint8_t crc_check;
	uint16_t length;
	uint16_t params_length;

} statusINFO;

typedef struct h_XL320_t
{
	uint8_t msg[64];
	uint16_t model_number;
	uint8_t firmware_version;
	uint8_t id;
	UART_HandleTypeDef * uart;
	SemaphoreHandle_t sem_packet;

} h_XL320_t;

uint8_t XL320_send_packet(h_XL320_t * XL320, XL320_instructions inst, uint8_t *params, uint16_t params_length,uint8_t Rx_packet_length);

uint8_t XL320_parse_status_packet(uint8_t *packet, uint32_t packet_length, uint8_t *id, uint8_t *params, uint16_t *params_length, uint8_t *error, uint8_t *crc_check);

void XL320_Clear_rxBUF();

uint8_t XL320_check_status(uint8_t *receiveDATA,statusINFO rxINFO,uint8_t *returnedDATA );

void XL320_Display_Packet(uint8_t * packet,uint8_t size);

uint16_t XL320_read_position(h_XL320_t * XL320);

uint8_t XL320_ping(h_XL320_t * XL320,uint16_t *model_number,uint8_t *firmware_version);

uint8_t XL320_set_goal_position(h_XL320_t * XL320, uint16_t position);

uint8_t XL320_set_speed_position(h_XL320_t * XL320, uint16_t speed);

uint8_t XL320_set_torque_enable(h_XL320_t * XL320, uint8_t enable);

uint8_t XL320_read(h_XL320_t * XL320, uint16_t address, uint16_t data_length, statusINFO rxinfo, uint8_t *returnedDATA);

uint8_t XL320_write(h_XL320_t * XL320, uint16_t address, uint8_t *data, uint16_t data_length);

uint8_t XL320_Init(h_XL320_t * XL320);

uint8_t XL320_Catch(h_XL320_t * XL320);

