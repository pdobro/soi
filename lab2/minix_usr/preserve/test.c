#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>
#include <limits.h>

int getpri(int pid){
	message m;
	m.m1_i1 = pid;
	printf("PID: %d \n", pid);
	return _syscall(MM, GETPRI, &m);
}

void setpri(int pid, int group){
	message m;
	m.m1_i1 = pid;
	m.m1_i2 = group;
	_syscall(MM, SETPRI, &m);
}

int main(int argc, char* argv[]){

	int group;
	int newgroup;
	int i;
	setpri(19, 3);
	group = getpri(19);
	printf("wynik: %d", group);
	

	return 0;
}
