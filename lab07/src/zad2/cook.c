#include "common.h"

void place_in_oven(oven *o, int type)
{
    o->arr[o->to_place_idx] = type;
    o->to_place_idx++;
    o->to_place_idx = o->to_place_idx % OVEN_SIZE;
    o->pizzas++;
}

int take_out_pizza(oven *o)
{
    int type = o->arr[o->to_take_out_idx];
    o->arr[o->to_take_out_idx] = -1;
    o->to_take_out_idx++;
    o->to_take_out_idx = o->to_take_out_idx % OVEN_SIZE;
    o->pizzas--;
    return type;
}

void place_on_table(table *t, int type)
{
    t->arr[t->to_place_idx] = type;
    t->to_place_idx++;
    t->to_place_idx = t->to_place_idx % OVEN_SIZE;
    t->pizzas++;
}

int main()
{

    sem_t *oven_sem = get_sem(OVEN_SEM);
    sem_t *table_sem = get_sem(TABLE_SEM);
    sem_t *full_oven_sem = get_sem(FULL_OVEN_SEM);
    sem_t *full_table_sem = get_sem(FULL_TABLE_SEM);
    sem_t *empty_table_sem = get_sem(EMPTY_TABLE_SEM);

    int shm_oven_fd = get_shm_oven_fd();
    int shm_table_fd = get_shm_table_fd();

    oven *o = mmap(NULL, sizeof(oven), PROT_READ | PROT_WRITE, MAP_SHARED, shm_oven_fd, 0);
    table *t = mmap(NULL, sizeof(table), PROT_READ | PROT_WRITE, MAP_SHARED, shm_table_fd, 0);

    int type;

    srand(getpid());
    while (true)
    {

        type = rand() % 10;
        printf("[COOKER] (pid=%d time=%s): MAKING PIZZA=%d\n", getpid(), get_current_time(), type);
        sleep(PREPARATION_TIME);

        lock_sem(full_oven_sem);

        lock_sem(oven_sem);
        place_in_oven(o, type);
        printf("[COOKER] (pid=%d time=%s): ADDED PIZZA=%d. IN OVEN=%d\n", getpid(), get_current_time(), type, o->pizzas);
        unlock_sem(oven_sem);

        sleep(BAKING_TIME);
        lock_sem(oven_sem);
        type = take_out_pizza(o);
        printf("[COOKER] (pid=%d time=%s): REMOVED PIZZA=%d, IN OVEN=%d, ON TABLE=%d    \n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        unlock_sem(oven_sem);
        unlock_sem(full_oven_sem);

        lock_sem(full_table_sem);

        lock_sem(table_sem);
        place_on_table(t, type);
        printf("[COOKER] (pid=%d time=%s): PLACING PIZZA=%d, IN OVEN=%d, ON TABLE=%d\n", getpid(), get_current_time(), type, o->pizzas, t->pizzas);
        unlock_sem(table_sem);

        unlock_sem(empty_table_sem);
    }
}