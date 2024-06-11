#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#define N 5 // So ghe trong

sem_t Customers;
sem_t Barber;
pthread_mutex_t Seats;
int FreeSeats = N;


void* barber(void* arg) {
    while (1) {
		// Cho doi khach hang
        sem_wait(&Customers);
        
        // Giam bien mutex, bao ve so ghe kha dung
        pthread_mutex_lock(&Seats);
        printf("Barber is cutting hair.\n");
        // Ghe trong tang len
        FreeSeats++;
        
        printf("Number of customers in waitingroom: %d\n", 5 - FreeSeats);
        // Mang khach hang vao cat toc
        sem_post(&Barber);
        
        // Giai phong bien mutex
        pthread_mutex_unlock(&Seats);
        
        // Cat toc cho khach
        sleep(rand() % 2);
    }
}

void* customer(void* arg) {
    while (1) {
        //Bao ve ghe de chi co mot khach hang duoc ngoi vao ghe
        pthread_mutex_lock(&Seats);
        if (FreeSeats > 0) {
            // ngoi xuong
            FreeSeats--;
            printf("Number of customers in waitingroom: %d\n", 5 - FreeSeats);
            // Thong bao cho tho cat toc
            sem_post(&Customers);
            
            
            
            // Giai phong mutex
            pthread_mutex_unlock(&Seats);
            
            // Cho trong phong doi neu khach dang ban
            sem_wait(&Barber);
            // Khach hang duoc cat toc
            //printf("Customer is having a haircut.\n");
        } else {
            // Giai phong mutex
            pthread_mutex_unlock(&Seats);
            // Khach hang roi di
            printf("Customer leaves.\n");
        }
//        sleep(10);
        sleep(rand() % 5); // Gia lap thoi gian giua cac khach hang
    }
}

int main() {
	srand(time(0));
    pthread_t barberThread;
    pthread_t customerThreads[10];

    // Khoi tao semaphore va mutex 
    sem_init(&Customers, 0, 0);
    sem_init(&Barber, 0, 0);
    pthread_mutex_init(&Seats, NULL);
    
    // Tao thread cho tho cat toc
    pthread_create(&barberThread, NULL, barber, NULL);
    
    // Tao thread cho khach hang
    for (int i = 0; i < 10; i++) {
        pthread_create(&customerThreads[i], NULL, customer, NULL);
    }
    
    // Cho tho cat toc hoan thanh
    pthread_join(barberThread, NULL);
    
    // Cho cac khach hang hoan thanh
    for (int i = 0; i < 10; i++) {
        pthread_join(customerThreads[i], NULL);
    }
    
    // Huy semaphore va mutex
    sem_destroy(&Customers);
    sem_destroy(&Barber);
    pthread_mutex_destroy(&Seats);
    
    return 0;
}

