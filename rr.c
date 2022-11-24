#include <stdio.h>

struct P {
	int pn, arr, bur, star, finish, tat, wt, rt, rb;
	void (*print)(const struct P*);
	void (*set)(struct P*);
};

struct ReadyQueue {
    int size;
    struct P processes[10];
};

void init(struct ReadyQueue *ready) {
    ready->size=0;
}

void setP(struct P *p1) {
	p1->rt=p1->star-p1->arr;
	p1->tat=p1->finish-p1->arr;
	p1->wt=p1->tat-p1->bur;
}

void printP(const struct P *p1) {
	printf("%d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\t%6d\n", p1->pn, p1->arr, p1->bur, p1->star, p1->finish, p1->tat, p1->rt, p1->wt);
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

void SortArr(struct P plist[], int n) {
	int i, j;
	for(i=0; i<n; i++) {
		for(j=i+1; j<n; j++) {
			if(plist[i].arr>plist[j].arr)
				swap(&plist[i], &plist[j]);		
		}
	}
}

void pushP(struct ReadyQueue *ready, struct P p1) {
    ready->processes[ready->size++]=p1;
}

void popP(struct ReadyQueue *ready) {
    for(int i=0; i<ready->size-1; i++)
        ready->processes[i]=ready->processes[i+1];
    ready->size--;
}

void moveP(struct ReadyQueue *ready) {
    struct P P0;
    P0=ready->processes[0];
    for(int i=0; i<ready->size-1; i++)
        ready->processes[i]=ready->processes[i+1];
    ready->processes[ready->size-1]=P0;
}

int main() {
    	struct P plist[10];
	struct ReadyQueue ready;
    	init(&ready);
    	struct P term[10];

	int i, n;
	int totwt=0, tottat=0, totrt=0;

    	printf("Enter the number of processes: ");
	scanf("%d", &n);
	for(i=0; i<n; i++) {
		printf("Enter the Process Name, Arrival Time & Burst Time: ");
		scanf("%d%d%d", &plist[i].pn, &plist[i].arr, &plist[i].bur);
		plist[i].print=printP;
		plist[i].set=setP;
        	plist[i].rb=plist[i].bur;
	}

	SortArr(plist, n);

   	int q;
	printf("Enter the quantum time: ");
	scanf("%d", &q);
	
    	i=1;
	while(plist[i].arr==plist[0].arr) {
        	pushP(&ready, plist[i]);
        	i++;
    	}
    	int current_time = ready.processes[0].arr;
    	int exe_time;
    	int pi=i; //plist
    	i=0; //term

    	while (i<n) {
        	exe_time=1;
        	if (ready.processes[0].bur == ready.processes[0].rb)
			ready.processes[0].star = current_time;
        	while(exe_time<=q) {
            	current_time++;
            	ready.processes[0].rb--;
		exe_time++;
            	while(pi<n) {
                	if(plist[pi].arr==current_time) {
                    	pushP(&ready, plist[pi]);
                    	pi++;
                	}
                	else break;
            	}
            	if(ready.processes[0].rb==0) break;
        	}
		
        	if(ready.processes[0].rb==0) {
            	ready.processes[0].finish=current_time;
            	ready.processes[0].set(&ready.processes[0]);
            	term[i++]=ready.processes[0];
            	popP(&ready);
       	 	}
        	else moveP(&ready);
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
