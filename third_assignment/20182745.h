#ifndef STUDENT_ID_H
#define STUDENT_ID_H

#include "mystdio.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_PROCESSES 5
#define LINE_SIZE 256

// Function declarations
void function(const char *filepath);

// Main function to handle file reading and process creation
void function(const char *filepath) {
    FILE *file = fopen(filepath, "r");
    if (!file) {
        printf("Failed to open file\n");
        exit(1);
    }

    int pipes[NUM_PROCESSES][2];
    int i;
    pid_t pids[NUM_PROCESSES];
    char buffer[LINE_SIZE];
    int current_process = 0;

    // Initialize pipes for inter-process communication
    for (i = 0; i < NUM_PROCESSES; i++) {
        if (pipe(pipes[i]) == -1) {
            printf("pipe error\n");
            exit(1);
        }
    }

    // Create child processes
    for (i = 0; i < NUM_PROCESSES; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            printf("fork error\n");
            exit(1);
        }

        if (pids[i] == 0) {
            // Child process: close unused pipe ends
            for (int j = 0; j < NUM_PROCESSES; j++) {
                if (j != i) {
                    close(pipes[j][0]);
                    close(pipes[j][1]);
                }
            }
        
            close(pipes[i][1]); // Close the write end for this process
            while (1) {
                ssize_t bytes_read = read(pipes[i][0], buffer, sizeof(buffer) - 1);
                if (bytes_read > 0) {
                    buffer[bytes_read] = '\0'; // Null-terminate the buffer
                    printf("PID %d: %s\n", getpid(), buffer);
                } else break; // Exit loop if read returns 0 or an error
                
            }
            
            close(pipes[i][0]); // Close the read end after finishing reading
            exit(0);
        
        }
    }

    // Read each line using fgets and write to pipes
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        write(pipes[current_process][1], buffer, strlen(buffer)); // Write line to pipe
        current_process = (current_process + 1) % NUM_PROCESSES; // Update index for next process
        sleep(0.1);
    }
   
    // Close all write ends after sending all lines
    for (i = 0; i < NUM_PROCESSES; i++) {
        close(pipes[i][1]);
    }
    
    
    // Wait for all child processes to finish
    for (i = 0; i < NUM_PROCESSES; i++) {
        int pid = waitpid(pids[i], NULL, 0);
        if (i == current_process) {
            printf("PID %d: Read all data\n", pid);
        }
        printf("PID %d: I’m exiting...\n", pid);
    }

    fclose(file);
}

#endif // STUDENT_ID_H
