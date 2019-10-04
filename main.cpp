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
#define STATE1 1 //new
#define STATE2 2 //ready
#define STATE3 3//executing
#define STATE4 4//tereminate

////////////////////////////////////////////////////////////////     //event structure
struct event{
    int PID;
    float arrivalTime;
    int type;
    event *next;
    // add more fields
};
event *head = NULL;

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
void process_event4(struct event* eve);

////////////////////////////////////////////////////////////////
//Global variables
float cpuClock; // simulation clock
bool end_condition;
int PID;;
int algorithm;
float lambda;
int terminatedProcess;

vector<process>ProcessList;


////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////
// void generate_report()
// {
//     // output statistics
// }
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
        cpuClock = eve->arrivalTime;
        switch (eve->type)
        {
            case EVENT1:
                cout << "I am in case EVENT1" << endl;
                process_event1(eve);
                break;
            case EVENT2:
                process_event2(eve);
                break;
            case EVENT3:
				//process_event3(eve);
                break;
			case EVENT4:
				process_event4(eve);
				break;

            default: ;
                // error
        }
      head = eve->next;
      free(eve);
      eve = NULL;
    }
    cout << "FINISHED" << endl;
    return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char *argv[] )
{
    // parse arguments
    algorithm = atoi(argv[1]);
    lambda = atof(argv[3]);
    init();
    cout << algorithm << " " << lambda << endl;
    
    cout << head->PID << " " << head->type << head->arrivalTime << endl;
    run_sim();
    //generate_report();
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
    eve->arrivalTime = genexp(0.06);
    eve->next = NULL;
    head = eve;
}

void process_event1(event *one){
	//add schedule and move to <<<What does this mean?
	//need to add a scheduling alg for others
    
    //cout << "I am in process_event1()" << endl;  //debug statement
    //create and put a process in a vector (not necessary for FCFS)
	process proc;
    proc.burst = genexp(0.06);
    proc.ID = PID;
    proc.state = 1;
    event *cursor = head;
    while(!cursor) {
        if(cursor->PID == proc.ID) {
            proc.arrivalTime = cursor->arrivalTime;
            break;
        }
    }

    ProcessList.push_back(proc);

    //schedule new arrivals (currently has a default number)
    for(int i = 0; i < 30; i++){
        event *eve = new event;
        eve->type = EVENT1;
        eve->next = one->next;
        one->next = eve;
    }

    //schedule type 2 event and place at front of event queue
    event *eve = new event;
    eve->PID = one->PID;
    eve->type = EVENT2;
    eve->next = one->next;
    one->next = eve;

}

void process_event2(event *two) {
	// move a process to ready 
	// Update/edit the event queue
    //cout << "I am in process_event2" << endl;  //debug statement
	ProcessList[0].state = STATE2;

    //schedule type 4 event for FCFS
    event *eve = new event;
    eve->PID = two->PID;
    eve->type = EVENT4;
    eve->next = two->next;
    two->next = eve;

}
// void process_event3(event Three){
// 	new event;//this event will be added to queue for report.
// 	//??how to do this with update to time.
// 	return;
// }

void process_event4(event *four) {
	//Update the event list

    //change process state (not necessary for FCFS)
	ProcessList[0].state = STATE4;

    //increment terminatedProcess
	terminatedProcess++;
	//check if to terminate

    //check if number of terminated processes is enough (currently default value)
	if (terminatedProcess == 30) {
		end_condition = true;
	}
}