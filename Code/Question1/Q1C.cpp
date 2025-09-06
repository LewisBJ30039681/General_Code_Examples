#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring> //Had to add this as now using strings

using namespace std;

int main()
{
    const char *messages[] = {"Message1", "Message2", "Message3", "Message4", "Message5"}; //Setting the array for writing to the pipe.

    int p[2]; //Creating the pipe

    if (pipe(p) < 0) //check to see if pipe has been created
    {
        cout << "Failed to create the pipe" << endl;
        exit(1);
    }

    int p2[2]; //Creating the 2nd pipe

    if(pipe(p2) < 0) //check to see if the 2nd pipe has been created
    {
        cout << "Faild to create the 2nd pipe" << endl;
        exit(1);
    }

    pid_t x = fork(); //creating child process

    if (x == 0) //child reading
    {
        close(p[1]);
        char buffer[9]; //Buffer to better suit the messages
        int a;

        while ((a = read(p[0], buffer, 9)) > 0) //this loop lets the child read all the messages
        {
            cout << "The parent said: " << buffer << endl;
        }
        
        cout << "The child has read all the messages." << endl;
        close(p[0]);
        write(p2[1], "Done.", 6);
        close(p2[1]);
    }
    else //the else is the parent running
    {
        int numOfMesseages = sizeof(messages) / sizeof(messages[0]); //Calculating the elements in the array

        close(p[0]);

        for (int i = 0; i < numOfMesseages; i++) //Loop to write into the pipe
        {
            if (write(p[1], messages[i], strlen(messages[i]) + 1) == -1)
            {
                perror("Error writing to the pipe."); //Error check if pipe doesnt write
                exit(1);
            }
        }

        close(p[1]);
        cout << "The parent has sent its messages." <<endl;
        wait(NULL); //Parent is waiting for child to be done
        char buffer[6];
        read(p2[0], buffer, 6);
        cout << buffer << endl;
    }

    return 0;
}