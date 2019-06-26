/**
 * @#file	pPutc
 */

#include <xinu.h>
#include <stddef.h>
#include <framebuffer.h>
#include <conf.h>
#include <device.h>
#include <pane.h>

extern int rows;
extern int cols;
extern int cursor_col;
extern int cursor_row;
extern ulong foreground;
extern ulong background;
extern bool initalized;

/**
 * @ingroup pane
 *
 * Write a single character to the selected pane
 * @param  devptr  pointer to pane device
 * @param  ch    character to write
 */
devcall pPutc(device *devptr, char ch) {
/*
	if (devptr->init == TRUE) {
		if (ch == '\n') {
			cursor_row++;
			cursor_col = 0;
		}
		else if (ch == '\t') {
			cursor_col += 4;
		}
		drawChar(ch, cursor_col * CHAR_WIDTH, cursor_row * CHAR_HEIGHT, foreground);
		cursor_col++;
		if (cursor_col == cols) {
			cursor_col = 0;
			cursor_row += 1;
		}
		if ( (minishell == TRUE) && (cursor_row == rows) ) {
			minishellClear(background);
			cursor_row = rows - MINISHELLMINROW;
		}
		else if (cursor_row == rows) {
			screenClear(background);
			cursor_row = 0;
		}
		return (uchar)ch;
	}
	return SYSERR;
*/
	return pWrite(devptr, ch, 1);


}
