/**
 * @file	xsh_kmux.c
 *
 */

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <tty.h>
#include <string.h>
#include <shell.h>
#include <framebuffer.h>
#include <device.h>
#include <pane.h>

/**
 * Shell command (split). Splits the screen and spawns a new shell window until ended
 * (via 'join' command).
 * @param nargs	 number of arguments in args array
 * @param args   array of arguments
 * @return OK at end
 */


#if FRAMEBUF	/* Don't include if no framebuffer exists */

/* Fixed pane locations */
#define PANE1X	0
#define	PANE1Y	0
#define	PANE2X	(DEFAULT_WIDTH / 2)
#define PANE2Y	0

/* kmux function prototypes */
void new(void);
void split(void);
void startShell(void);
void join(void);			/* new version needs paneid */
void help(void);

/* kmux built-in commands */
struct defaultcommand builtInCommands[] = {
	{"new", new},
	{"split", split},
	{"join", join}
	
};

/* global variables used by xsh_kmux */
int panecnt = 1;	/* number of current panes */


shellcmd xsh_kmux(int nargs, char *args[])
{
	char buf[SHELL_BUFLEN];		/* line input buffer */
	short buflen;			/* length of line inpu */
	char tokbuf[SHELL_BUFLEN + SHELL_MAXTOK];	/* token value buffer */
	short ntok;			/* number of tokens */
	char *tok[SHELL_MAXTOK];	/* pointers to token values */	

	/* Constantly receive and process input */
	while(TRUE) {
		printf("\nkmux$ ");
		/* Receive and parse input */
		control(stdin, TTY_CTRL_CLR_IFLAG, TTY_IRAW, NULL);
		control(stdin, TTY_CTRL_SET_IFLAG, TTY_ECHO, NULL);
		buflen = read(stdin, buf, SHELL_BUFLEN - 1);
		if ( (SYSERR == (ntok = lexan(buf, buflen, &tokbuf[0], &tok[0]))) || ntok <= 0)
		{
			printf("Sorry, command not recognized.");
			continue;
		}
		//type end to break;
		if ( (0 == strcmp(tok[0], "end")) && (ntok == 1) ) break;
		
		//display a help menu that explains available commands
		else if ((0 == strcmp(tok[0], "help")) && (ntok == 1)) {
			printf("help goes here\n");
			continue;
		}

		//split command
		else if ((0 == strcmp(tok[0], "split")) && (ntok == 1)) {
		//	printf("split!\n");
		//	startShell();
			split();
			panecnt++;
			return OK;
		}

		//join command 
		else if ((0 == strcmp(tok[0], "join")) && (ntok == 2)) {
		//	printf("joined!\n");	
			killPane(tok[1]);	//2nd argument is pane NAME
			return OK;
		}
		
		//new command
		else if ((0 == strcmp(tok[0], "new")) && (ntok == 1)) {
			new();
			return OK;
		}

		//status command
		else if ((0 == strcmp(tok[0], "status")) && (ntok == 1)) {
			int i;
			printf("ID\t|\tNAME\t|\tSTATE\n");
			for (i = 0; i < MAXPANES; i++) {
				printf("%d\t\t%s\t\t%d\n", panetab[i].id, panetab[i].name, panetab[i].state);
			}
		}

		return OK;

	}
}

void new() {
	spawnFrame();
}

void split() {
	spawnPane();
}

void startShell() {
	if (panecnt < MAXPANES) {
		switch(panecnt) {

			case 0:		ready(create(shell, INITSTK, INITPRIO, "PANE1", 3, TTY1, TTY1, TTY1), RESCHED_NO); 
				//	screenInit();
					panecnt++;
					while(1);
					break;
		
			case 1: 	ready(create(shell, INITSTK, INITPRIO, "PANE2", 3, TTY1, TTY1, TTY1), RESCHED_NO);
				//	screenInit();
					panecnt++;
					while(1);
					break;

			case 2: 	ready(create(shell, INITSTK, INITPRIO, "PANE3", 3, TTY1, TTY1, TTY1), RESCHED_NO);
				//	screenInit(); 
					panecnt++;
					while(1);
					break;

			case 3:		ready(create(shell, INITSTK, INITPRIO, "PANE4", 3, TTY1, TTY1, TTY1), RESCHED_NO);
				//	screenInit();
					panecnt++;
					while(1);
					break;
		}
	}
	else {
		printf("Maximum number of panes spawned!\n");
	}	



//	ready(create((void *)shell, INITSTK, INITPRIO, "SHELL2", 3, CONSOLE, CONSOLE, CONSOLE), RESCHED_YES);
//	open(TTY1, KBDMON0);
//	screenInit();
}


void join() {	/* new version needs paneid */
	struct thrent *thrptr;	/* pointer to thread entry */
	int i;
/*
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if (strcmp(thrptr->name, "SHELL2") == 0) {
			kill(i)
		}
	} */

/*
	for (i = 0; i < NTHREAD; i++) {
		thrptr = &thrtab[i];
		if (strcmp(thrptr->name, "SHELL2") == 0) {
			kill(i);
			panecnt--;
			return;
		} else if (strcmp(thrptr->name, "SHELL3") == 0) {
			kill(i);
			panecnt--;
			return;
		} else if (strcmp(thrptr->name, "SHELL4") == 0) {
			kill(i);
			panecnt--;
			return;
		} else if (strcmp(thrptr->name, "SHELL5") == 0) {
			kill(i);
			panecnt--;
			return;
		}
	}
	printf("Unable to destroy SHELL process!\n");
*/
}


#endif /* FRAMEBUF */
