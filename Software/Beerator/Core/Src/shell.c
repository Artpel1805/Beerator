/*
 * shell.c
 *
 *  Created on: 7 juin 2019
 *      Author: Laurent Fiack
 */

#include "shell.h"
#include "main.h"

#include <stdio.h>

static int sh_help(h_shell_t * h_shell, int argc, char ** argv) {
	int i;
	for(i = 0 ; i < h_shell->func_list_size ; i++) {
		int size;
		size = snprintf (h_shell->print_buffer, BUFFER_SIZE, "%c: %s\r\n", h_shell->func_list[i].c, h_shell->func_list[i].description);
		if( h_shell->drv.transmit(h_shell->print_buffer, size) != HAL_OK ){
			return SHELL_ERROR;
		}
	}

	return SHELL_OK;
}

int shell_init(h_shell_t * h_shell) {
	int size = 0;

	h_shell->func_list_size = 0;

	size = snprintf (h_shell->print_buffer, BUFFER_SIZE, "\r\n\r\n===== Beerator Shell =====\r\n");

	if( h_shell->drv.transmit(h_shell->print_buffer, size) != HAL_OK ){
		return SHELL_ERROR;
	}
	shell_add(h_shell, 'h', sh_help, "Help");
	return SHELL_OK;
}

int shell_add(h_shell_t * h_shell, char c, shell_func_pointer_t pfunc, char * description) {
	if (h_shell->func_list_size < SHELL_FUNC_LIST_MAX_SIZE) {
		h_shell->func_list[h_shell->func_list_size].c = c;
		h_shell->func_list[h_shell->func_list_size].func = pfunc;
		h_shell->func_list[h_shell->func_list_size].description = description;
		h_shell->func_list_size++;
		return 0;
	}

	return -1;
}

static int shell_exec(h_shell_t * h_shell, char * buf) {
	int i;

	char c = buf[0];

	int argc;
	char * argv[ARGC_MAX];
	char *p;

	for(i = 0 ; i < h_shell->func_list_size ; i++) {
		if (h_shell->func_list[i].c == c) {
			argc = 1;
			argv[0] = buf;

			for(p = buf ; *p != '\0' && argc < ARGC_MAX ; p++){
				if(*p == ' ') {
					*p = '\0';
					argv[argc++] = p+1;
				}
			}

			return h_shell->func_list[i].func(h_shell, argc, argv);
		}
	}

	int size;
	size = snprintf (h_shell->print_buffer, BUFFER_SIZE, "%c: no such command\r\n", c);
	h_shell->drv.transmit(h_shell->print_buffer, size);
	return -1;
}

static const char backspace[] = "\b \b";
static const char prompt[] = "> ";

int shell_run(h_shell_t * h_shell) {
	h_shell->drv.transmit(prompt, 2);
	while (1) {
		h_shell->drv.receive(&h_shell->RxBuffer, 1);
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		shell_Rx_CallBack(h_shell);
	}
	return 0;
}


int shell_Rx_CallBack(h_shell_t * h_shell){
	int size;
	switch (h_shell->RxBuffer) {
	//process RETURN key
	case '\r':
		//case '\n':
		size = snprintf (h_shell->print_buffer, BUFFER_SIZE, "\r\n");
		h_shell->drv.transmit(h_shell->print_buffer, size);
		h_shell->cmd_buffer[h_shell->pos++] = 0;     //add \0 char at end of string
		size = snprintf (h_shell->print_buffer, BUFFER_SIZE, ":%s\r\n", h_shell->cmd_buffer);
		h_shell->drv.transmit(h_shell->print_buffer, size);
		h_shell->pos = 0;
		shell_exec(h_shell, h_shell->cmd_buffer);
		h_shell->drv.transmit(prompt, 2);
		//exit read loop
		//reset buffer
		break;
		//backspace
	case '\b':
		if (h_shell->pos > 0) {      //is there a char to delete?
			h_shell->pos--;          //remove it in buffer

			h_shell->drv.transmit(backspace, 3);	// delete the char on the terminal
		}
		break;
		//other characters
	default:
		//only store characters if buffer has space
		if (h_shell->pos < BUFFER_SIZE) {
			h_shell->drv.transmit(&h_shell->RxBuffer, 1);
			h_shell->cmd_buffer[h_shell->pos++] = h_shell->RxBuffer; //store
		}
	}
	return 0;
}
