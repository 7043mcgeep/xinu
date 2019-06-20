#include <kernel.h>
#include <device.h>
#include <pane.h>
#include <stdio.h>

int pFree(struct pane *ppane) {
	if (PANE_FREE == ppane->state) {
		return SYSERR;
	}

	ppane->state = PANE_FREE;
	return OK;
}
