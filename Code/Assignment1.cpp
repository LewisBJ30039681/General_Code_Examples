#include <iostream>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>
#include <random>

using namespace std;
using namespace chrono;

enum trafficLight{Red, Yellow, Green}; //Setting up the traffic lights that can be used by both sets. Red = 0 Yellow = 1 Green = 2.
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //Setting up the mutex initializer for later use.
sem_t semaphoreSignal; //Setting up semaphore for later use.

void* lightSetA(void* pVal) //Setting up the funcation for the traffic light set A to be used in a thread.
{
    trafficLight light_1 = Red; //Light 1 variable is set from the trafficLight data.
    trafficLight light_2 = Red; //Light 2 variable is set from the trafficLight data.

    int minVal = 5; //Setting up min random value.
    int maxVal = 10; //Setting up max random value.

    random_device ranNum;
    mt19937 gen(ranNum());
    uniform_int_distribution <> distr(minVal, maxVal); //Setting the range for random car counter value.

    int light1CarCounter = distr(gen); //Setting up variable for light 1 car counter.
    int light2CarCounter = distr(gen); //Setting up variable for light 2 car counter.

    cout << light1CarCounter << " cars waiting at light 1." << endl; //Mainly for debugging.
    cout << light2CarCounter << " cars waitng at light 2." << endl; //Mainly for debugging.

    pthread_mutex_lock (&mutex); //Locking the mutex, as it needs to be locked first.

    if (light_1 == Red && light_2 == Red) //Simple if statement to check if lights are red, then change the lights to yellow and green.
    {
        cout << "Traffic light 1 and light 2 are Red" << endl;

        sleep(3); //This is here to make the simulation feel more natural.

        cout << "Light 1 and light 2 are turning Yellow" << endl;
        
        light_1 = Yellow;
        light_2 = Yellow;

        cout << "Light 1 and light 2 are yellow" << endl;
        cout << "Light 1 and light 2 are turning Green" << endl;

        sleep(3); //This is here to make the simulation feel more natural.

        light_1 = Green;
        light_2 = Green;

        cout << "Light 1 and light 2 are Green" << endl;
    }
    else //Error check if the lights aren't the same the error will be prodecued.
    {
        cout << "Error with light set A." << endl;
    }

    while (light_1 == Green && light_2 == Green && (light1CarCounter > 0 || light2CarCounter > 0)) //loop will run until both random cars go through
    {
        if (light1CarCounter > 0)
        {
            cout << "A car has passed through light 1." << endl;
            light1CarCounter--; // Decrement only if greater than 0 as not to break the loop
        }
        if (light2CarCounter > 0)
        {
            cout << "A car has passed through light 2." << endl;
            light2CarCounter--; // Decrement only if greater than 0 as not to break the loop
        }
        
        sleep(2); // Simulate the delay between cars passing
    }

    if (light1CarCounter == 0 && light2CarCounter == 0) //If statement to turn the light back to red.
    {
        cout << "Light 1 and light 2 are turning Yellow." << endl;

        sleep(3); //This is to make the simulation feel more natural.

        light_1 = Yellow;
        light_2 = Yellow;

        cout << "Light 1 and light 2 are Yellow." << endl;
        cout << "Light 1 and light 2 are turning Red" << endl;

        sleep(3); //This is to make the simulation feel more natural.

        cout << "Light 1 and light 2 are Red." << endl;
    }

    pthread_mutex_unlock (&mutex);

    sem_post(&semaphoreSignal); //Sends signal that setA is complete.

    return NULL;
}

void* lightSetB(void* pVal) //Setting up function for light set B to be used as a thread.
{
    trafficLight light_3 = Red; //Light 3 variable is set from the trafficLight data.
    trafficLight light_4 = Red; //Light 4 variable is set from the trafficLight data.

    int minVal = 5; //setting up min random value
    int maxVal = 10; //setting up max random value

    random_device ranNum;
    mt19937 gen(ranNum());
    uniform_int_distribution <> distr(minVal, maxVal); //Random number range for the car counter random number

    int light3CarCounter = distr(gen); //Setting up variable for light 1 car counter. Is random for better simulation.
    int light4CarCounter = distr(gen); //Setting up variable for light 2 car counter. Is random for better simulation.
    
    sem_wait(&semaphoreSignal); //Waitng for signal to start.

    pthread_mutex_lock (&mutex); //This will stop setB running until setA is complete.

    cout << light3CarCounter <<  " cars waitng at light 3" << endl; //Mainly for debugging.
    cout << light4CarCounter << " cars waiting at light 4." << endl; //Mainly for debugging.

    if (light_3 == Red && light_4 == Red) //Simple if statement to check if lights are red, then change the lights to yellow and green.
    {
        cout << "Traffic light 3 and light 4 are Red" << endl;

        sleep(3); //This is here to make the simulation feel more natural.

        cout << "Light 3 and light 4 are turning Yellow" << endl;
        
        light_3 = Yellow;
        light_4 = Yellow;

        cout << "Light 3 and light 4 are yellow" << endl;
        cout << "Light 3 and light 4 are turning Green" << endl;

        sleep(3); //This is here to make the simulation feel more natural.

        light_3 = Green;
        light_4 = Green;

        cout << "Light 3 and light 4 are Green" << endl;
    }
    else //Error check if the lights aren't the same the error will be prodecued.
    {
        cout << "Error with light set B." << endl;
    }

    while (light_3 == Green && light_4 == Green && (light3CarCounter > 0 || light4CarCounter > 0)) //loop will run until both random cars go
    {
        if (light3CarCounter > 0)
        {
            cout << "A car has passed through light 3." << endl;
            light3CarCounter--; // Decrement only if greater than 0 as to not break the loop
        }
        if (light4CarCounter > 0)
        {
            cout << "A car has passed through light 4." << endl;
            light4CarCounter--; // Decrement only if greater than 0 as to not break the loop
        }
        
        sleep(2); // Simulate the delay between cars passing
    }

    if (light3CarCounter == 0 && light4CarCounter == 0) //If statement to turn the light back to red.
    {
        cout << "Light 3 and light 4 are turning Yellow." << endl;

        sleep(3); //This is to make the simulation feel more natural.

        light_3 = Yellow;
        light_4 = Yellow;

        cout << "Light 3 and light 4 are Yellow." << endl;
        cout << "Light 3 and light 4 are turning Red" << endl;

        sleep(3); //This is to make the simulation feel more natural.

        cout << "Light 3 and light 4 are Red." << endl;
    }

    pthread_mutex_unlock (&mutex); //Unlocks the mutex after setB is complete.

    return NULL;
}

int main() //Main code
{
    
    pthread_t threadA; //Setting up threadA variable.
    pthread_t threadB; //Setting up threadB variable.

    auto start = high_resolution_clock::now(); //Starts the tracking of how long the process takes.

    pthread_create (&threadA, NULL, lightSetA, NULL); //Creating new thread.
    pthread_create (&threadB, NULL, lightSetB, NULL); //Creating new thread.

    pthread_join (threadA, NULL); //Getting the thread created to join.
    pthread_join (threadB, NULL); //Getting the thread created to join.

    auto stop = high_resolution_clock::now(); //Stops the tracking of the process.

    auto duration = duration_cast <seconds> (stop - start); //Getting the duration time in seconds.

    cout << duration.count() << " seconds run time." << endl; //Outputting the duration of the program runtime.

    pthread_mutex_destroy (&mutex); //Destorying the mutext as it is no longer needed.
    sem_destroy(&semaphoreSignal); //Destorying semaphore as it is no longer needed.

    return 0;
}