/*
 * Copyright (C) 2009 Renê de Souza Pinto
 * Tempos - Tempos is an Educational and multi purpose Operating System
 *
 * File: i8042.h
 *
 * This file is part of TempOS.
 *
 * TempOS is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * TempOS is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef  CHAR_I8042_H

	#define CHAR_I8042_H

	#include <unistd.h>


	#define KBD_IRQ		   0x01

	#define STATUS_PORT    0x64
	#define KBD_OUT_BUF    0x60
	#define KBD_INPT_BUF   0x60
	#define KBD_CMD_BUF    0x64
	#define KBD_DATA_BUF   0x60
	#define PORT_A         0x60
	#define PORT_B         0x61
	#define OUT_BUF_FULL   0x01
	#define INPT_BUF_FULL  0x02
	#define SYS_FLAG       0x04
	#define CMD_DATA       0x08
	#define KEYBD_INH      0x10
	#define TRANS_TMOUT    0x20
	#define RCV_TMOUT      0x40
	#define PARITY_EVEN    0x80
	#define INH_KEYBOARD   0x10
	#define KBD_ENABLE     0xAE
	#define KBD_DISABLE    0xAD

	#define KB_MENU        0xF1
	#define KB_ENABLE      0xF4
	#define KB_MAKEBREAK   0xF7
	#define KB_ECHO        0xEE
	#define KB_RESET       0xFF
	#define KB_LED_CMD     0xED

	#define KB_SELFTEST    0xAA
	#define KB_ITEST	   0xAB
	#define KB_ACK         0xFA
	#define KB_OVERRUN     0xFF
	#define KB_RESEND      0xFE
	#define KB_BREAK       0xF0
	#define KB_FA          0x10
	#define KB_FE          0x20
	#define KB_PR_LED      0x40


	void init_8042(void);

	void kbc_sendcomm(uchar8_t command);

	uchar8_t kbc_read_from_buffer(void);

	uchar8_t kbc_read(void);

	void wait_read_8042(void);

	void wait_write_8042(void);

	uchar8_t read_scancode(void);

#endif /* CHAR_I8042_H */

