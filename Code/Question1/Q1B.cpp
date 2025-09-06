#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    int p[2]; //Creating the pipe

    if (pipe(p) < 0) //check to see if pipe has been created
    {
        cout << "Failed to create the pipe" << endl;
        exit(1);
    }

    pid_t x = fork(); //creating child process

    if (x == 0) //child reading
    {
        close(p[1]);
        char buffer[10];
        int a;

        while ((a = read(p[0], buffer, 10)) > 0) //this loop lets the child read all the messages
        {
            cout << "The parent said: " << buffer << endl;
        }
        
        cout << "The child has read all the messages." << endl;
        close(p[0]);
    }
    else //the else is the parent running
    {
        close(p[0]);
        write(p[1], "Message 1", 10); //parent is writing to the pipe
        write(p[1], "Message 2", 10);
        write(p[1], "Message 3", 10);
        write(p[1], "Message 4", 10);
        write(p[1], "Message 5", 10);
        close(p[1]);
        cout << "The parent has sent its messages." <<endl;
        wait(NULL); //Parent is waiting for child to be done.
    }

    return 0;
}