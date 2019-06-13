/**
 * @file fbPutc.c
 */

/* Embedded Xinu, Copyright (C) 2009, 2013.  All rights reserved. */

#include <stddef.h>
#include <framebuffer.h>
#include <device.h>
#include <pane.h>

extern int rows;
extern int cols;
extern int cursor_col;
extern int cursor_row;
extern ulong foreground;
extern ulong background;
extern bool screen_initialized;

/**
 * @ingroup framebuffer
 *
 * Write a single character to the framebuffer
 * @param  devptr  pointer to framebuffer device
 * @param  ch    character to write
 */
devcall fbPutc(device *devptr, struct pane p,  char ch)
{

	if (pane == NULL) {

		if (screen_initialized) {
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
	}
	else {
		cursor_col = p.xpos;
		cursor_row = p.ypos;

		if (screen_initialized) {
			if (ch == '\n') {
				cursor_row++;
				cursor_col = p.xpos;
			}
			else if (ch == '\t') {
				cursor_col += 4;
			}
			drawChar(ch, cursor_col * CHAR_WIDTH, cursor_row * CHAR_HEIGHT, foreground);
			cursor_col++;
			if (cursor_col == cols - p.xpos) {
				cursor_col = p.xpos;
				cursor_row += 1;
			}
			if ( (minishell == TRUE) && (cursor_row == rows - p.ypos) ) {
				minishellClear(background);
				cursor_row = rows - p.ypos - MINISHELLMINROW;
			}
			else if (cursor_row == rows - p.ypos) {
				screenClear(background);
				cursor_row = p.ypos;
			} 
			return (uchar)ch;
		}
		return SYSERR;
	}
}
