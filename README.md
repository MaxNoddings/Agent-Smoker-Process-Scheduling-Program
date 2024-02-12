# Agent-Smoker Process Scheduling Program

Solution to the classic "smoker-agent" synchronization problem! Programmed entirely in C. Simultaniously synchronizes agents and smokers using semaphores and mutexes. The programs run until there has been 13 smoking events that have taken place.

## Description of the Problem

A classic synchronization problem used to illustrate the challenges of inter-process communication and synchronization. The problem involves **4 total processes**: **3 processes representing the smokers (smoker6.c)** and **1 process representing the agent (agent6.c)** 

### 1. The Agent:
* Responsible for placing two different ingredients on a table.

### 2. The Smokers:
* There are three smokers, each with a unique ingredient. Let's say smoker A has ingredient X, smoker B has ingredient Y, and smoker C has ingredient Z.
* A smoker can make a cigarette if it has its unique ingredient and the ingredient placed by the agent on the table.

### 3. The Table:
* There is a shared table where the agent places the two different ingredients required to make a cigarette.

### 4. Rules:
* The agent continually places two random ingredients on the table.
* A smoker with the missing ingredient picks up the ingredients from the table and makes a cigarette.
* After smoking, the smoker signals the agent that it's ready for more ingredients.
* The agent then places two new random ingredients on the table, and the cycle continues.

### 5. Challenge
* The challenge is to synchronize the processes so that the smokers and the agent can work together without conflicts or race conditions.
