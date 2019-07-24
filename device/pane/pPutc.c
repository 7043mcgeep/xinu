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
	struct pane *ppane = NULL;
	device *phw = NULL;

	/* Setup and error check pointers to structures */
	ppane = &panetab[devptr->minor];
	phw = ppane->phw;
	if (NULL == phw) {
		return SYSERR;
	}

/*
	switch (ch) {
		// Newline 
		case '\n':
			if (SYSERR == (*phw->putc) (phw, '\n')) {
				return SYSERR;
			}
			kprintf("pPutc: newline!\r\n");	
			break;
		// Carriage return 
		case '\r':
			ch = '\r';
			kprintf("pPutc: carriage return!\r\n");
			break;
	}
*/
//	return (*phw->putc) (phw, ch);
//	kprintf("pPutc: %c\r\n", ch);	
	return pWrite(devptr, &ch, 1);


}
