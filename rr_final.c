#include <stdio.h>

/*
Structure P contains the information of a process, 
including process name, arrival time, burst time, start time, finish time,
response time, waiting time, turn-around time, and remaining burst time, respectively.

The pointers *print and *set point to the printP(used to print the information of a process) 
and setP function(used to calculate its rt, wt and tat). 
*/
struct P {
	int pn, arr, bur, star, finish, tat, wt, rt, rb;
	void (*print)(const struct P*);
	void (*set)(struct P*);
};

/*
Structure ReadyQueque contains an array of processes and its size(the number of processes in the array).
*/
struct ReadyQueue {
    int size;
    struct P processes[10];
};

/* The init() function to initialize the ready queue with size = 0.*/
void init(struct ReadyQueue *ready) {
    ready->size=0;
}

/* The setP() function is used to calculate rt, wt and tat of a process p1.*/
void setP(struct P *p1) {
	p1->rt=p1->star-p1->arr;
	p1->tat=p1->finish-p1->arr;
	p1->wt=p1->tat-p1->bur;
}

/* The printP() function is used to print the information of a process p1.*/
void printP(const struct P *p1) {
	printf("%d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\n", p1->pn, p1->arr, p1->bur, p1->star, p1->finish, p1->rt, p1->wt, p1->tat);
}

/* 
The SortArr() function is used to sort a process array plist[] with n elements 
in ascending order by arrival time.
*/
void SortArr(struct P plist[], int n) {
	int i, j;
	for(i=0; i<n; i++) {
		for(j=i+1; j<n; j++) {
			if(plist[i].arr>plist[j].arr) {
				struct P P0;
                P0=plist[i];
                plist[i]=plist[j];
                plist[j]=P0;		
			}
		}
	}
}

/* The pushP() function pushes a process into the ready queue at the very back of the queue.*/
void pushP(struct ReadyQueue *ready, struct P p1) {
    ready->processes[ready->size++]=p1;
}

/* 
The popP() function pops the first process, which is done running, out of the ready queue.
The next process in the queue becomes the first one and starts running.
*/
void popP(struct ReadyQueue *ready) {
    for(int i=0; i<ready->size-1; i++)
        ready->processes[i]=ready->processes[i+1];
    ready->size--;
}

/* 
The moveP() function moves the first process, which is running but the quantum time runs out, to the very back of the ready queue.
The next process in the queue becomes the first one and starts running.
*/
void moveP(struct ReadyQueue *ready) {
    struct P P0;
    P0=ready->processes[0];
    for(int i=0; i<ready->size-1; i++)
        ready->processes[i]=ready->processes[i+1];
    ready->processes[ready->size-1]=P0;
}

int main() {

    /* Define a process array, called plist[]. */
    struct P plist[10];

    /* Define and initialize a ready queue. */
	struct ReadyQueue ready;
    init(&ready);

    /* Define a terminated array, which will contains all the done running processes. */
    struct P term[10];

	int i, n;

    /* Total waiting time, total turn-around time, total response time of the processes. */
	int totwt=0, tottat=0, totrt=0;

    /*** GET THE INPUT ***/
	/* Input of a process includes process name, arrival time and burst time. */
    printf("Enter the number of processes: ");
	scanf("%d", &n);
	for(i=0; i<n; i++) {
		printf("Enter the Process Name, Arrival Time & Burst Time: ");
		scanf("%d%d%d", &plist[i].pn, &plist[i].arr, &plist[i].bur);
		plist[i].print=printP;
		plist[i].set=setP;
        /* rb has inital value the same as burst time value. */
        plist[i].rb=plist[i].bur;
	}

    /* Sort the input in ascending order by arrival time. */
	SortArr(plist, n);
    /* plist[] now contains the processes with increasing arrival time. */

    /* Get the quantum time. */
    int q;
	printf("Enter the quantum time: ");
	scanf("%d", &q);
	
    /*
	Push the process which arrives earliest into the ready queue.

	Use the while loop to push all the processes having the same smallest arrival time
	into ready queue. 
	*/
    i=0;
	while(plist[i].arr==plist[0].arr) {
        pushP(&ready, plist[i]);
        i++;
    }

    /*** THE READY QUEUE IS SUPPOSED TO BE A PROCESS ARRAY 
	AND THE INDEX 0 OF THE ARRAY IS ALWAYS THE RUNNING PROCESS. ***/

	/* The variable current_time simulates how the ready queue works in real time.
	current_time has initial value of arrival time of the first running process. */
    int current_time = ready.processes[0].arr;

    /* exe_time calculates execute time of the running process, guaranteeing it won't exceed quantum time. */
    int exe_time;

    /* pi indexes to the processes in plist[] that haven't been pushed to ready queue yet. */
    int pi=i; 

    /* A process when popped out of ready queue will be added to term[] array at the index i, starting at 0. */
    i=0; 

    /* Keep doing these handling works until all the processes are added to term[]. */
    while (i<n) {

        /* Initialize value of exe_time for each time a process running. */
        exe_time=1;

        /* If a process runs for the first time, its start time is current_time. */
        if (ready.processes[0].bur == ready.processes[0].rb)
			ready.processes[0].star = current_time;

        /* Do these until the exe_time of running process exceeds quantum time. */
        while(exe_time<=q) {
            /* When a time unit passes, remaining burst time of running process is reduced by 1 
            and its exe_time increases by 1. */
            current_time++;
            ready.processes[0].rb--;
            exe_time++;


            /* Push all the processes arriving at current_time to ready queue. 
		    Only do this if there's any process in plist[] not been pushed into ready queue yet. */
            while(pi<n) {
                if(plist[pi].arr==current_time) {
                    pushP(&ready, plist[pi]);
                    pi++;
                }
                else break;
            }

            /* Get out of the loop when the running process has done running, even if it still has time to execute. */
            if(ready.processes[0].rb==0) break;
        }

        /* If remaining burst time of the running process equals 0, it finishes at current_time. 
		So, we calculate its rt, wt, and tat via set function.
		Then add it to term[] at the index i (increase i by 1 after that), and finally pop it out of the queue. */
        if(ready.processes[0].rb==0) {
            ready.processes[0].finish=current_time;
            ready.processes[0].set(&ready.processes[0]);
            term[i++]=ready.processes[0];
            popP(&ready);
        }
        /* If it's not yet, move it to the back of the queue and keep waiting for its turn to take CPU. */
        else moveP(&ready);
    }

    /* Print the information of the terminated processes by their finishing order. 
	Calculate the total numbers. */
    printf("\nPName\tArrtime\tBurtime\tStart\tFinish\tRT\tWT\tTAT\n");
	for(i=0; i<n; i++) {
		term[i].print(&term[i]);
		totwt+=term[i].wt;
		tottat+=term[i].tat;
        totrt+=term[i].rt;
	}

    /* Calculate the average numbers and print them. */
	float avewt, avetat, avert;
	avewt=(float) totwt/n;
	avetat=(float) tottat/n;
	avert=(float) totrt/n;
	printf("Average response time: %0.1f\nAverage waiting time: %0.1f\nAverage turnaround time: %0.1f\n", avert, avewt, avetat);
}