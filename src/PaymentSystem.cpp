#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#include "../include/PaymentSystem.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(int argc, char **args)
{
    struct TPayment *payment;
    int shm_payment;
    sem_t *p_sem_balance_ready, *p_sem_balance_charge;

    /* Get shared memory segments and semaphores */
    get_shm_segments(&shm_payment, &payment);
    get_sems(&p_sem_balance_ready, &p_sem_balance_charge);

    /* Will work until killed by the buscador */
    for(;;) {
        get_and_process_task(p_sem_balance_ready, p_sem_balance_charge, payment);
    }

    return EXIT_SUCCESS;
}

void get_shm_segments(int *shm_payment, struct TPayment **p_payment)
{
    *shm_payment = shm_open(SHM_PAYMENT, O_RDWR, 0644);
    *p_payment = static_cast<TPayment *>(mmap(NULL, sizeof(struct TPayment),
                                               PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));
}

void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge)
{
    *p_sem_balance_ready = get_semaphore(SEM_BALANCE_READY);
    *p_sem_balance_charge = get_semaphore(SEM_BALANCE_CHARGE);
}

void get_and_process_task(sem_t *p_sem_balance_ready, sem_t *p_sem_balance_charge,
                          struct TPayment *p_payment)
{
    wait_semaphore(p_sem_balance_ready);
    p_payment->balance = p_payment->client_initial_balance;
    std::cout << "[Payment System] Client " << p_payment->id << " Has been charge with " << p_payment->balance << " credits" << std::endl;
    signal_semaphore(p_sem_balance_charge);
}


#pragma clang diagnostic pop