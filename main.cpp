#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <list>
#include <iostream>
#include <vector>

using namespace std;

struct process {
	float burst;
	int state;
	int arivalTime;
	int ID;
};

////////////////////////////////////////////////////////////////
// sample events
#define EVENT1 1 //Schedule new arrivals, put processes in PCB
#define EVENT2 2 //Change process state, send to 
#define EVENT3 3 //kick into ready if x condition is meet
#define EVENT4 4 //exit and terminate process 

// state of processes
#define STATE1 1 //new
#define STATE2 2 //ready
#define STATE3 3//executing
#define STATE4 4//tereminate

////////////////////////////////////////////////////////////////     //event structure
struct event{
    int event_PID;
    float time;
    int   type;
    // add more fields
    struct event* next;
};

////////////////////////////////////////////////////////////////
// function definition
void init();
int run_sim();
void generate_report();
int schedule_event(struct event*);
int process_event1(struct event* eve);
int process_event2(struct event* eve);

////////////////////////////////////////////////////////////////
//Global variables
struct event* head; // head of event queue
float clock; // simulation clock
bool end_condition;
int PID;
int algorithm;
float lambda;
int terminatedProcess = 0;

list<event>EventQueue;
vector<process>ProcessList;


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
void generate_report()
{
    // output statistics
}
//////////////////////////////////////////////////////////////// //schedules an event in the future
int schedule_event(struct event* new)
{
// insert event in the event queue in its order of time
}
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
        clock = eve->time;
        switch (eve->type)
        {
            case EVENT1:
                process_event1(eve);
                break;
            case EVENT2:
                process_event2(eve);
                break;
            case EVENT3:
				process_event3(eve);
                break;
			case EVENT4:
				process_event4(eve);
				break;

            default:
                // error
        }

        head = eve->next;
        free(eve);
        eve = NULL;
    }
    return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[] )
{
    // parse arguments
    algorithm = strtol(argv[1], NULL, 10);
    lambda = strtof(argv[3], NULL);
    init();
    run_sim();
    generate_report();
    return 0;
}

void init()
{
    // initialize all varilables, states, and end conditions
    // schedule first events
    end_condition = true;
    clock = 0;
	PID = 0;
    //create first node
	event eve;
	eve.event_PID = PID;
	eve.time = genexp(lambda);
	//set all event variables
	eve.type = EVENT1;
	EventQueue.push_front(eve);
	head = EventQueue.begin();
}

void process_event1(event One){
	//add schedule and move to 
	// need to add a scheduling alg for others

	new process tempProcess;
	tempProcess.ID = One.event_PID;
	tempProcess.state = STATE2;
	tempProcess.burst = One.time;
	ProcessList.back(tempProcess);
	delete tempProcess;
	One.type = EVENT2;
	return;
}

void process_event2(event Two) {
	// move a process to ready 
	// Update/edit the event queue
	new process tempProcess;
	tempProcess = ProcessList.begin();
	tempProcess.state = STATE4;
	ProcessLsit[0].replace(ProcessList[0], tempProcess);//IDK if this is how to replace the current process we are working on.
	Two.type = EVENT4;
	delete tempProcess;

	new event;//this event will be added to queue for report.
	//??how to do this with update to time.

	return;
}
void process_event3(event Three){
	new event;//this event will be added to queue for report.
	//??how to do this with update to time.
	return;
}
void process_event4(event Four) {
	//Update the event list
	ProcessList.pop_front();//removes the first element 

	terminatedProcess++;
	//check if to terminate
	if (terminatedProcess == 10000) {
		end_condition = true;
	}
	new event;//this event will be added to queue for report.
	//??how to do this with update to time.
	return;
}