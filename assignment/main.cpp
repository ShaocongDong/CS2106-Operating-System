#include <stdio.h>
#include "kernel.h"

#define MISS_DEADLINE		1
int main()
{
	initOS();

#if SCHEDULER_TYPE == 0
	addProcess(15);
	addProcess(106);
	addProcess(109);
	addProcess(139);
	addProcess(109);
	addProcess(15);
	addProcess(139);
	addProcess(109);
#elif SCHEDULER_TYPE == 1

#if MISS_DEADLINE==0
	addProcess(4, 1);
	addProcess(8, 2);
	addProcess(12, 3);
#elif MISS_DEADLINE==1
	addProcess(3, 1);
	addProcess(6, 2);
	addProcess(8, 3);
#endif
#endif
	startOS();
}
