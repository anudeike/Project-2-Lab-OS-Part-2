#include "BENSCHILLIBOWL.h"

#include <assert.h>
#include <stdlib.h>
#include <time.h>

bool IsEmpty(BENSCHILLIBOWL* bcb);
bool IsFull(BENSCHILLIBOWL* bcb);
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

void CloseRestaurant(BENSCHILLIBOWL* bcb) {
    printf("Restaurant is closed!\n");
}

/* add an order to the back of queue */
int AddOrder(BENSCHILLIBOWL* bcb, Order* order) {
    return -1;
}

/* remove an order from the queue */
Order *GetOrder(BENSCHILLIBOWL* bcb) {
    return NULL;
}

// Optional helper functions (you can implement if you think they would be useful)
bool IsEmpty(BENSCHILLIBOWL* bcb) {
  return false;
}

bool IsFull(BENSCHILLIBOWL* bcb) {
  return false;
}

/* this methods adds order to rear of queue */
void AddOrderToBack(Order **orders, Order *order) {}

