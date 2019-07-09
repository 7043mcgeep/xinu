/**
 * @file	pWrite.c
 */

#include <xinu.h>
#include <device.h>
#include <pane.h>

static int writcopy(char *buf, struct pane *ppane, int avail, int len);


devcall pWrite(device *devptr, char *buf, int len) {
	struct pane *ppane = &panetab[devptr->minor];	

	irqmask	im;
	int avail;

	if (len < 0)
		return SYSERR;
	if (len == 0)
		return 0;
	if (ppane == NULL)
		return SYSERR;

	im = disable();		/* disable interrupts */

	avail = semcount(ppane->p_outsem);

	if (avail >= len) {
		writcopy(buf, ppane, avail, len);
		/* Wake output process */
		send(ppane->outprocid, len);
	}
	else {


		if (avail > 0) {
			writcopy(buf, ppane, avail, len);
			/* wake output process */
			send(ppane->outprocid, 0);
			buf += avail;
			len -= avail;
		}
		for ( ; len > 0; len--) {
			wait(ppane->p_outsem);
			ppane->p_outbuf[ppane->p_otail++] = *buf++;
			++ppane->p_ocount;
			if (ppane->p_otail >= ppane->p_olimit) {
				ppane->p_otail = 0;
			}
		}
	}
	restore(im);
	return len;
}

static int writcopy(char *buf, struct pane *ppane, int avail, int len) {
	int index, count;
	char *qhead;
	
	index = ppane->p_otail;
	qhead = &ppane->p_outbuf[index];
	
	for (count = len; count > 0; count--) {
		*qhead++ = *buf++;
		if (++index >= ppane->p_olimit) {
			index = 0;
			qhead = ppane->p_outbuf;
		}
	}

	ppane->p_ocount += len;
	ppane->p_otail = index;
	signal(ppane->p_outsem);	

	return OK;
}
