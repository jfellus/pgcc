/*
 * sync_semaphores.h
 *
 *  Created on: 6 avr. 2015
 *      Author: jfellus
 */

#ifndef SYNC_SEMAPHORES_H_
#define SYNC_SEMAPHORES_H_

#include <semaphore.h>

#define SEM_DECL(a,b) sem_t sem_recv_##a##_to_##b; sem_t sem_send_##a##_to_##b
#define SEM_INIT(a,b) sem_init(&sem_recv_##a##_to_##b, 0, 0); sem_init(&sem_send_##a##_to_##b, 0, 0);
#define SEM_SEND(a,b) sem_post(&sem_send_##a##_to_##b); sem_wait(&sem_recv_##a##_to_##b);
#define SEM_RECV(a,b) sem_wait(&sem_send_##a##_to_##b); sem_post(&sem_recv_##a##_to_##b);


#endif /* SYNC_SEMAPHORES_H_ */
