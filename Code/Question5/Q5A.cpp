#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <thread>
#include <vector>
#include <mutex>
#include <cmath>
#include <algorithm>

using namespace std;

bool isNumPrime(int number) //this function is to check if the numbers are prime
{
    if (number <= 1)
    {
        return false; //error check for numbers <= 1
    }
    for (int i = 2; i <= sqrt(number); ++i) //checks for squre root divison 
    {
        if (number % i == 0)
        {
            return false; //error check as number is divisable so can not be prime
        }
    }
    return true; //returns if number is a prime number
}

void primeNumbersInRange(int startNum, int endNum, int threadID, vector<int>& primes, mutex& mtx) //this function lets each thread find the prime numbers within the set range
{
    vector<int> primeNums; //this vector is local to store prime numbers found by the thread
    
    for (int number = startNum; number < endNum; ++number)
    {
        if (isNumPrime(number)) //checks to see if number is prime if so will add to list
        {
            primeNums.push_back(number);
        }
    }

    lock_guard<mutex> lock(mtx); //this is for data saftey when printing the prime numbers
    primes.insert(primes.end(), primeNums.begin(), primeNums.end()); //primes are added to the shared list
    cout << "Thread " << threadID << " found these Prime numbers: ";
    for (const auto& prime : primeNums)
    {
        cout << prime << " "; //lets each thread print out the primes they find
    }
    cout << endl;
}

int main()
{
    int N; //Max number of numbers to check variable
    int T; //the number of thread that will be used varible

    cout << "Enter limit for prime number search: ";
    cin >> N; //gets the max number to check from user input

    if (N < 2) //error check so that you can not input number that wont work.
    {
        cout << "Error, insufficient max number search. Number must be 2 or greater than 2." << endl;
        exit(1);
    }

    cout << "Enter the amount of threads to be used: ";
    cin >> T; //gets the number of threads by user input

    if (T <= 0) //check to see if there is a vaible number of threads to use
    {
        cout << "Error, insufficient number of threads inputted. Number must be greater than 0." << endl;
        exit(1);
    }

    vector<thread> threads; //this vector is to hold the threads
    vector<int> primes; //this is a shared vector that stores the primes, is used in the primeNumbersInRange fucntion
    mutex mtx; //mutex is created to unsure data sync

    for (int i = 0; i < T; i++) //this is to divide the rnage of numbers so the work is spread more evenly, I think it looks better this way less empty threads
    {
        int range = (N - 2 + T - 1) /T; //range caluction
        int startNum = 2 + i * range; //the start of the range
        int endNum = min(N + 1, startNum + range); //the end of the range
        threads.push_back(thread(primeNumbersInRange, startNum, endNum, i + 1, ref(primes), ref(mtx))); //this creats the thread
    }

    for (auto& th : threads) //this waits for the threads the complete there tasks
    {
        th.join();
    }

    sort(primes.begin(), primes.end()); //this creats a sort for the primes
    cout << "Total of Prime numbers found: " << primes.size() << endl; //this prints the total number of primes found

    return 0;
}