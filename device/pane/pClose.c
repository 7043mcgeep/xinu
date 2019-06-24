/**
 * @file	pClose.c
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
#include <pane.h>
#include <framebuffer.h>

devcall pClose(device *devptr) {
	struct pane *ppane = NULL;
	int outpid = -1;

	ppane = (struct pane *)devptr;
	ppane->devptr = 0;
	semfree(ppane->p_outsem);
	ppane->p_outsem = -1;
	memfree(ppane->p_outbuf, sizeof(ppane->p_outbuf));
	kill(ppane->outprocid);
	pFree(ppane);
	devptr = (char *)NULL;
	return OK;
}
