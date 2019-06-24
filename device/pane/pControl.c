/**
 * @file	pControl.c
 *
 * Control function for PANE pseudo devices
 */

#include <xinu.h>
#include <pane.h>
#include <device.h>


devcall pControl(device *devptr, int func, char *arg1, char *arg2) {
	struct pane *ppane;	//= (struct win *)pdev->dvioblk;
	char ch;
	
	switch (func) {
	default:
		return OK;
	}
	return SYSERR;

}
