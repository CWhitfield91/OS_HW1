#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iostream>
#include <vector>

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
	int arrivalTime;
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
vector<int>waitTimes;
vector<int>turnAroundTimes;

// //////////////////////////////////////////////////////////////// //schedules an event in the future
// int schedule_event(struct event* new)
// {
// // insert event in the event queue in its order of time
// }
////////////////////////////////////////////////////////////////
// returns a random number between 0 and 1
float urand()
{
    return( (float) rand()/RAND_MAX );
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
        x = (-1/lambda)*log(u);
    }
    return(x);
}
////////////////////////////////////////////////////////////
int run_sim()
{
    struct event* eve;
    while (!end_condition)
    {
        eve = head;
        cpuClock += eve->arrivalTime;
        switch (eve->type)
        {
            case EVENT1:
                //cout << "event 1: " << eve->arrivalTime << "  PID: " << eve->PID << "  status: " << ProcessList[].ID << endl;
                process_event1(eve);
                cout << "event 1 burst: " << eve->burstTime << endl;
                break;
            case EVENT2:
                cout << "event 2 burst: " << eve->burstTime << endl;
                cout << "next event burst: " << eve->next->burstTime << endl;
                process_event2(eve);
                break;
            case EVENT3:
				process_event3(eve);
                cout << "event 3 burst: " << eve->burstTime << endl;
                cout << "next event burst: " << eve->next->burstTime << endl;
                break;
			case EVENT4:
                //cout << "event 4: " << eve->arrivalTime << "  PID: " << eve->PID << "  state: " << ProcessList[eve->PID].state << endl;               
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
    cout << "FINISHED" << endl;
    return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[] )
{
    // parse arguments
    algorithm = atoi(argv[1]);
    lambda = atof(argv[2]);
    serviceAvg = atof(argv[3]);
    init();
    //cout << algorithm << " " << lambda << endl;
    
    //cout << head->PID << " " << head->type << " " << head->arrivalTime << endl;
    run_sim();
    generate_report();
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
    eve->type = 1;
    eve->arrivalTime = genexp(1 / lambda);
    eve->burstTime = genexp(1 / serviceAvg);
    eve->next = NULL;
    head = eve;
    tail = eve;

    PID++;
}

void process_event1(event *one){
    //create a new process
	process proc;
    proc.burst = one->burstTime;
    proc.ID = one->PID;
    proc.state = STATE1;
    

    ProcessList.push_back(proc);

    //schedule new arrivals (currently has a default number)
    //arrival is clock + random
    for(int i = 0; i < 1; i++){
        event *eve = new event;
        eve->type = EVENT1;
        eve->arrivalTime = genexp(1 / lambda);
        eve->burstTime = genexp(1 / serviceAvg);
        eve->PID = PID;
        eve->next = NULL;
        tail->next = eve;
        tail = eve;

        PID++;
    }

    //schedule type 2 event and place at front of event queue
    event *eve = new event;
    eve->PID = one->PID;
    eve->type = EVENT2;
    eve->burstTime = one->burstTime;
    eve->burstTime = one->arrivalTime;
    eve->next = one->next;
    one->next = eve;
}

void process_event2(event *two) {
	// move a process to ready 
	// Update/edit the event queue
    //cout << "I am in process_event2" << endl;  //debug statement
	ProcessList[two->PID].state = STATE2;
    waitTimes.push_back(cpuClock - two->arrivalTime);

    //schedule type 4 event for FCFS
    switch(algorithm){
        case 1: {
            event *eve = new event;
            eve->PID = two->PID;
            eve->type = EVENT4;
            eve->burstTime = two->burstTime;
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
    turnAroundTimes.push_back(cpuClock);

    //increment terminatedProcess
	terminatedProcess++;
	//check if to terminate

    //check if number of terminated processes is enough (currently default value)
	if (terminatedProcess == 10000) {
		end_condition = true;
	}
}

void generate_report()
{
    int  long avgWaitTime = 0;
    int long avgTurnAround = 0;
    int long totalTurnAroundtime = 0;
    //calculate average wait time
    for(int i = 0; i < waitTimes.size(); i++) {
        avgWaitTime += waitTimes[i];
    }

    avgWaitTime /= waitTimes.size();
    float avgQueueLen = lambda * avgWaitTime;
    cout << "Average Queue Length: " << avgQueueLen << endl;

    //cout << avgWaitTime << endl;
    //calculate average turn around time
    for(int i = 0; i < turnAroundTimes.size(); i++) {
        avgTurnAround += turnAroundTimes[i];
    }

    avgTurnAround /= turnAroundTimes.size();
    cout << "Average Turn Around Time: " << avgTurnAround / 10000 << " seconds." << endl;

}