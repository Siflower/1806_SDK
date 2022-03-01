/*
 * Copyright (c) 2014 Google, Inc
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __serial_sfax8_h
#define __serial_sfax8_h

enum sfax8_type {
	TYPE_SFAX8,
};

/*
 *Information about a serial port
 *
 * @base: Register base address
 * @type: Port type
 * @clock: Input clock rate, used for calculating the baud rate divisor
 * @skip_init: Don't attempt to change port configuration (also means @clock
 * is ignored)
 */
struct sfax8_serial_platdata {
	unsigned long base;
	enum sfax8_type type;
	unsigned int clock;
	bool skip_init;
};

#endif
