/**
 * @file	pOpen.c
 */

#include <xinu.h>
#include <conf.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <tty.h>
#include <string.h>
#include <shell.h>
#include <device.h>
#include <framebuffer.h>
#include <pane.h>

extern struct pane *palloc();
extern int poutproc(struct pane *ppane);
//extern int console_rows;
//extern int console_cols;


devcall pOpen(device *devptr, va_list ap) {
	int ulrow, ulcol, lrrow, lrcol, fg, bg;
	int outpid = -1;
	int dvnum = KBDMON0;
	


	struct pane *ppane = NULL;
	
	ulrow = va_arg (ap, int);
	ulcol = va_arg (ap, int);
	lrrow = va_arg (ap, int);
	lrcol = va_arg (ap, int);
	fg    = va_arg (ap, int);
	bg    = va_arg (ap, int);	


/*
	if ((ulrow < 0) || (ulrow >= console_rows)
		|| (lrrow < 0) || (lrrow >= console_rows)
		|| (ulcol < 0) || (ulcol >= console_cols)
		|| (lrcol < 0) || (lrcol >= console_cols))
	{
			return SYSERR;
	}

	if ((ulrow >= lrrow) || (ulcol >= lrcol)) {
			return SYSERR;
	}
*/

	ppane = palloc();	/* allocate new pane from table */
	if ((struct pane *)SYSERR == ppane)
		return SYSERR;
	
//	devptr = (device *)ppane;	/* dev table entry points to pane */

//	devptr = (device *)ppane;
//	devtab[devptr->minor] = ppane;
	
//	devtab[devptr->minor].devptr = devptr;
//	devptr = (device *)ppane;

	ppane->devptr	= devptr;
	ppane->ul_row	= ulrow;
	ppane->ul_col	= ulcol;
	ppane->lr_row	= lrrow;
	ppane->lr_col	= lrcol;

	ppane->rows 	= lrrow - ulrow - 1;
	ppane->cols	= lrcol - ulcol - 1;

	ppane->cursrow	= 0;
	ppane->curscol	= 0;

	ppane->fg	= fg;
	ppane->bg	= bg;

	

	/* initialize input buffer */
	ppane->istart = 0;
	ppane->icount = 0;
	ppane->idelim = FALSE;

	/* initalize input flags */
	ppane->iflags = TTY_ICRNL;
	ppane->ieof = FALSE;
	
	ppane->phw = (device *)&devtab[dvnum];






	ppane->p_olimit = ppane->cols;	/* buffer output limit is the length of one line of the pane */ 
	ppane->p_outsem = semcreate(ppane->p_olimit);


	

	
	if ((semaphore)SYSERR == ppane->p_outsem) {
		return SYSERR;
	}

	ppane->p_ohead	= 0;
	ppane->p_otail	= 0;
	ppane->p_ocount	= 0;
	ppane->p_odsend = 0;
	ppane->p_olowat = ppane->p_olimit / 8;
	ppane->p_outbuf = (char *)memget(ppane->p_olimit);

	if ((unsigned char *)SYSERR == ppane->p_outbuf) {
		return SYSERR;
	}

//	devptr = (device *)ppane;

	outpid = create(poutproc, INITSTK, PANEPRIO, "*PANEOUT*", 1, ppane);
	
	if (SYSERR == outpid) {
		kprintf("Cannot create pane output process.\n");
	} else {
		ppane->outprocid = outpid;
		ready(outpid, RESCHED_NO);
	}
	
	devptr = (device *)ppane;
	return ppane;

}
