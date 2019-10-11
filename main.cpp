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
struct event {
	int PID;
	float arrivalTime;
	float burstTime;
	int type;
	event* next;
	// add more fields
};
event* head = NULL;
event* tail = NULL;

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
//void schedule_event(struct event* eve);
void process_event1(struct event* eve);
void process_event2(struct event* eve);
void process_event3(struct event* eve);
void process_event4(struct event* eve);

////////////////////////////////////////////////////////////////
//Global variables
float cpuClock; // simulation clock
bool end_condition; //ends the sim loop
int PID;
int algorithm;
int lambda;
float serviceAvg;
int terminatedProcess;
float quantum;

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
	return((float)rand() / RAND_MAX);
}
/////////////////////////////////////////////////////////////
// returns a random number that follows an exp distribution
float genexp(float lambda)
{
	float u, x;
	x = 0;
	while (x == 0)
	{
		u = urand();
		x = (-1 / lambda) * log(u);
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
		//cpuClock += eve->arrivalTime + cpuClock;
		switch (eve->type)
		{
		
		case EVENT1:
			//cout << "event 1: " << eve->arrivalTime << "  PID: " << eve->PID << "  status: " << ProcessList[].ID << endl;
			process_event1(eve);
			//cout << "event 1 burst: " << eve->burstTime << endl;
			//cout << " cpuClock after event 1: " << cpuClock << endl;
			break;
		case EVENT2:
			//cout << "event 2 burst: " << eve->burstTime << endl;
			//cout << "next event burst: " << eve->next->burstTime << endl;
			
			process_event2(eve);
			//cout << " cpuClock after event 2: " << cpuClock << endl;
			break;
		case EVENT3:
			process_event3(eve);
			
			//cout << "event 3 burst: " << eve->burstTime << endl;
			//cout << "next event burst: " << eve->next->burstTime << endl;
			//cout << " cpuClock after event 3: " << cpuClock << endl;
			break;
		case EVENT4:
			//cout << "event 4: " << eve->arrivalTime << "  PID: " << eve->PID << "  state: " << ProcessList[eve->PID].state << endl;               
			process_event4(eve);
			break;

		default:;
			// error
		}

		if (algorithm == 3) {

		}
		else if (algorithm != 2) {
			head = eve->next;
			free(eve);
			eve = NULL;
		}
		else if (algorithm == 2 && eve->type != STATE3) {
			head = eve->next;
			free(eve);
			eve = NULL;
		}
	}
	cout << "FINISHED" << endl;
	return 0;
}
////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
	float temp = .06;
	// parse arguments
	algorithm = 3;// atoi(argv[1]);
	lambda = 1;// atof(argv[2]);
	serviceAvg = temp;// atof(argv[3]);
	quantum = 0.1;
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
	event* eve = new event;
	eve->PID = PID;
	eve->type = 1;
	eve->arrivalTime = genexp(1 / lambda); // arival times for lambda processes
	eve->burstTime = genexp(1 / serviceAvg); // burst time.
	eve->next = NULL;
	head = eve;
	tail = eve;

	process proc;
	proc.burst = eve->burstTime;
	proc.ID = eve->PID;
	proc.state = STATE1;
	ProcessList.push_back(proc);

	PID++;
}


void process_event1(event* one) {
	cout << "in event 1" << endl;
	
	if (algorithm == 3) {
		//sort into algorithm
		
		
		float tempClock = cpuClock;
		float nextArrivalTime = genexp(1 / serviceAvg);
		
		float breakTime = cpuClock + quantum;

		while (breakTime >= tempClock + nextArrivalTime) //&& ProcessList.size() <= 30)
		{
			cout << "safty check cpuClock: " << cpuClock << endl;
			cout << "safty check breakTime: " << breakTime << endl;
			nextArrivalTime += tempClock;			
			//create new event1
			//schedule new arrivals (currently has a default number)
			//arrival is clock + random
			event* eve = new event;
			eve->type = EVENT1;
			eve->arrivalTime = nextArrivalTime + tempClock;
			nextArrivalTime = genexp(1 / lambda);
			eve->burstTime = genexp(1 / serviceAvg);
			eve->PID = PID;

			eve->next = NULL;
			tail->next = eve;
			tail = eve;

			//creates a corrisponding process
			process proc;
			proc.burst = eve->burstTime;
			proc.ID = eve->PID;
			proc.state = STATE2;
			ProcessList.push_back(proc);
			PID++;

			//schedules the process list for execution
			//places the new event at the end of the list
			//schedule type 2 event and place at front of event queue
			//event* eve = new event;
			
		}
		one->type = EVENT2;
	}
	else {
		process proc;
		event* eve = new event;
		proc.burst = one->burstTime;
		proc.ID = one->PID;
		proc.state = STATE1;
		ProcessList.push_back(proc);

		//schedule new arrivals (currently has a default number)
		//arrival is clock + random
		for (int i = 0; i < 1; i++) {
			event* eve = new event;
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
		//event* eve = new event;
		eve->PID = one->PID;
		eve->type = EVENT2;
		eve->burstTime = one->burstTime;
		eve->burstTime = one->arrivalTime;
		eve->next = one->next;
		one->next = eve;
	}
}

void process_event2(event* two) {
	// move a process to ready 
	// Update/edit the event queue
	cout << "in event 2" << endl;  //debug statement
	//cout << "Cpu start in event 2: " << cpuClock << endl;
	ProcessList[two->PID].state = STATE2;
	waitTimes.push_back(cpuClock - two->arrivalTime);///////////////////////timing
	
	//schedule type 4 event for FCFS
	switch (algorithm) {
	case 1: {
		event* eve = new event;
		eve->PID = two->PID;
		eve->type = EVENT4;
		eve->burstTime = two->burstTime;
		eve->next = two->next;
		two->next = eve;
		break;
	}
	case 2: {
		if (two->burstTime > two->next->burstTime&& two->next != NULL) {
			two->type = EVENT3;
		}
		else {
			event* eve = new event;
			eve->PID = two->PID;
			eve->type = EVENT4;
			eve->burstTime = two->burstTime;
			eve->next = two->next;
			two->next = eve;
		}
		break;
	}
	case 3: {
		//this if statment is to reschedule

		if (two->burstTime > quantum) {
			two->type = EVENT3;
			float tempBurst;
			tempBurst = two->burstTime - quantum;
			two->burstTime -= quantum;
			cout << "Event 2 reschedule" << endl;
			two->burstTime = tempBurst;
			cout << "event rescheduled" << endl;
			/*event* tempHead = two;
			if (tempHead->next != NULL) {

				while ((tempHead->next != NULL)) {
					tempHead = tempHead->next;
					//cout <<"list check"<< tempHead->burstTime<<endl;
				}
				tempHead->next = two;
			}*/

		}
		else {
			two->type = EVENT4;
		}
	}
		  break;
	}
}


void process_event3(event* three) {
	cout << "in event 3" << endl;
	
	if (algorithm == 3) {
		head = three->next;
		/*while ((tempHead->next != NULL)) {
			tempHead = tempHead->next;
			cout << "loop test";
		}*/
		tail->next = three;
		tail = three;
		three->next = NULL;
		three->type = EVENT1;
		cpuClock += quantum;
		cout << "testing: " << endl;
	}
	else{

		event* cursor = three->next;
		event* temp = three;
		three->next = cursor->next;
		cursor->next = three;
		three = cursor;
	}

}

void process_event4(event* four) {
	//change process state (not necessary for FCFS)
	cout << "in event 4" << endl;
	cout << "ID: " << four->PID << endl;
	//cout << "clock" << cpuClock << endl;
	//cpuClock += four->burstTime;
	//cout << "clock + burst" << cpuClock << endl;
	ProcessList[four->PID].state = STATE4;
	turnAroundTimes.push_back(cpuClock);

	//increment terminatedProcess
	terminatedProcess++;
	//check if to terminate

	cpuClock += four->burstTime;

	//terminate a event
	if (algorithm == 3) {
		//cout << four->next << endl << endl;
		//cout << four->next->type << endl;
		
		four->type = 1;
		if (four->next == NULL) {
			cout << "entered the doom relme" << endl;
			event* eve = new event;
			eve->PID = PID;
			eve->type = 1;
			eve->arrivalTime = genexp(1 / lambda); // arival times for lambda processes
			eve->burstTime = genexp(1 / serviceAvg); // burst time.
			eve->next = NULL;
			head = eve;
			//tail = eve;

			process proc;
			proc.burst = eve->burstTime;
			proc.ID = eve->PID;
			proc.state = STATE1;
			ProcessList.push_back(proc);

			PID++;

			event* temp = four->next;
			delete four;
			head->next = temp;
		}
		else {
			/*event* temp = four->next;
			delete four;
			head->next = temp;*/
			head = four->next;
			free(four);
			four = NULL;
		}
	}

	//check if number of terminated processes is enough (currently default value)
	if (terminatedProcess == 10000) {
		end_condition = true;
		cout << "true exit" << endl;
	}
}

void generate_report()
{
	int  long avgWaitTime = 0;
	int long avgTurnAround = 0;
	int long totalTurnAroundtime = 0;
	//calculate average wait time
	for (int i = 0; i < waitTimes.size(); i++) {
		avgWaitTime += waitTimes[i];
	}

	//avgWaitTime /= waitTimes.size();
	float avgQueueLen = lambda * avgWaitTime;
	cout << "Average Queue Length: " << avgQueueLen << endl;

	//cout << avgWaitTime << endl;
	//calculate average turn around time
	for (int i = 0; i < turnAroundTimes.size(); i++) {
		avgTurnAround += turnAroundTimes[i];
	}

	avgTurnAround /= turnAroundTimes.size();
	cout << "Average Turn Around Time: " << avgTurnAround / 10000 << " seconds." << endl;

}