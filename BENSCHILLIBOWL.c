#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(BENSCHILLIBOWL* cb);
bool IsFull(BENSCHILLIBOWL* cb);
void AddOrderToBack(Order **orders, Order *order);

MenuItem BENSCHILLIBOWLMenu[] = { 
    "BensChilli", 
    "BensHalfSmoke", 
    "BensHotDog", 
    "BensChilliCheeseFries", 
    "BensShake",
    "BensHotCakes",
    "BensCake",
    "BensHamburger",
    "BensVeggieBurger",
    "BensOnionRings",
};

int BENSCHILLIBOWLMenuLength = 10;

/* Select a random item from the Menu and return it */
MenuItem PickRandomMenuItem() {
    return BENSCHILLIBOWLMenu[rand() % BENSCHILLIBOWLMenuLength];
}

/* Allocate memory for the Restaurant, then create the mutex and condition variables needed to instantiate the Restaurant */ 
// DONE
BENSCHILLIBOWL* OpenRestaurant(int max_size, int expected_num_orders) {

  // Allocate memory
    BENSCHILLIBOWL *cb= (BENSCHILLIBOWL*) malloc(sizeof(BENSCHILLIBOWL));
    cb->orders = NULL;
    cb->current_size = 0;
    cb->max_size = max_size;
	  cb->next_order_number = 1;
    cb->orders_handled = 0;
    cb->expected_num_orders = expected_num_orders;
  
    // generate the mutex. set the conditions that will be needed to create a new resturant in the separate thread
    pthread_mutex_init(&(cb->mutex), NULL);
    pthread_cond_init(&(cb->can_add_orders), NULL);
    pthread_cond_init(&(cb->can_get_orders), NULL);

    // notify that the resturant is open
    printf("Restaurant is open!\n");
    return cb;
}


/* check that the number of orders received is equal to the number handled (ie.fullfilled). Remember to deallocate your resources */
// DONE
void CloseRestaurant(BENSCHILLIBOWL* cb) {

  // check that the number of orders received is equal to the number handled
  // this translate into making sure they were all completed
    if (cb->orders_handled != cb->expected_num_orders) {
        fprintf(stderr, "Some of the orders were left unhandled.\n");
        exit(0);
    }
  
    // deallocate resources 
    pthread_mutex_destroy(&(cb->mutex));
    free(cb);

    printf("Restaurant is closed!\n");
}

/* add an order to the back of queue */
int AddOrder(BENSCHILLIBOWL* cb, Order* order){
   // lock so two things can't be added at the same time
    pthread_mutex_lock(&(cb->mutex)); 
  
    // the restaurant has reached maximum number of orders the restaurant can handle 
    // wait until the queue can take more orders
    while (IsFull(cb)) { 
        pthread_cond_wait(&(cb->can_add_orders), &(cb->mutex));
    }
    
    // add order to back 
    order->order_number = cb->next_order_number;
    AddOrderToBack(&(cb->orders), order);
  
    // update next order number and current size
    cb->next_order_number += 1; 
    cb->current_size += 1;
    
    // notify the process that it can get orders now
    pthread_cond_broadcast(&(cb->can_get_orders));
  
    // release lock
    pthread_mutex_unlock(&(cb->mutex));
    
    return order->order_number;
    return -1;
}

/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* cb) {
    pthread_mutex_lock(&(cb->mutex));
    
    // wait until the restaurant has any order to handle at the moment
    while(IsEmpty(cb)) { 
        // if expected number of orders have been fulfilled, 
        // unlock the mutex and return null (no more order will be taken).
        if (cb->orders_handled >= cb->expected_num_orders) {
            pthread_mutex_unlock(&(cb->mutex));
            return NULL;
        }
        pthread_cond_wait(&(cb->can_get_orders), &(cb->mutex));
    }
    
    // get order from queue(FIFO).
    Order *order = cb->orders;
    cb->orders = cb->orders->next;
    
    // update the current order size and orders handled
    cb->current_size -= 1; 
    cb->orders_handled += 1;
    
    // notify the process that it add get orders now
    pthread_cond_broadcast(&(cb->can_add_orders));
        
    // release the lock.
    pthread_mutex_unlock(&(cb->mutex));   
    return order;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(BENSCHILLIBOWL* cb) {
  if (cb->current_size == 0){
    return true;
  }
  else{
    return false;
  }
}

bool IsFull(BENSCHILLIBOWL* cb) {
  if (cb->current_size == cb->max_size){
    return true;
  }
  else{
    return false;
  }
  return false;
}

/* this methods adds order to rear of queue */
void AddOrderToBack(Order **orders, Order *order) {

  // if its empty
  if (*orders == NULL) {
      *orders = order;
  } 
  else {
    // this is like a linked list
      Order *curr_order = *orders;
      while (curr_order->next) {
          curr_order = curr_order->next;
      }
      curr_order->next = order;
    }
}

