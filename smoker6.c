// Max Noddings
// Professor Trono
// Agent Smoker Programming Project - Smoker 6 Process
// Due December 2nd, 2022

// need to include the following to allow programs to compile
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h> // for inter-process communication
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    // initialize array of 4 semaphores
    int ret, ingredient, agent, smoker, activeSmokers, smokeTimes;
    
    // set random
    srand(time(NULL));
    
    // assign the value of this smokers ingredient to a variable
    int myIngredient = atoi(argv[1]);
    // do some error handling for the input so the program only takes in specific input.
    if ((myIngredient != 1) && (myIngredient != 2) && (myIngredient != 3))
    {
        perror("(FROM SMOKER) invalid command entered for the smoker process, the number associated with a smoker process should be either 1, 2, or 3\nTERMINATING NOW\n");
        exit(-1);
    }
    if ((argc > 2) || (argc < 1))
    {
        perror("(FROM SMOKER) invalid command entered for the smoker process\nTERMINATING NOW\n");
        exit(-1);
    }
    
    printf("(FROM SMOKER %d) starting process...\n", myIngredient);
    printf("--------------------------------------------------\n");
    fflush(stdout);

    // CONNECT TO SEMAPHORES----------------------------------------------------------------------------------------------------------------
    // ingredient
    ingredient = semget(43, 1, IPC_CREAT | 0600);
    if (ingredient < 0)
    {
        perror("(FROM SMOKER) Error with semget trying to create an array of 1 semaphore for ingredient semaphore\n");
        exit(-1);
    }
    // agent
    agent = semget(44, 1, IPC_CREAT | 0600);
    if (agent < 0)
    {
        perror("(FROM SMOKER) Error with semget trying to create an array of 1 semaphore for agent semaphore\n");
        exit(-1);
    }
    // smoker
    smoker = semget(45, 1, IPC_CREAT | 0600);
    if (smoker < 0)
    {
        perror("(FROM SMOKER) Error with semget trying to create an array of 1 semaphore for smoker semaphore");
        exit(-1);
    }
    // activeSmokers
    activeSmokers = semget(46, 1, IPC_CREAT | 0600);
    if (activeSmokers < 0)
    {
        perror("(FROM SMOKER) Error with semget trying to create an array of 1 semaphore for activeSmokers semaphore\n");
        exit(-1);
    }
    // smokeTimes
    smokeTimes = semget(47, 1, IPC_CREAT | 0600);
    if (smokeTimes < 0)
    {
        perror("(FROM SMOKER) Error with semget trying to create an array of 1 semaphore for smokeTimes semaphore\n");
        exit(-1);
    }
    // ----------------------------------------------------------------------------------------------------------------------------------

    // SETTING UP P() AND V() OPERATIONS ------------------------------------------------------------------------------------------------
    printf("(FROM SMOKER %d) setting up P() and V() operations\n", myIngredient);
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

    // INCREMENT ACTIVESMOKERS ----------------------------------------------------------------------------------------------------------
    // V(activeSmokers); incriment activeSmokers to show that a new smoker has been added
    ret = semop(activeSmokers, &signal, 1);
    if (ret < 0)
    {
        perror("(FROM SMOKER) Error with semop\n");
        exit(-1);
    }
    // Print out to make sure it worked
    ret = semctl(activeSmokers, 0, GETVAL, (void*) 0);
    if (ret < 0)
    {
        perror("(FROM SMOKER) Error with semctl\n");
        exit(-1);
    }
    // print out ret to see the value that the semahore was set to
    printf("(FROM SMOKER %d) activeSmokers: %d\n", myIngredient, ret);
    fflush(stdout);
    // ----------------------------------------------------------------------------------------------------------------------------------

    // While activeSmokers in less than 3 pause here ------------------------------------------------------------------------------------
    while (semctl(activeSmokers, 0, GETVAL, (void *) 0) < 3)
    {
        ; // Pause here
    }
    // ----------------------------------------------------------------------------------------------------------------------------------

    for (;;)
    {
        // P(smoker); pause here until agent wakes up smoker
        ret = semop(smoker, &wait, 1);
        if (ret < 0)
        {
            perror("(FROM SMOKER) Error with semop\n");
            exit(-1);
        }
        printf("(FROM SMOKER %d) I just woke up!\n", myIngredient);
        fflush(stdout);

        // if smokeTimes >= 13
        if (semctl(smokeTimes, 0, GETVAL, (void*) 0) >= 13)
        {
            printf("(FROM SMOKER %d) smokeTimes is >= 13, exiting program\n", myIngredient);
            fflush(stdout);
            // if activeSmokers <= 1
            if (semctl(activeSmokers, 0, GETVAL, (void*) 0) <= 1)
            {
                // TERMINATE ALL SEMAPHORES
                printf("(FROM SMOKER %d) TERMINATING ALL SEMAPHORES\n", myIngredient);
                fflush(stdout);
                // remove the ingredient semaphore
                ret = semctl(ingredient, 1, IPC_RMID, (void*) 0);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semctl\n");
                    exit(-1);
                }
                // remove the agent semaphore
                ret = semctl(agent, 1, IPC_RMID, (void*) 0);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semctl\n");
                    exit(-1);
                }
                // remove the smoker semaphore
                ret = semctl(smoker, 1, IPC_RMID, (void*) 0);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semctl\n");
                    exit(-1);
                }
                // remove the activeSmokers semaphore
                ret = semctl(activeSmokers, 1, IPC_RMID, (void*) 0);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semctl\n");
                    exit(-1);
                }
                // remove the smokeTimes semaphore
                ret = semctl(smokeTimes, 1, IPC_RMID, (void*) 0);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semctl\n");
                    exit(-1);
                }
                
                printf("(FROM SMOKER %d) DONE\n", myIngredient);
                fflush(stdout);

                // exit
                exit(-1);
            }
            // else
            else
            {
                // activeSmokers -= 1 (decrement the amount of active smokers)
                ret = semop(activeSmokers, &wait, 1);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semop\n");
                    exit(-1);
                }
                ret = semctl(activeSmokers, 0, GETVAL, (void *) 0);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semop\n");
                    exit(-1);
                }
                printf("(FROM SMOKER %d) terminating process, activeSmoker value is now: %d\n", myIngredient, ret);
                printf("(FROM SMOKER %d) signaling another smoker to wake up!\n\n", myIngredient);
                fflush(stdout);

                // V(smoker); alert another smoker
                // Increment semaphore
                ret = semop(smoker, &signal, 1);
                if (ret < 0)
                {
                    perror("(FROM SMOKER) Error with semop signaling the smoker semaphore\n");
                    exit(-1);
                }


                // exit
                exit(-1);
            }
        }

        // if ingridient semaphore == my ingridient (specified through command line)
        if (semctl(ingredient, 0, GETVAL, (void*) 0) == myIngredient)
        {
            printf("(FROM SMOKER %d) ingridient semaphore == my ingredient so I am going to smoke now...\n", myIngredient);
            // smoke() (for some random amount of time)
            printf("(FROM SMOKER %d) Smoking right now!\n", myIngredient);
            fflush(stdout);
            sleep(5);
            printf("(FROM SMOKER %d) Done smoking\n",  myIngredient);
            fflush(stdout);

            // V(smokeTimes); increment smokeTimes semaphore
            ret = semop(smokeTimes, &signal, 1);
            if (ret < 0)
            {
                perror("(FROM SMOKER) Error with semop incrementing the smokeTimes semaphore\n");
                exit(-1);
            }
            ret = semctl(smokeTimes, 0, GETVAL, (void*)0);
            if (ret < 0)
            {
                perror("(FROM SMOKER) Error with semctl\n");
                exit(-1);
            }
            printf("(FROM SMOKER %d) smokeTimes semaphore value: %d\n", myIngredient, ret);
            fflush(stdout);

            // V(agent); alert the agent that the smoker is done smoking
            ret = semop(agent, &signal, 1);
            if (ret < 0)
            {
                perror("(FROM SMOKER) Error with semop incrementing the agent semaphore\n");
                exit(-1);
            }
            printf("(FROM SMOKER %d) signaling the agent to wake up...\n\n", myIngredient);
            fflush(stdout);
        }
        // else (the ingridient isn't what I need)
        else
        {
            printf("(FROM SMOKER %d) ingredient semaphore doesn't equal my ingredient so going to signal another smoker to wake up...\n", myIngredient);
            fflush(stdout);
            // V(smoker); (alert another smoker to wake up)
            ret = semop(smoker, &signal, 1);
            if (ret < 0)
            {
                perror("(FROM SMOKER) Error with semop signaling the smoker semaphore\n");
                exit(-1);
            }

            // sleep(3); (sleep for 3 time units to make sure that another smoker wakes up an not the same one)
            printf("(FROM SMOKER %d) going to sleep...\n\n", myIngredient);
            fflush(stdout);
            sleep(rand() % 15 + 1); // sleeps for a random time between 1 and 15 time units
        }
    }
}

