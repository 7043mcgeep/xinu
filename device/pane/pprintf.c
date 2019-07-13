/**
 * @file pprintf.c
 */

#include <stddef.h>
#include <stdarg.h>
#include <device.h>
#include <stdio.h>
#include <pane.h>

syscall pputc(char c, device *devptr) {
	pPutc(devptr, c);
	return c;
}

syscall pgetc(device *devptr) {
	return SYSERR;
}

syscall pprintf(char *fmt, ...) {
//	struct pane *ppane;
//	ppane = &panetab[devptr->minor];
	va_list ap;
	irqmask mask = disable();

	va_start(ap, fmt);
//	_doprnt(fmt, ap, (int (*)(int, int))pputc, (int)&devtab[ppane->devptr]);
	_doprnt(fmt, ap, (int (*)(int, int))pputc, (int)&devtab[PANE1]);
	va_end(ap);

	restore(mask);
	return OK;
}
