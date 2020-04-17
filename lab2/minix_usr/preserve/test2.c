#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <math.h>
/*#include "test.h"*/
int getpri(int pid){
	message m;
	m.m1_i1 = pid;
	return _syscall(MM, GETPRI, &m);
}

void setpri(int pid, int group){
	message m;
	m.m1_i1 = pid;
	m.m1_i2 = group;
	_syscall(MM, SETPRI, &m);
}

int main(int argc, char* argv[]){

	clock_t b, e;
	double result;
	int i, j, group;
	group = atoi(argv[1]);
	setpri(getpid(), group);


	b = clock();
	for(i = j = 0; i < 2000; i++){
		/*j+=160;*/
		sqrt(i);
	}
	e = clock();
	result = ((double)(e - b))/CLOCKS_PER_SEC;

	group = getpri(getpid());
	printf("pid: %d\nprio: %d\ntime: %fs\n", getpid(),group, result);

	return 0;
}
