/**
 * @file pGetc.c
 *
 */

#include <stddef.h>
#include <ctype.h>
#include <device.h>
#include <thread.h>
#include <pane.h>

devcall pGetc(device *devptr) {
	char ch;
	int result = NULL;
	struct pane *ppane = &devtab[devptr->minor];


	result = pRead(ppane, &ch, 1);


	if (1 != result) {
		return result;
	}


	return ch;

}
