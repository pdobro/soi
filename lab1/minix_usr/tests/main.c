#include </usr/include/lib.h>
#include </usr/include/stdio.h>
#include <stdio.h>
#include <stdlib.h>
int getprocnr(int id_num)
{
  message msg;
  msg.m1_i1 = id_num;
  return (_syscall(0,78,&msg));
}

int main (int argc, char* argv[]) 
{
  int res, i;
  if(argc == 1) 
	printf("need process id\n");
  else{
	int start = atoi(argv[1]); 
	for(i=0;i<10;i++)
 	 {
    		res = getprocnr(start + i);
    		if(res !=-1) 
			printf("find process with pid: %d index: %d\n",start+i,res);
    		else 
			printf("cannot find process %d . errno: %d\n",start+i,errno); }
	}
  return 0;
}
