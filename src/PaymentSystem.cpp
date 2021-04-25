#include "../include/PaymentSystem.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
int main(int argc, char **args)
{
    struct T_Payment *payment;
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
#pragma clang diagnostic pop

void get_shm_segments(int *shm_payment, struct T_Payment **p_payment)
{
    *shm_payment = shm_open(SHM_PAYMENT, O_RDWR, 0644);
    *p_payment = static_cast<T_Payment *>(mmap(NULL, sizeof(struct T_Payment),
                                               PROT_READ | PROT_WRITE, MAP_SHARED, *shm_payment, 0));
}

void get_sems(sem_t **p_sem_balance_ready, sem_t **p_sem_balance_charge)
{
    *p_sem_balance_ready = get_semaphore(SEM_BALANCE_READY);
    *p_sem_balance_charge = get_semaphore(SEM_BALANCE_CHARGE);
}

void get_and_process_task(sem_t *p_sem_balance_ready, sem_t *p_sem_balance_charge,
                          struct T_Payment *p_payment)
{
    wait_semaphore(p_sem_balance_ready);
    p_payment->balance = p_payment->client_initial_balance;
    std::cout << "[Payment System] Client " << p_payment->id << " Has been charge" << std::endl;
    signal_semaphore(p_sem_balance_charge);
}

