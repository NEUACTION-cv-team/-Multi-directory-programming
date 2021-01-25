#include "process_comm.h"
#include <iostream>

using namespace std;

int main()
{
    ProcessComm cvData;
    char buff[BUF_SIZE] = "hello"; 
    if(cvData.init((key_t)1024) == 0)
    {
        cout << "shared memory init done ..\n";
    }
    else
    {
        cout << "shared memory init fail ..\n";
    }
    
    while(true)
    {
        cvData.read(buff);
    }

}