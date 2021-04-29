
#include "../include/semaphoreI.h"

sem_t *create_semaphore (std::string name, unsigned int value) {
    sem_t *sem;

    if ((sem = sem_open(name.c_str(), O_CREAT, 0644, value)) == SEM_FAILED) {
        fprintf(stderr, "Error creating semaphore : %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    return sem;
}

sem_t *get_semaphore (std::string name) {
    sem_t *sem;

    if ((sem = sem_open(name.c_str(), O_RDWR)) == SEM_FAILED) {
        fprintf(stderr, "Error retrieving semaphore <%s>: %s\n", name.c_str(), strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    return sem;
}

void remove_semaphore (std::string name) {
    sem_t *sem = get_semaphore(name);

    if ((sem_close(sem)) == -1) {
        fprintf(stderr, "Error closing semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }

    if ((sem_unlink(name.c_str())) == -1) {
        fprintf(stderr, "Error removing semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

void signal_semaphore (sem_t *sem) {
    if ((sem_post(sem)) == -1) {
        fprintf(stderr, "Error incrementing the semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

void wait_semaphore (sem_t *sem) {
    if ((sem_wait(sem)) == -1) {
        fprintf(stderr, "Error decrementing the semaphore: %s\n", strerror(errno));
        std::exit(EXIT_FAILURE);
    }
}

