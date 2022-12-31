/*
 * shell.h
 *
 *  Created on: 7 juin 2019
 *      Author: laurent
 */

#ifndef INC_LIB_SHELL_SHELL_H_
#define INC_LIB_SHELL_SHELL_H_

#include <stdint.h>

#define UART_DEVICE huart1

#define ARGC_MAX 8
#define BUFFER_SIZE 40
#define SHELL_FUNC_LIST_MAX_SIZE 64

struct h_shell_struct;

typedef int (*shell_func_pointer_t)(struct h_shell_struct * h_shell, int argc, char ** argv);
typedef uint8_t (* drv_shell_transmit_t)(const char * pData, uint16_t size);
typedef uint8_t (* drv_shell_receive_t)(char * pData, uint16_t size);

typedef struct shell_func_struct
{
	char c;
	shell_func_pointer_t func;
	char * description;
} shell_func_t;

typedef struct drv_shell_struct
{
    drv_shell_transmit_t transmit;
    drv_shell_receive_t receive;
} drv_shell_t;

typedef struct h_shell_struct
{
	int func_list_size;
	shell_func_t func_list[SHELL_FUNC_LIST_MAX_SIZE];
	char print_buffer[BUFFER_SIZE];
	char cmd_buffer[BUFFER_SIZE];
	drv_shell_t drv;
} h_shell_t;

void shell_init(h_shell_t * h_shell);
int shell_add(h_shell_t * h_shell, char c, shell_func_pointer_t pfunc, char * description);
int shell_run(h_shell_t * h_shell);

#endif /* INC_LIB_SHELL_SHELL_H_ */
