// returns 1 if the function "store" uses little endian
// order to store value in the memory.
// otherwise returns 0
int is_little_endian(void (*store)(int*, int));
