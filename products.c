/*
 * This is the file in which you will implement the functionality required by
 * the assignment.  Make sure to add your name and @oregonstate.edu email
 * address below:
 *
 * Name: Greggory Hickman
 * Email: hickmang@oregonstate.edu
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "products.h"
#include "dynarray.h"

#define MAX_STRING_LENGTH 100
#define MAX_INT_LENGTH 10

/*
 * This function should allocate and initialize a single product struct with
 * name, inventory, and price data.
 *
 * Params:
 *   name - a string containing the product's name.  Note that you'll have to
 *     make a copy of this string to store in the product struct instead of
 *     storing this string directly.
 *   inventory - an integer for how many of this product in the store
 *   price - the product's price
 *
 * Return:
 *   Should return a newly-allocated product structure whose fields are
 *   initialized with the values provided.
 */
struct product* create_product(char* name, int inventory, float price) {
  //Define & assert a new product pointer
  struct product* out = malloc(sizeof(struct product));
  assert(out);

  //Allocate & set each of its parameters with the specified inputs
  //Make sure all of the passed parameters exist
  assert(name);
  //Allocate memory for the name
  out->name = malloc(sizeof(char) * MAX_STRING_LENGTH);
  //Set parameters
  out->name = name;
  out->inventory = inventory;
  out->price = price;

  return out;
}


/*
 * This function should free all memory allocated by create_product() for a
 * single product.  This function must free all relevant memory and cannot
 * result in a memory leak.
 *
 * Params:
 *   product - a pointer to the product whose memory should be freed.  This
 *     function must free any memory allocated to the fields of this struct
 *     as well as memory allocated for the struct itself.
 */
void free_product(struct product* product) {
  assert(product);
  //Free the name, then the struct itself
  free(product->name);
  free(product);
  //Magnifique!
}


/*
 * This function should create a struct product for each product represented
 * in the information provided in the function arguments, and it should store
 * those products in a dynamic array (i.e. struct dynarray) allocated by this
 * function.  In particular, you should create a dynamic array and insert new
 * product structs into it, such that the i'th product in the array has the
 * i'th name, the i'th inventory, and the i'th price from the provided arrays
 * arguments.  You should use your create_product() function to allocate and
 * initialize each product struct stored in the array, and you should use the
 * provided dynamic array functions to allocate and work with the dynamic
 * array.  At the end of the function, you should return the dynamic array
 * with product structures stored in it.
 *
 * Params:
 *   num_products - the number of products to be stored in the newly allocated
 *     dynamic array
 *   names - an array of the names of the products to be stored in the dynamic
 *     array of product structs.  This array will have length num_products.
 *   inventory - an array of the inventory values, how many of the products in
 *   the store.
 *     array of product structs.  This array will have length num_products.
 *   prices - an array of the product prices to be stored in the dynamic
 *     array of product structs.  This array will have length num_products.
 *
 * Return:
 *   Should return a pointer to the newly allocated dynamic array containing
 *   newly-created product structs.  The i'th product in this array should have
 *   the i'th name, the i'th ID, and the i'th GPA from the arrays provided as
 *   arguments.
 */
struct dynarray* create_product_array(int num_products, char** names, int* inventory, float* prices) {
  //From now on, when you see asserts, just assume I'm making sure that no NULLs are being passed in.
  assert(names);
  assert(inventory);
  assert(prices);
  assert(num_products > -1);
  //Create the dynarray
  struct dynarray* out = dynarray_create();
  
  //Parse all of the passed information into product structs, and put them into the dynarray in order
  for (int i = 0; i < num_products; i++) {
    //Create a product,
    struct product* product = create_product(names[i], inventory[i], prices[i]);
    //Put it in the dynarray!
    dynarray_insert(out, i, product);
  }

  return out;
}


/*
 * This function should free all of the memory allocated to a dynamic array of
 * product structs, including the memory allocated to the array itself as
 * well as any memory allocated to the individual product structs.  You
 * should use free_product() to free the memory for each individual product,
 * and you should use provided dynamic array functions to free the memory
 * associated with the dynamic array itself.  This function must free *all*
 * memory associated with a dynamic array of products and must not result in
 * any memory leaks.
 *
 * Params:
 *   products - a pointer to the dynamic array of product structs whose memory
 *     is to be freed
 */
void free_product_array(struct dynarray* products) {
  assert(products);
  //Delete everything in the dynarray
  for (int i = 0; i < dynarray_length(products); i++) {
    //Get the i'th product from the dynarray
    struct product* victim = (struct product*)(dynarray_get(products, 0));
    //Relieve the victim of their data
    free_product(victim);
    //Relieve the victim of their existence
    dynarray_remove(products, 0);
    /*
     * By the way, the reason that I can use 0 instead of i for the dynarray 
     * product addresses is because the dynarray_remove() function automatically
     * shifts every other element inside to fill the gap that was left by each old 
     * victim, ensuring that the next "victim" is always located at address 0.
     */
  }

  //Finally, delete the dynarray itself
  dynarray_free(products);
}


/*
 * This function should print the name, inventory, and price of products in an
 * array, one product per line.  You must use provided dynamic array functions
 * to access the product data stored in the array.
 *
 * Params:
 *   products - the dynamic array of products to be printed
 */
void print_products(struct dynarray* products) {
  //Print all of everything out in order from the first element to the last
  for (int i = 0; i < dynarray_length(products); i++) {
    struct product* diva = dynarray_get(products, i); //This is the i'th product
    printf("  - name: %-13s\tinventory: %-4d\tprice: %f\n", diva->name, diva->inventory, diva->price);
  }
}


/*
 * This function should return a pointer to the product in a given array with
 * the highest price.  You should not make a copy of the product being returned,
 * i.e. this function should not allocate any memory.  Instead, you should
 * simply return the pointer to the product struct that's already stored in
 * the array.  You must use the provided dynamic array functions to access
 * the data stored in the array.
 *
 * Params:
 *   products - the array from which to find the product with the highest price
 *
 * Return:
 *   Should return a pointer to the product in the array with the highest price.
 *   You should not make a copy of the product being returned but should
 *   instead return the pointer to the product struct that's already stored in
 *   the array.
 */
struct product* find_max_price(struct dynarray* products) {
//Very basic maximum-finding algorithm because I can't be bothered to implement a super efficient but complicated one
  float max = 0;
  int maxAddress = 0;
  for (int i = 0; i < dynarray_length(products); i++) {
    float currentPrice = ((struct product*)(dynarray_get(products, i)))->price;
    //If the i'th price is larger than the current max price...
    if (currentPrice > max) {
      //...then replace the current max with the i'th price!
      max = currentPrice;
      maxAddress = i;
    }
  }

  //Return the product which was found to have the highest price
  return (struct product*)(dynarray_get(products, maxAddress));
}


/*
 * This function should return a pointer to the product in a given array with
 * the largest investment, defined as the product of inventory and price.
 * investment = inventory*price, meaning how many does the store have times
 * how many
 * You should not make a copy of the product being returned,
 * i.e. this function should not allocate any memory.  Instead, you should
 * simply return the pointer to the product struct that's already stored in
 * the array.  You must use the provided dynamic array functions to access
 * the data stored in the array.
 *
 * Params:
 *   products - the array from which to find the product with the lowest price
 *
 * Return:
 *   Should return a pointer to the product in the array with the highest 
 *   investment = price*investment.
 *   You should not make a copy of the product being returned but should
 *   instead return the pointer to the product struct that's already stored in
 *   the array.
 */
struct product* find_max_investment(struct dynarray* products) {
  //This algorithm is the same one as in find_max_price but with certain things edited
  float max = 0;
  int maxAddress = 0;
  for (int i = 0; i < dynarray_length(products); i++) {
    float currentPrice = ((struct product*)(dynarray_get(products, i)))->price;
    float currentInventory = ((struct product*)(dynarray_get(products, i)))->inventory;
    float currentInvestment = currentPrice * currentInventory;
    //If the i'th price is larger than the current max price...
    if (currentInvestment > max) {
      //...then replace the current max with the i'th price!
      max = currentInvestment;
      maxAddress = i;
    }
  }

  //Return the product which was found to have the highest price
  return (struct product*)(dynarray_get(products, maxAddress));
}


/*
 * This function should sort the products stored in a dynamic array by
 * ascending inventory (i.e. lowest inventory at the beginning of the array).
 * You may implement any sorting algorithm you want, with the following 
 * constraints:
 *   - You must sort in place, i.e. you can't allocate additional memory.
 *   - You may not use built-in sorting functions like qsort(), i.e. you must
 *     implement the sort yourself.  You may implement any in-place sorting
 *     algorithm you like.  Some possibilities are selection sort, insertion
 *     sort, bubble sort, and quicksort.
 *   - You must use provided dynamic array functions (e.g. dynarray_get(),
 *     dynarray_set(), etc.) to access and modify the contents of the dynamic
 *     array.
 *
 * Params:
 *   products - the dynamic array of products to be sorted.  When the function
 *   returns, this array should be sorted by ascending inventory (lowest to
 *   highest).
 */
void sort_by_inventory(struct dynarray* products) {
  int dynarrayLength = dynarray_length(products);
  int sorted = 1; //0 = false, 1 = true

  //For this, I'm going to use good 'ol bubble sort
  while (1) {
    sorted = 1;    

    for (int i = 0; i < dynarrayLength - 1; i++) {
      if (i+1 <= dynarrayLength) { //This if statement is just to really make sure that we avoid a seg fault
        int currentInv = ((struct product*)(dynarray_get(products, i)))->inventory;
        int nextInv = ((struct product*)(dynarray_get(products, i+1)))->inventory;
        
        //If the next value is lower than the current, swap them.
        if (nextInv < currentInv) {
          sorted = 0; //This boolean determines that the array is not sorted yet

          //Swap them!
          void* buffer = dynarray_get(products, i+1); //No need to cast to product*, so it shall stay as void*!
          dynarray_set(products, i+1, dynarray_get(products, i));
          dynarray_set(products, i, buffer);
        }

      }
    }

    if (sorted == 1) {
      break;
    }

  }
}
