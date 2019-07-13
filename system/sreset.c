#include <kernel.h>
#include <thread.h>
#include <semaphore.h>

syscall sreset(semaphore sem, int count) {
	struct sement *sptr;
	int pid;
	irqmask im;

	im = disable();
	if (isbadsem(sem) || count < 0 || semtab[sem].state == SFREE) {
		restore(im);
		return SYSERR;
	}

	sptr = &semtab[sem];
	while (nonempty(sptr->queue)) {
		pid = getfirst(sptr->queue);
		ready(pid, RESCHED_NO);
	}
	sptr->count = count;
	resched();
	restore(im);
	return OK;
}
