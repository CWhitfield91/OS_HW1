#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iostream>
#include <vector>
#include <iomanip>
#include <random>

using namespace std;

////////////////////////////////////////////////////////////////
// sample events
#define EVENT1 1 //Schedule new arrivals, put processes in PCB
#define EVENT2 2 //Change process state, send to 
#define EVENT3 3 //kick into ready if x condition is meet
#define EVENT4 4 //exit and terminate process 

// state of processes
#define STATE1 1 //ready
#define STATE2 2 //executing
#define STATE3 3//waiting
#define STATE4 4//terminated

////////////////////////////////////////////////////////////////     //event structure
struct event{
    int PID;
    float arrivalTime;
    float burstTime;
    int type;
    event *next;
    // add more fields
};
event *head = NULL;
event *tail = NULL;

struct process {
	float burst;
	int state;
	float arrivalTime;
    float waitTime;
    float serviceTime;
    float turnAroundTime;
	int ID;
};

////////////////////////////////////////////////////////////////
// function definition
void init();
int run_sim();
void generate_report();
void schedule_event(struct event* eve);
void process_event1(struct event* eve);
void process_event2(struct event* eve);
void process_event3(struct event* eve);
void process_event4(struct event* eve);

////////////////////////////////////////////////////////////////
//Global variables
float cpuClock; // simulation clock
bool end_condition;
int PID;
int algorithm;
int lambda;
float serviceAvg;
int terminatedProcess;

vector<process>ProcessList;  //vector for proccesses

// //////////////////////////////////////////////////////////////// //schedules an event in the future
// int schedule_event(struct event* new)
// {
// // insert event in the event queue in its order of time
// }
////////////////////////////////////////////////////////////////
// returns a random number between 0 and 1
float urand()
{
    return( (float)rand()/RAND_MAX );
}
/////////////////////////////////////////////////////////////
// returns a random number that follows an exp distribution
float genexp(float lambda)
{
    float u,x;
    x = 0;
    while (x == 0)
    {
        u = urand();
        x = (1-log(u))/lambda;
    }
    return(x);
}
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[] )
{
    // parse arguments
    algorithm = atoi(argv[1]);
    lambda = atof(argv[2]);
    serviceAvg = atof(argv[3]);
    init();
    run_sim();
    generate_report();
    cout << "Algorithm: " << algorithm << " " << "Lambda: " << lambda << " " << "Service Avg: " << serviceAvg << endl;
    return 0;
}

int run_sim()
{
    struct event* eve;
    while (!end_condition)
    {
        eve = head;
        //cpuClock += eve->arrivalTime;
        switch (eve->type)
        {
            case EVENT1:
                process_event1(eve);
                break;
            case EVENT2:
                process_event2(eve);
                //cout << "Proc arrival time: " << ProcessList[eve->PID].arrivalTime << endl;
                break;
            case EVENT3:
				process_event3(eve);
                //cout << "event 3 arrival: " << eve->arrivalTime << endl;
                break;
			case EVENT4:
				process_event4(eve);
				break;

            default: ;
                // error
        }
        if(algorithm != 2) {
            head = eve->next;
            free(eve);
            eve = NULL;
        }
        if(algorithm == 2 && eve->type != STATE3) {
            head = eve->next;
            free(eve);
            eve = NULL;
        }
    }
    return 0;
}

void init()
{
    // initialize all varilables, states, and end conditions
    // schedule first events
    cpuClock = 0;
    end_condition = false;
	PID = 0;
    terminatedProcess = 0;

    //create first node
	event *eve = new event;
    eve->PID = PID;
    eve->type = EVENT1;
    eve->arrivalTime = 0;
    eve->burstTime = genexp(1/serviceAvg);
    eve->next = NULL;
    head = eve;
    tail = eve;

    PID++;

    
}

void process_event1(event *one){
    //create a new process
    cpuClock += one->arrivalTime;

	process proc;
    proc.burst = one->burstTime;
    proc.ID = one->PID;
    proc.arrivalTime = cpuClock;
    proc.state = STATE1;
    

    ProcessList.push_back(proc);

    //schedule new arrivals (currently has a default number)
    //arrival is clock + random
    event *eve = new event;
    eve->type = EVENT1;
    eve->arrivalTime = genexp(lambda);
    eve->burstTime = genexp(1/serviceAvg);
    eve->PID = PID;
    eve->next = NULL;
    tail->next = eve;
    tail = eve;

    PID++;

    //schedule type 2 event and place at front of event queue
    event *new_eve = new event;
    new_eve->PID = one->PID;
    new_eve->type = EVENT2;
    new_eve->burstTime = one->burstTime;
    new_eve->arrivalTime = one->arrivalTime;
    new_eve->next = one->next;
    one->next = new_eve;
    
}

void process_event2(event *two) {
	// move a process to ready 
	// Update/edit the event queue
	ProcessList[two->PID].state = STATE2;
    if(PID > 0) {
        cpuClock += ProcessList[two->PID - 1].burst;
    }
    ProcessList[two->PID].serviceTime = cpuClock;
    //ProcessList[two->PID].waitTime = cpuClock - ProcessList[two->PID].arrivalTime;
    cout << "Process arrival time: " << ProcessList[two->PID].arrivalTime << endl;
    cout << "Process service time: " << ProcessList[two->PID].serviceTime << endl;

    //schedule type 4 event for FCFS
    switch(algorithm){
        case 1: {
            event *eve = new event;
            eve->PID = two->PID;
            eve->type = EVENT4;
            eve->burstTime = two->burstTime;
            eve->arrivalTime = 0;
            eve->next = two->next;
            two->next = eve;
            break;
        }
        case 2: {   
            if(two->burstTime > two->next->burstTime && two->next != NULL) {
                two->type = EVENT3;
            }
            else {
                event *eve = new event;
                eve->PID = two->PID;
                eve->type = EVENT4;
                eve->burstTime = two->burstTime;
                eve->next = two->next;
                two->next = eve;
            }
            break;
        }
    }
}
void process_event3(event *three){
    event *cursor = three->next;
    event *temp = three;
    three->next = cursor->next;
    cursor->next = three;
    three = cursor;

}

void process_event4(event *four) {
    //change process state (not necessary for FCFS)
    cpuClock += four->burstTime;

	ProcessList[four->PID].state = STATE4;
    ProcessList[four->PID].turnAroundTime = cpuClock - ProcessList[four->PID].arrivalTime;

    //increment terminatedProcess
	terminatedProcess++;

    //check if number of terminated processes is enough (currently default value)
	if (terminatedProcess == 10) {
		end_condition = true;
	}
}

void generate_report()
{
    float avgWaitTime = 0;
    float avgTurnAround = 0;
    float totalTurnAroundtime = 0;
    //calculate average wait time
    for(int i = 0; i < ProcessList.size(); i++) {
        avgWaitTime += ProcessList[i].waitTime;
        //cout << "avg wait time: " << avgWaitTime << endl;
    }

    avgWaitTime = avgWaitTime / 10;
    float avgQueueLen = lambda * avgWaitTime;
    cout << "Average Queue Length: " << avgQueueLen << endl;
    //cout << "Average wait time: " << fixed << setprecision(3) << avgWaitTime << endl;
    // calculate average turn around time
    for(int i = 0; i < ProcessList.size(); i++) {
        avgTurnAround += ProcessList[i].turnAroundTime;
    }

    avgTurnAround /= ProcessList.size();
    //cout << "Average Turn Around Time: " << avgTurnAround << " seconds." << endl;

}