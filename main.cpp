#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

////////////////////////////////////////////////////////////////
// sample events
#define EVENT1 1 // First come first serve
/*
//for event 1
int processCompleted  = 0;
while(processCompleted < 10000){


}
*/
#define EVENT2 2 // least burst time first.
//for event 1
/*int processCompleted = 0;
while (processCompleted < 10000) {


}
*/
// .. add more events
#define EVENT3 3 // Round robin

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
                //do something
                break;

                // add more events

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
    //create first node
    event *new_eve = new event(); //create first node
    new_eve->event_PID = PID;     //set node variables
    new_eve->time = genexp(lambda);
    new_eve->type = EVENT1;
    new_eve->next = head;

    head = new_eve; //make head point to new node

}