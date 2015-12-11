#include <stdio.h>
#include <unistd.h>
#include "events.h"

void proc2(char **events)
{
	int rt, e0, e1;
	printf("proc2 - event_set\n");
	e1 = event_set(events[1]);
	event_check_error_exit(e1, "proc2 - event_set");
	e0 = event_set(events[0]);
	event_check_error_exit(e0, "proc2 - event_set");
	printf("proc2 - event_wait\n");
	rt = event_wait(e0);
	event_check_error_exit(rt, "proc2 - event_wait");
	printf("proc2 - event_throw\n");
	rt = event_throw(e1);
	event_check_error_exit(rt, "proc2 - event_throw");
	printf("proc2 - event_unset\n");
	rt = event_unset(e1);
	event_check_error_exit(rt, "proc2 - event_unset");
	rt = event_unset(e0);
	event_check_error_exit(rt, "proc2 - event_unset");
	printf("proc2 - dead\n");
	exit(0);	
}

void proc3(char **events)
{
	int rt, e1, e2;
	printf("proc3 - event_set\n");
	e2 = event_set(events[2]);
	event_check_error_exit(e2, "proc3 - event_set");
	e1 = event_set(events[1]);
	event_check_error_exit(e1, "proc3 - event_set");
	printf("proc3 - event_wait\n");
	rt = event_wait(e1);
	event_check_error_exit(rt, "proc3 - event_wait");
	printf("proc3 - event_unset\n");
	rt = event_unset(e2);
	event_check_error_exit(rt, "proc3 - event_unset");
	rt = event_unset(e1);
	event_check_error_exit(rt, "proc3 - event_unset");
	printf("proc3 - dead\n");
	exit(0);	
}

void proc4(char **events)
{
	int rt, e2;
	printf("proc4 - event_set\n");
	e2 = event_set(events[2]);
	event_check_error_exit(e2, "proc4 - event_set");
	sleep(3);
	printf("proc4 - event_throw\n");
	rt = event_throw(e2);
	event_check_error_exit(rt, "proc4 - event_throw");
	printf("proc4 - event_unset\n");
	rt = event_unset(e2);
	event_check_error_exit(rt, "proc4 - event_unset");
	printf("proc4 - dead\n");
	exit(0);
}

int main(void)
{
	char *events[] = {"a\0", "b\0", "c\0"};
	pid_t pid;
	int rt, e0, e2;
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
	pid = fork();
	switch (pid) {
	case 0:
		sleep(3);
		proc4(events);
		break;
	default:
		break;
	}
	printf("proc1 - event_set\n");
	e0 = event_set(events[0]);
	event_check_error_exit(e0, "proc1 - event_set");
	e2 = event_set(events[2]);
	event_check_error_exit(e2, "proc1 - event_set");
	sleep(5);
	printf("proc1 - event_wait\n");
	rt = event_wait(e2);
	event_check_error_exit(rt, "proc1 - event_wait");
	printf("proc1 - event_throw\n");
	rt = event_throw(e0);
	event_check_error_exit(rt, "proc1 - event_throw");
	printf("proc1 - event_unset\n");
	rt = event_unset(e0);
	event_check_error_exit(rt, "proc1 - event_unset");	
	rt = event_unset(e2);
	event_check_error_exit(rt, "proc1 - event_unset");
	printf("proc1 - dead\n");
	return 0;
}
