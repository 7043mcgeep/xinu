/**
 * @file	poutproc.c
 */

#include <xinu.h>
#include <device.h>
#include <pane.h>
#include <framebuffer.h>
#include <stdio.h>


/*-------------------------------------------------------------------
 * poutproc - lower-half device driver process for pane pseudo-device
 *-------------------------------------------------------------------
 */

void clearBackground(struct pane *ppane);
void drawOutline(struct pane *ppane, int color);
void drawNameBox(struct pane *ppane, int color);
void drawName(struct pane *ppane);
void paneClear(struct pane *ppane, ulong color);
void updateCursor(struct pane *ppane);

extern int cursor_col;
extern int cursor_row;

int boxcolor;

int poutproc(struct pane *ppane) {
	int count;
	irqmask im;
	char c;
	
	/* draw pane background */
	
	if (ppane->hasFocus) {
		boxcolor = RASPBERRY;
	} else {
		boxcolor = LEAFGREEN;
	}

	clearBackground(ppane);
	drawOutline(ppane, boxcolor);
	drawNameBox(ppane, boxcolor);
	drawName(ppane);
	
	while (1) {
		if (ppane->hasFocus)
			boxcolor = RASPBERRY;
		else
			boxcolor = LEAFGREEN;



//		updateCursor(ppane);

		if ( (count = ppane->p_ocount) > 0) {	/* if there are chars to output */
			im = disable();
			c = ppane->p_outbuf[ppane->p_ohead++];
			ppane->p_ocount--;
			if (ppane->p_ohead >= ppane->p_olimit) {
				ppane->p_ohead = 0;
			}

			/* if buffer level below low water mark and no delayed signal waiting proc now */
			if ( (count < (ppane->p_olimit - ppane->p_olowat) ) && (0 == ppane->p_odsend) ) {
//				kprintf("outsem: %d\r\n", semcount(ppane->p_outsem));
				signal(ppane->p_outsem);
			}
			/* else delay signal until at least OBMINSP buffer slots are free */
			else if ( ++(ppane->p_odsend) == ppane->p_olowat) {
				while (ppane->p_odsend > 0) {
					ppane->p_odsend--;
//					kprintf("outsem: %d\r\n", semcount(ppane->p_outsem));
					signal(ppane->p_outsem);
				}
			}
			restore(im);
			if ('\n' == c) {
			//	kprintf("outproc new line!\r\n");
				updateCursor(ppane);
				ppane->cursrow += CHAR_HEIGHT + 2;
				ppane->curscol = ppane->ul_col;

//				updateCursor(ppane);
			}
			else if ('\r' == c) {
			//	kprintf("outproc carriage return!\r\n");
				ppane->curscol = ppane->ul_col;
				ppane->cursrow += CHAR_HEIGHT + 2;

//				updateCursor(ppane);
			}
			else {
				drawChar(c, ppane->ul_col + ppane->curscol + 1,
					ppane->ul_row + ppane->cursrow + 1,
					ppane->fg);
			}
//			ppane->curscol += CHAR_WIDTH;
			ppane->curscol += CHAR_WIDTH;

//			updateCursor(ppane);
			if (ppane->curscol >= ppane->cols) {
				ppane->curscol = 0;
				ppane->cursrow++;

//				updateCursor(ppane);
			}
			
			if (ppane->cursrow >= ppane->rows - 15 - CHAR_HEIGHT) {
				clearBackground(ppane);
				drawOutline(ppane, boxcolor);
				drawNameBox(ppane, boxcolor);
				drawName(ppane);
			//	paneClear(ppane, BLACK);
				ppane->cursrow = 0;

//				updateCursor(ppane);
			}
			
			continue;
	
		}
		bzero(ppane->p_outbuf, sizeof(ppane->p_outbuf)/sizeof(char));
		receive();
	}
}

void drawOutline(struct pane *ppane, int color) {
	drawRect(ppane->ul_col, ppane->ul_row, ppane->lr_col, ppane->lr_row, color);
}

void drawNameBox(struct pane *ppane, int color) {
	drawRect(ppane->ul_col, ppane->lr_row - 15, ppane->lr_col, ppane->lr_row, color);
}

void drawName(struct pane *ppane) {
	char name[6];
	strcpy(name, ppane->devptr->name);
	int i, x, y;
	x = ppane->ul_col + 1;
	y = ppane->lr_row - CHAR_HEIGHT;
	for (i = 0; i < sizeof(name)/sizeof(char); i++) {
		drawChar(name[i], x, y, CYAN);
		x += CHAR_WIDTH;
	} 
}

void clearBackground(struct pane *ppane) {
	int x, y; 

	drawOutline(ppane, boxcolor);
	drawNameBox(ppane, boxcolor);
	drawName(ppane);


	for (x = ppane->ul_col + 1; x < ppane->lr_col - 1; x++) {
		for (y = ppane->ul_row + 1; y < ppane->lr_row - 15; y++) {
			drawPixel(x, y, ppane->bg);
		}
	}

/*
	for (y = ppane->ul_row + 1; y < ppane->lr_row - 15; y++) {
		for (x = ppane->ul_col + 1; x < ppane->lr_col - 1; x++) {
			drawPixel(x, y, ppane->bg);
		}
	}
*/

}

void paneClear(struct pane *ppane, ulong color) {
	ulong *paneAddr = (ulong *)(framebufferAddress + ppane->ul_row);

	ulong *address = paneAddr;
	ulong *maxaddress = (ulong *)(paneAddr + (ppane->lr_row * pitch) + (ppane->lr_col * (BIT_DEPTH / 8)));
	while (address != maxaddress) {
		*address = color;
		address++;
	}
	ppane->cursrow = 0;
	ppane->curscol = 0;

}

void updateCursor(struct pane *ppane) {
	cursor_row = ppane->cursrow;
	cursor_col = ppane->curscol;
}
