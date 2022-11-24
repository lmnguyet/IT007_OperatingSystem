#include <stdio.h>

struct P {
	int pn, arr, bur, star, finish, tat, wt, rt;
	void (*print)(const struct P*);
	void (*set)(struct P*);
};

struct ReadyQueue {
    int size;
    int front;
    int rear; 
    struct P processes[10];
};

void init(struct ReadyQueue *ready) {
    ready->front=0;
    ready->size=0;
    ready->rear=ready->size-1;
}

void printP(const struct P *p1) {
	printf("%d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\n", p1->pn, p1->arr, p1->bur, p1->star, p1->finish, p1->tat, p1->rt, p1->wt);
}

void setP(struct P *p1) {
	p1->wt=p1->rt=p1->star-p1->arr;
	p1->finish=p1->star+p1->bur;
	p1->tat=p1->finish-p1->arr;
}

void pushP(struct ReadyQueue *readyQ, struct P p1)
{
    readyQ->rear = readyQ->rear + 1;
    readyQ->processes[readyQ->rear] = p1;
    readyQ->size = readyQ->size + 1;
}

void popP(struct ReadyQueue *readyQ)
{
    readyQ->front = readyQ->front + 1;
    readyQ->size = readyQ->size - 1;
}

void swap(struct P *a, struct P *b) {
	int tmp=a->arr;
	a->arr=b->arr;
	b->arr=tmp;

	tmp=a->pn;
	a->pn=b->pn;
	b->pn=tmp;

	tmp=a->bur;
	a->bur=b->bur;
	b->bur=tmp;
}

void QSort(struct P plist[], int l, int r) {
	int i, j, x;
	x=plist[(l+r)/2].arr;
	i=l; j=r;
	while (i<j) {
		while (plist[i].arr<x) i++;
		while (plist[j].arr>x) j--;
		if (i<=j) {
			swap(&plist[i], &plist[j]);
			i++;
			j--;
		}
	}
	if (l<j) QSort(plist, l, j);
	if (i<r) QSort(plist, i, r);
}


void main() {
	struct P plist[10];
    	struct ReadyQueue ready;
    	init(&ready);
    	struct P term[10];
	int i, n;
	int totwt=0, tottat=0, totrt=0;
    	int current_time;
	printf("Enter the number of processes: ");
	scanf("%d", &n);
	for(i=0; i<n; i++) {
		printf("Enter the Process Name, Arrival Time & Burst Time: ");
		scanf("%d%d%d", &plist[i].pn, &plist[i].arr, &plist[i].bur);
		plist[i].print=printP;
		plist[i].set=setP;
	}
	QSort(plist, 0, n-1);

    	pushP(&ready, plist[0]);
    
    	current_time=ready.processes[0].arr;
    	i=0; // ready
    	int j=0; // term
    	int m=1; // plist
    	while(ready.front<n) {
        	i=ready.front;
        	if(i==0) ready.processes[i].star=ready.processes[i].arr;
        	else ready.processes[i].star=term[j-1].finish;
        	ready.processes[i].set(&ready.processes[i]);
        	while(current_time<=ready.processes[i].finish && m<n) {
            		if(current_time==plist[m].arr) {
                		pushP(&ready, plist[m]);
                		m++;
            		}
            		current_time++;
        	}
        	term[j++]=ready.processes[i];
        	popP(&ready);
    	}

	
	printf("\nPName\tArrtime\tBurtime\tStart\tFinish\tTAT\tRT\tWT\n");
	for(i=0; i<n; i++) {
		term[i].print(&term[i]);
		totwt+=term[i].wt;
		tottat+=term[i].tat;
        	totrt+=term[i].rt;
	}

	float avewt, avetat, avert;
	avewt=(float) totwt/n;
	avetat=(float) tottat/n;
    	avert=(float) totrt/n;
	printf("Average waiting time: %0.3f\nAverage response time: %0.3f\nAverage turnaround time: %0.3f\n", avewt, avert, avetat);

}

