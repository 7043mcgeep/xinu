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


	im = disable();
	while(1) {
		if ( (count = ppane->p_ocount) > 0) {	/* if there are chars to output */
			c = ppane->p_outbuf[ppane->p_ohead++];
			ppane->p_ocount--;
			if (ppane->p_ohead >= ppane->p_olimit) {
				ppane->p_ohead = 0;
			}

			/* if buffer level below low water mark and no delayed signal waiting proc now */
			if ( (count < (ppane->p_olimit - ppane->p_olowat) ) && (0 == ppane->p_odsend) ) {
				signal(ppane->p_outsem);
			}
			/* else delay signal untilk at least OBMINSP buffer slots are free */
			else if ( ++(ppane->p_odsend) == ppane->p_olowat) {
				while (ppane->p_odsend > 0) {
					ppane->p_odsend--;
					signal(ppane->p_outsem);
				}
			}
			restore(im);
			if ('\n' == c) {
				for (c = ppane->curscol; c < ppane->cols; c++) {
					drawChar(' ', ppane->ul_row + ppane->cursrow + 1,
						ppane->ul_col + c + 1, ppane->fg);
				}
			}
			else {
				drawChar(c, ppane->ul_row + ppane->cursrow + 1,
					ppane->ul_col + ppane->curscol + 1,
					ppane->fg);
			}
			ppane->curscol++;
			if (ppane->curscol >= ppane->cols) {
				ppane->curscol = 0;
				ppane->cursrow++;
			}
			
			if (ppane->cursrow >= ppane->rows) {
				ppane->cursrow = 0;
			}
			im = disable();
			continue;
	
		}	
		receive();
	}
}
