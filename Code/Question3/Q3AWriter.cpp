#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

int main()
{
    key_t key;
    int shmid;

    if ((key = ftok(".", 'R')) == -1) //error check when creating key
    {
        cout << "Error when creating key." << endl;
    }

    shmid = shmget(key, 1024, 0644 | IPC_CREAT); //creating shared memory space

    if (shmid == -1) //error check for shared memory
    {
        cout << "Error while accessing shared memory space." << endl;
        exit(1);
    }

    char *data = (char *)shmat(shmid, (void *)0, 0); //attaching to shared memory space

    cout << "In put data: "; //cout to make it look better for the user
    cin.getline(data, 1024); //writing into shared memory getline to not lose data

    shmdt(data); //clean up by detaching from shared memory

    return 0;
}