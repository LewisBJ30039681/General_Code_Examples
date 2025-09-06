#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

int main()
{
    const char *data = "I am data"; //message that will be sent in the program

    int p[2]; //creating the pipe

    if (pipe(p) < 0) //check to see if pipe has been created
    {
        cout << "Failed to create the pipe" << endl;
        exit(1);
    }

    write(p[1], data, 10); //Writing into the pipe
    cout << "Data has been written" << endl; //Meaage to make the code less stop and start
    close(p[1]); //closing the write pipe

    char inbuff[11] = {0}; //setting the buffer
    sleep(3); //sleeping for the progame to wait to read
    read(p[0], inbuff, 10); //reading the buffer that was the written data from the pipe.
    cout << inbuff << endl; //cout the buffer
    close(p[0]); //closing the read pipe

    cout << "data sent" << endl; //message for when everything is done

    return 0;
}