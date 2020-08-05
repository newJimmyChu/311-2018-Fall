#include "test_endian.h"
// this function returns 1 if the store function
// actually stores value in little endian order.
// otherwise returns 0.
int is_little_endian(void (*store)(int*,int)) {
  int x;
  // The cast below is to have an unsigned char pointer pointing
  // to the first byte of int variable x. You can use it
  // in your code to observe x one byte at a time.
  unsigned char *byte_ptr = (unsigned char*)(&x);
  // This is how you call store function.
  // After the function call, value 1 is stored
  // in the four bytes x is referring to in some order.
  // One of the four bytes will have the value 1, and the
  // other three bytes will have the value 0. 
  // You can call store function with any value that you think
  // will help you solve the problem.
  // Refer to code in Figure 2.4 on page 45 of the textbook
  // to see how you retrieve one byte at a time.
  
   store(&x, 0x12345678);  //test case
   return (byte_ptr[0]==0x78&&byte_ptr[1]==0x56&&byte_ptr[2]==0x34&&byte_ptr[3]==0x12);


   //if is little endian, return 1
   //else return 0


  // Replace 0 with a proper value that will be 0 when store
  // is not little endian and 1 when store is little endian.
  return 0;
}
