/**
 * @file pGetc.c
 *
 */

#include <stddef.h>
#include <ctype.h>
#include <device.h>
#include <pane.h>

devcall pGetc(device *devptr) {
	char ch;
	int result = NULL;

	result = pRead(devptr, &ch, 1);

	if (1 != result) {
		return result;
	}

	return ch;

}
