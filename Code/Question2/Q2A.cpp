#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstring>

using namespace std;

int main()
{
    if (mkfifo("FIFO", S_IFIFO | 0644) < 0) //creating the named pipe.
    {
        cout << "Pipe already created" << endl;
    }

    cout << getpid() << endl; //getting the pid number.

    int fdStopBlocking = open("FIFO", O_RDONLY | O_NONBLOCK); //this is a here as without i could not write and or read. It opens the pipe to stop blocks.
    
    if (fdStopBlocking < 0) //error check
    {
        perror("Non-blocking pipe error.");
    }
    

    int fdWrite = open("FIFO", O_WRONLY); //opening to write

    if (fdWrite < 0) //error check
    {
        perror("Error writing to FIFO.");
        return 1;
    }

    const char *data = "Hello, FIFO!"; //section writting the message into the pipe
    write(fdWrite, data, strlen(data) +1);
    close(fdWrite);
    cout << "data written into pipe." << endl;

    int fdRead = open("FIFO", O_RDONLY | O_NONBLOCK); //putting pipe to read

    char buffer[256]; //setting the buffer
    ssize_t bytes_read = read(fdRead, buffer, sizeof(buffer)); //putting the data/message into the buffer
    cout << buffer << endl; //outputting the data from the read
    close(fdRead);

    unlink("FIFO"); //unlinking the pipe

    return 0;
}