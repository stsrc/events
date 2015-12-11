#include <stdio.h>
#include <unistd.h>
#include "events.h"
/*
 * TEST SCENARIO:
 * proc2 throws b, waits first for a
 * proc3 throws c, waits second for b
 * proc1 throws a, tries to wait for c
 */
void proc2(char **events)
{
	int e0, e1, rt;
	printf("proc2 - event_set\n");
	e0 = event_set(events[0]);
	e1 = event_set(events[1]);
	event_check_error_exit(e0, "proc2 - event_set");
	event_check_error_exit(e1, "proc2 - event_set");
	printf("proc2 - event_wait\n");
	rt = event_wait(e0);
	event_check_error_exit(rt, "proc2 - event_wait");
	rt = event_throw(e1);
	event_check_error_exit(rt, "proc2 - event_throw");
	e1 = event_unset(e1);
	event_check_error_exit(e1, "proc2 - event_unset");
	e0 = event_unset(e0);
	event_check_error_exit(e0, "proc2 - event_unset");
	printf("proc2 exits\n");
	exit(0);	
}

void proc3(char **events)
{
	int rt, e1, e2;
	printf("proc3 - event_set\n");
	e2 = event_set(events[2]);
	e1 = event_set(events[1]);
	event_check_error_exit(e1, "proc3 - event_set");
	event_check_error_exit(e2, "proc3 - event_set");
	printf("proc3 - event_wait\n");
	rt = event_wait(e1);
	event_check_error_exit(rt, "proc3 - event_wait");
	rt = event_unset(e2);
	event_check_error_exit(rt, "proc3 - event_unset");
	rt = event_unset(e1);
	event_check_error_exit(rt, "proc3 - event_unset");
	printf("proc3 exits\n");
	exit(0);	
}

int main(void)
{
	char *events[] = {"a\0", "b\0", "c\0"};
	pid_t pid;
	int e0, e1, e2, rt;
	pid = fork();
	switch(pid) {
	case 0:
		sleep(1);
		proc2(events);
		break;
	default:
		break;
	}
	pid = fork();
	switch (pid) {
	case 0:
		sleep(2);
		proc3(events);
		break;
	default:
		break;
	}
	printf("proc1 - event_set\n");
	e0 = event_set(events[0]);
	event_check_error_exit(e0, "proc1 - event_set");
	e1 = event_set(events[1]);
	event_check_error_exit(e1, "proc1 - event_set");
	e2 = event_set(events[2]);
	sleep(5);
	printf("proc1 - event_wait - powinien pojawic sie blad\n");
	rt = event_wait(e2);
	event_check_error(rt, "proc1 - event_wait. Blad prawidlowy.");
	rt = event_throw(e0);
	event_check_error_exit(rt, "proc1 - event_throw");
	rt = event_unset(e0);
	event_check_error_exit(rt, "proc1 - event_unset");
	rt = event_unset(e1);
	event_check_error_exit(rt, "proc1 - event_unset");
	rt = event_unset(e2);
	event_check_error_exit(rt, "proc1 - event_unset");
	printf("proc1 exits\n");
	return 0;
}
