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


int poutproc(struct pane *ppane) {
	int count;
	irqmask im;
	char c;

/*
	drawChar('+', ppane->ul_col, ppane->ul_row, ppane->fg);
	drawChar('+', ppane->ul_col, ppane->lr_row, ppane->fg);
	drawChar('+', ppane->lr_col, ppane->ul_row, ppane->fg);
	drawChar('+', ppane->lr_col, ppane->lr_row, ppane->fg);

	for (count = ppane->ul_col + 1; count < ppane->lr_col; count++) {
		drawChar('-', ppane->ul_col, count, ppane->fg);
		drawChar('-', ppane->lr_col, count, ppane->fg);
	}
	for (count = ppane->ul_row + 1; count < ppane->lr_row; count++) {
		drawChar('|', count, ppane->ul_row, ppane->fg);
		drawChar('|', count, ppane->lr_row, ppane->fg);	}

*/
//	im = disable();

	/* draw pane background */
	int x, y;
	for (x = ppane->ul_col; x < ppane->lr_col; x++) {
		for (y = ppane->ul_row; y < ppane->lr_row; y++) {
			drawPixel(x, y, ppane->bg);
		}
	}


//	im = disable();
//	kprintf("off top");
	while(1) {
		kprintf("at the top");
		/* Draw pane outline */
		drawRect(ppane->ul_col, ppane->ul_row, ppane->lr_col, ppane->lr_row, LEAFGREEN);
	
		/* Draw pane name box outline */
		drawRect(ppane->ul_col, ppane->lr_row - 15, ppane->lr_col, ppane->lr_row, LEAFGREEN);
		
		/* Draw thread name in box */
		char name[6];
		strcpy(name, ppane->devptr->name);
		int i;
		x = ppane->ul_col + 1;
		y = ppane->lr_row - CHAR_HEIGHT;
		for (i = 0; i < sizeof(name)/sizeof(char); i++) {
			drawChar(name[i], x, y, CYAN);
			x += CHAR_WIDTH;
		} 

//		im = disable();
		if ( (count = ppane->p_ocount) > 0) {	/* if there are chars to output */
			c = ppane->p_outbuf[ppane->p_ohead++];
			ppane->p_ocount--;
			if (ppane->p_ohead >= ppane->p_olimit) {
				ppane->p_ohead = 0;
			}

			/* if buffer level below low water mark and no delayed signal waiting proc now */
			if ( (count < (ppane->p_olimit - ppane->p_olowat) ) && (0 == ppane->p_odsend) ) {
				signal(ppane->p_outsem);
//				restore(im);
				kprintf("Hello");
			}
			/* else delay signal until at least OBMINSP buffer slots are free */
			else if ( ++(ppane->p_odsend) == ppane->p_olowat) {
				kprintf("got here");
				while (ppane->p_odsend > 0) {
					kprintf("hola1");
					ppane->p_odsend--;
					signal(ppane->p_outsem);
					kprintf("hola2");
				}
			}
			kprintf("got here 2");
//			restore(im);
		//	kprintf("on middle");
			if ('\n' == c) {
				/*
				for (c = ppane->cursrow; c < ppane->rows; c++) {
					drawChar(' ', ppane->ul_col + ppane->curscol + 1,
						ppane->ul_row + c + 1, ppane->fg);
				}
				*/
				ppane->curscol = ppane->ul_col;
				ppane->cursrow += CHAR_HEIGHT + 2;
				kprintf("newline");
			}
			else {
				drawChar(c, ppane->ul_col + ppane->curscol + 1,
					ppane->ul_row + ppane->cursrow + 1,
					ppane->fg);
				kprintf("regular");
			}
			ppane->curscol += CHAR_WIDTH;
			if (ppane->curscol >= ppane->cols) {
				ppane->curscol = 0;
				ppane->cursrow++;
			}
			
			if (ppane->cursrow >= ppane->rows) {
				ppane->cursrow = 0;
			}
//			im = disable();
//			kprintf("off bottom");
			kprintf("continue");
			continue;
	
		}
//		restore(im);
//		kprintf("on bottom");	
		receive();
		kprintf("received");
	}
}
