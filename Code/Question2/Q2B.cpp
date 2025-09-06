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
        cout << "Pipe already created" << endl; //error check.
    }

    cout << getpid() << endl; //getting the pid number.

    pid_t x = fork(); //creating a child process.

    if (x == 0) //runing if process is child.
    {
        int fdRead = open("FIFO", O_RDONLY); //opening pipe to read.

        if (fdRead < 0)
        {
            perror("Error reading to FIFO."); //error check.
            return 1;
        }

        char buffer[256]; //creating buffer.
        ssize_t bytes_read = read(fdRead, buffer, sizeof(buffer)); //getting written data.
        cout << buffer << endl; //outputting data to screen.
        close(fdRead); //closing read end.
    }
    else //ruuning if parent.
    {
        int fdWrite = open("FIFO", O_WRONLY); //opeing write to pipe.

        if (fdWrite < 0)
        {
            perror("Error writing to FIFO."); //error check.
            return 1;
        }

        const char *data = "Hello, FIFO!"; //data to be sent.
        write(fdWrite, data, strlen(data) +1); //writing data to pipe.
        close(fdWrite); //closing write pipe.
        cout << "Data sent." << endl;
        wait(NULL); //waitng for child to finish.

        unlink("FIFO"); //unlinking the pipe.
    }

    return 0;
}