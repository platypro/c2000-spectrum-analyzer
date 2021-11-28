/*
 * display.h
 *
 *  Created on: Nov. 27, 2021
 *      Author: platypro
 */

#ifndef SRC_DISPLAY_H_
#define SRC_DISPLAY_H_

#include <xdc/std.h>

void display_init();
void display_update();

void display_begin();
void display_write8(Uint32 value);
void display_write16(Uint32 value);
void display_write24(Uint32 value);
void display_cmd(Uint32 cmd);

void display_write(Uint16* data, size_t length);

#endif /* SRC_DISPLAY_H_ */
