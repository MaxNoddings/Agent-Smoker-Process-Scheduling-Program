// Max Noddings
// Professor Trono
// Agent Smoker Programming Project - Agent 6 Process
// Due December 2nd, 2022

// need to include the following to allow programs to compile
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h> // for inter-process communication
#include <time.h>
#include <unistd.h>

int main()
{
    printf("Starting AGENT\n");
    printf("--------------------------------------------------\n");
    fflush(stdout);

    // initialize array of 4 semaphores
    int ret, ingredient, agent, smoker, activeSmokers, smokeTimes;

    // set random
    srand(time(NULL));

    // CREATE SEMAPHORES----------------------------------------------------------------------------------------------------------------
    // Create the array of 1 semaphore for ingredient with 43 being the name of the semaphore. If mutex < 0, return error and exit.
    ingredient = semget(43, 1, IPC_CREAT | 0600);
    if (ingredient < 0)
    {
        perror("(FROM AGENT) Error with semget trying to create an array of 1 semaphore for ingredient semaphore\n");
        exit(-1);
    }
    // Create the array of 1 semaphore for agent with 44 being the name of the semaphore. If mutex < 0, return error and exit.
    agent = semget(44, 1, IPC_CREAT | 0600);
    if (agent < 0)
    {
        perror("(FROM AGENT) Error with semget trying to create an array of 1 semaphore for agent semaphore\n");
        exit(-1);
    }
    // Create the array of 1 semaphore for smoker with 45 being the name of the semaphore. If mutex < 0, return error and exit.
    smoker = semget(45, 1, IPC_CREAT | 0600);
    if (smoker < 0)
    {
        perror("(FROM AGENT) Error with semget trying to create an array of 1 semaphore for smoker semaphore");
        exit(-1);
    }
    // Create the array of 1 semaphore for activeSmokers with 46 being the name of the semaphore. If mutex < 0, return error and exit.
    activeSmokers = semget(46, 1, IPC_CREAT | 0600);
    if (activeSmokers < 0)
    {
        perror("(FROM AGENT) Error with semget trying to create an array of 1 semaphore for activeSmokers semaphore\n");
        exit(-1);
    }
    // Create the array of 1 semaphore for smoker with 47 being the name of the semaphore. If mutex < 0, return error and exit.
    smokeTimes = semget(47, 1, IPC_CREAT | 0600);
    if (smokeTimes < 0)
    {
        perror("(FROM AGENT) Error with semget trying to create an array of 1 semaphore for smokeTimes semaphore\n");
        exit(-1);
    }
    // ----------------------------------------------------------------------------------------------------------------------------------

    // ASSIGN VALUES TO SEMAPHORES (all except ingredient)-------------------------------------------------------------------------------
    ret = semctl(agent, 0, SETVAL, 0); // should return 0 if succesful, if not returns -1
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl assigning a value to agent semaphore\n");
        exit(-1);
    }
    ret = semctl(smoker, 0, SETVAL, 0); // should return 0 if succesful, if not returns -1
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl assigning a value to smoker semaphore\n");
        exit(-1);
    }
    ret = semctl(activeSmokers, 0, SETVAL, 0); // should return 0 if succesful, if not returns -1
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl assigning a value to activeSmoker semaphore\n");
        exit(-1);
    }
    ret = semctl(smokeTimes, 0, SETVAL, 0); // should return 0 if succesful, if not returns -1
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl assigning a value to smokeTimes semaphore\n");
        exit(-1);
    }
    // ----------------------------------------------------------------------------------------------------------------------------------

    // PRINTING OUT INITIAL VALUES OF SEMAPHORES ----------------------------------------------------------------------------------------
    // agent
    ret = semctl(agent, 0, GETVAL, (void*)0);
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl\n");
        exit(-1);
    }
    printf("(FROM AGENT) agent semaphore initial value: %d\n", ret);
    fflush(stdout);
    // smoker
    ret = semctl(smoker, 0, GETVAL, (void*)0);
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl\n");
        exit(-1);
    }
    printf("(FROM AGENT) smoker semaphore initial value: %d\n", ret);
    fflush(stdout);
    // activeSmokers
    ret = semctl(activeSmokers, 0, GETVAL, (void*)0);
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl\n");
        exit(-1);
    }
    printf("(FROM AGENT) activeSmokers semaphore initial value: %d\n", ret);
    fflush(stdout);
    // smokeTimes
    ret = semctl(smokeTimes, 0, GETVAL, (void*)0);
    if (ret < 0)
    {
        perror("(FROM AGENT) Error with semctl\n");
        exit(-1);
    }
    printf("(FROM AGENT) smokeTimes semaphore initial value: %d\n\n", ret);
    fflush(stdout);
    // ----------------------------------------------------------------------------------------------------------------------------------

    // SETTING UP P() AND V() OPERATIONS ------------------------------------------------------------------------------------------------
    printf("(FROM AGENT) setting up P() and V() operations\n\n");
    fflush(stdout);
    // two variables holding structures for semop()
    struct sembuf wait,signal;

    wait.sem_flg = 0; // sem_flg specifies to bock or not for P(), 0 = blocked, 1 = not blocked
    signal.sem_flg = 0;
    wait.sem_num = 0; // index of semaphore in the array to use
    signal.sem_num = 0;
    wait.sem_op = -1; // value to be added to the internal semaphore - the "integer incrementer"
    signal.sem_op = 1;
    // ----------------------------------------------------------------------------------------------------------------------------------

    // MAIN WHILE LOOP ------------------------------------------------------------------------------------------------------------------
    for (;;)
    {
        // set ingredient semaphore to a random value between 1 and 3
        ret = semctl(ingredient, 0, SETVAL, rand() % 3 + 1); // should return 0 if succesful, if not returns -1
        if (ret < 0)
        {
            perror("(FROM AGENT) Error with semctl assigning a value to ingredient semaphore\n");
            exit(-1);
        }
        // print out ingredient semaphore value
        ret = semctl(ingredient, 0, GETVAL, (void*)0);
        if (ret < 0)
        {
            perror("(FROM AGENT) Error with semctl\n");
            exit(-1);
        }
        // print out ret to see the value that the semahore was set to
        printf("(FROM AGENT) putting out a new ingredient on the table...\n");
        printf("(FROM AGENT) ingredient semaphore value: %d\n", ret);

        printf("(FROM AGENT) signaling a smoker process to wake up...\n");
        fflush(stdout);
        // V(smoker); (signal a smoker to wake up)
        ret = semop(smoker, &signal, 1);
        if (ret < 0)
        {
            perror("(FROM AGENT) Error with semop signaling the smoker semaphore\n");
            exit(-1);
        }

        // if smokeTimes >= 13
        if (semctl(smokeTimes, 0, GETVAL, (void*) 0) >= 13)
        {
            printf("(FROM AGENT) smokeTimes is >= 13, exiting program\n\n");
            fflush(stdout);
            // exit
            exit(1);
        }

        //P(agent); wait for the smoker to be done
        printf("(FROM AGENT) waiting for a signal from smoker...\n\n");
        fflush(stdout);
        ret = semop(agent, &wait, 1);
        if (ret < 0)
        {
            perror("(FROM SMOKER) Error with semop\n");
            exit(-1);
        }
        printf("(FROM AGENT)  I just woke up!\n");
        fflush(stdout);
    }
    // ----------------------------------------------------------------------------------------------------------------------------------
}
