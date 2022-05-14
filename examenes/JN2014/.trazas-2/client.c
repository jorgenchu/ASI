#include <stdio.h>
#include <stdlib.h>

int pidMonitor;


int main(int argc, char const *argv[])
{

    pidMonitor =  atoi(argv[1]);
    printf("%d", pidMonitor);
    
    return 0;
}
