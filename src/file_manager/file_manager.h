#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H
#include"ware.h"

AssistanceRequest **read_from_memory(int *out_size);
int write_in_memory(AssistanceRequest **array, int size);

#endif //FILE_MANAGER_H