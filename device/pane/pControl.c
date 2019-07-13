/**
 * @file	pControl.c
 *
 * Control function for PANE pseudo devices
 */

#include <xinu.h>
#include <pane.h>
#include <device.h>


devcall pControl(device *devptr, int func, long arg1, long arg2) {
	struct pane *ppane;
	device *phw;
	char old;

	/* Setup and error check pointers to structures */
	ppane = &panetab[devptr->minor];
	phw = ppane->phw;
	if (NULL == phw) {
		return SYSERR;
	}

	switch (func) {
		/* Set input flags: arg1 = flags to set		*/
		/* return old value of flags			*/
		case PANE_CTRL_SET_IFLAG:
			old = ppane->iflags & arg1;
			ppane->iflags |= (arg1);
			return old;
	
		/* Clear input flags: arg1 = flags to clear	*/
		/* return old value of flags			*/
		case PANE_CTRL_CLR_IFLAG:
			old = ppane->iflags &arg1;
			ppane->iflags &= ~(arg1);
			return old;
		
	}

	return SYSERR;
}
