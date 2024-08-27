#include "buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define TRUE 1

buffer_item buffer[BUFFER_SIZE];
pthread_mutex_t mutex;
sem_t empty; // semaphore to keep track of empty slots in the buffer
sem_t full;  // semaphore to keep track of full slots in the buffer

int insertPointer = 0, removePointer = 0;

void *producer(void *param);
void *consumer(void *param);

int insert_item(buffer_item item)
{
  // Acquire Empty Semaphore
  sem_wait(&empty);

  // Acquire mutex lock to protect buffer
  pthread_mutex_lock(&mutex);
  buffer[insertPointer++] = item;
  insertPointer = insertPointer % BUFFER_SIZE;

  // print
  printf("\tremovePointer = %d, insertPointer = %d\n", removePointer, insertPointer);

  // Release mutex lock and full semaphore
  pthread_mutex_unlock(&mutex);
  sem_post(&full);

  return 0;
}

int remove_item(buffer_item *item)
{
  /* insert your code here for Task 1 (a) */

  /* Acquire Full Semaphore */
  sem_wait(&full);

  /* Acquire mutex lock to protect buffer */
  pthread_mutex_lock(&mutex);
  *item = buffer[removePointer];
  buffer[removePointer] = -1;
  removePointer = (removePointer + 1) % BUFFER_SIZE;

  // print
  printf("\tremovePointer = %d, insertPointer = %d\n", removePointer, insertPointer);

  /* Release mutex lock and empty semaphore */
  pthread_mutex_unlock(&mutex);
  sem_post(&empty);

  return 0;
}

int main(int argc, char *argv[])
{
  int i, j;

  if (argc != 4)
  {
    fprintf(stderr, "Usage: <sleep time> <producer threads> <consumer threads>\n");
    return -1;
  }

  /* insert you code below for Task 1 (b) */
  /* Get command line arguments argv[1], argv[2], argv[3]*/
  int sleepTime = atoi(argv[1]);
  int producerThreadNumber = atoi(argv[2]);
  int consumerThreadNumber = atoi(argv[3]);

  // Create arrays of thread ids
  pthread_t producer_thread_ids[producerThreadNumber];
  pthread_t consumer_thread_ids[consumerThreadNumber];

  /* Initialize the the locks */
  pthread_mutex_init(&mutex, NULL);
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);

  /* Create the producer and consumer threads */
  for (i = 0; i < producerThreadNumber; i++)
  {
    pthread_create(&producer_thread_ids[i], NULL, producer, NULL);
  }
  for (j = 0; j < consumerThreadNumber; j++)
  {
    pthread_create(&consumer_thread_ids[j], NULL, consumer, NULL);
  }

  // Sleep for user specified time
  sleep(sleepTime);

  return 0;
}

void *producer(void *param)
{
  buffer_item random;
  int r;

  while (TRUE)
  {
    r = rand() % 5;
    sleep(r);
    random = rand();

    if (insert_item(random))
    {
      fprintf(stderr, "Error");
    }
    else
    {
      printf("Producer produced %d \n", random);
    }
  }
}

void *consumer(void *param)
{
  buffer_item random;
  int r;

  while (TRUE)
  {
    r = rand() % 5;
    sleep(r);

    if (remove_item(&random))
    {
      fprintf(stderr, "Error Consuming");
    }
    else
    {
      printf("Consumer consumed %d \n", random);
    }
  }
}