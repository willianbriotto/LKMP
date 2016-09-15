#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <unistd.h>

#define PROC_CPU "/proc/cpuinfo"
#define PROC_MEMINFO "/proc/meminfo"

void getCpuClockSpeed() 
{
    int fd;
    char buffer[1024];
    char* match;
    float clock_speed;

    fd = open(PROC_CPU, O_RDWR);
    if(fd == -1) {
        printf("not open the %s file\n", PROC_CPU);
        exit(-1);
    }
    printf("opened the %s file\n", PROC_CPU);

    read(fd, buffer, sizeof(buffer));
    match = strstr(buffer, "cpu MHz");
    if(match == NULL) {
        printf("no %s", PROC_CPU);
        return;
    }

    sscanf(match, "cpu MHz : %f\n", &clock_speed);
    printf("CPU clock speed: %4.0f MHz\n", clock_speed);

    close(fd);
}

void getMemFreeInfo()
{
    int fd;
    char buffer[1024];
    char* match;
    float memfree;

    fd = open("/proc/meminfo", O_RDWR);
    if(fd == -1) {
        printf("not open the %s file\n", PROC_MEMINFO);
        exit(-1);
    }
    printf("opened the %s file\n", PROC_MEMINFO);

    read(fd, buffer, sizeof(buffer));
    match = strstr(buffer, "MemFree");
    if(match == NULL) {
        printf("without MemFree found\n");
        return;
    }

    sscanf(match, "MemFree : %f", &memfree);
    printf("MemFree: %f\n", memfree);

    close(fd);
}

void *doInfo(void* tid) {
    getCpuClockSpeed();
    getMemFreeInfo();

    pthread_exit(NULL);
}

int main()
{
   pthread_t* threads;
   int t = 0;

   for(;;t++) {
       if(pthread_create(&threads, NULL, doInfo, (void *)t)) {
           printf("Error: Create thread error");
           exit(1);
       }
       printf("\n\n");
       sleep(2);
   }

   pthread_exit(NULL);
   return 0;
}
