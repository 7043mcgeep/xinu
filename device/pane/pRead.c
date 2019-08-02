/**
 * @file pRead.c
 *
 */

#include <stddef.h>
#include <ctype.h>
#include <device.h>
#include <pane.h>

static void paneEcho(device *, char);

devcall pRead(device *devptr, void *buf, int len) {
	struct pane *ppane;
	device *phw;
	int ch = 0;
	int count = 0;
	char *buffer = buf;

	
	/* setup and error check pointers to structures */
	ppane = &panetab[devptr->minor];
	phw = ppane->phw;
	
	if (NULL == phw) {
		return SYSERR;
	}

//	if (ppane->hasFocus) {
		/* if the eof flag is set, clear the flag and return EOF */
		if (ppane->ieof) {
			ppane->ieof = FALSE;
			return EOF;
		}
		
		/* in raw mode, no line buffering and no cooking */
		if (ppane->iflags & PANE_IRAW) {
			/* fill user buffer first from input buffer */
			while ((0 < ppane->icount) && (count < len)) {
				*buffer++ = ppane->in[ppane->istart];
				ppane->icount--;
				ppane->istart = (ppane->istart + 1) % PANE_IBLEN;
				count++;
			}

			/* fill rest of user buffer by reading input */
			while (count < len) {
				ch = (*phw->getc) (phw);
				if (SYSERR == ch) {
					return SYSERR;
				}
				*buffer++ = ch;
				count++;

				paneEcho(devptr, ch);

				/* echo character if PANE_ECHO flag is set */
				// if (ppane->iflags & PANE_ECHO) {
				// 	paneEcho(devptr, ch);
				// }
			}

			return count;
		}

		/* in non-raw mode, read characters into the PANE input buffer */
		/* until a line delimiter is read or the PANE input buffer is full */
		while ((ppane->icount < PANE_IBLEN) && !ppane->idelim) {
			/* read character */
			ch = (*phw->getc) (phw);


	//		kprintf("read: %c\r\n", ch);
			if (SYSERR == ch) {
				return SYSERR;
			}		

			/* cook special characters */
			switch (ch) {
				/* backspace or delete */
				case '\b':
				case 0x7F:
					if (ppane->icount < 1) {
						continue;
					}
					ppane->icount--;
					break;
				/* newline */
				case '\n':
					/* translate NL to CR if INLCR flag is set */
					if (ppane->iflags & PANE_INLCR) {
						ch = '\r';
					}
					/* place character in PANE input buffer */
					ppane->in[(ppane->istart + ppane->icount) % PANE_IBLEN] = ch;
					ppane->icount++;
					ppane->idelim = TRUE;
					break;
				/* carriage return */
				case '\r':
					/* ignore carriage return if IGNCR flag is set */
					if (ppane->iflags & PANE_IGNCR) {
						continue;
					}
					/* translate CR to NL if ICRNL flag is set */
					if (ppane->iflags & PANE_ICRNL) {
						ch = '\n';
					}
					/* place character in PANE input buffer */
					ppane->in[(ppane->istart + ppane->icount) % PANE_IBLEN] = ch;
					ppane->icount++;
					ppane->idelim = TRUE;
					break;
				/* end of file */
				case 0x04:
					ppane->ieof = TRUE;
					ppane->idelim = TRUE;
					break;			

				/* all other characters */
				default:
					/* ignore unprintable characters */
					if (!isprint(ch)) {
						continue;
					}
					/* place character in PANE input buffer */
					ppane->in[(ppane->istart + ppane->icount) % PANE_IBLEN] = ch;
					ppane->icount++;
					break;
			}	

			/* echo character if PANE_ECHO flag is set */
			if (ppane->iflags & PANE_ECHO) {
				paneEcho(devptr, ch);
			}

			if (ppane->iflags & PANE_IRAW) {
				break;
			}
		}

		/* fill user buffer from input buffer */

		while ((0 < ppane->icount) && (count < len)) {
			*buffer++ = ppane->in[ppane->istart];
			ppane->icount--;
			ppane->istart = (ppane->istart + 1) % PANE_IBLEN;
			count++;
		}

		/* if PANE input buffer is empty, clear idelimiter flag */
		if (0 == ppane->icount) {
			ppane->idelim = FALSE;
		}

		/* if nothing was read, and the eof flag was set, return EOF */
		if ((0 == count) && (ppane->ieof)) {
			ppane->ieof = FALSE;
			return EOF;
		}

		return count;
//	}
	
}

/**
 * Echo a single character on a PANE.
 * @param devptr PANE device table entry
 * @param ch character to echo
 */

static void paneEcho(device *devptr, char ch) {
	/* backspace or delete */
	if (('\b' == ch) || (0x7F == ch)) {
		pPutc(devptr, '\b');
		pPutc(devptr, ' ');
		pPutc(devptr, '\b');
		return;
	}

	/* do not echo unprintable characters */
	// if (!isprint(ch)) {
	// 	return;
	// }

	/* echo character */
	pPutc(devptr, ch);
}
