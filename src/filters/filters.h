#ifndef FILTERS_H
#define FILTERS_H
#include"ware.h"

const AssistanceRequestArray *filter_by_status(const AssistanceRequestArray * const arr, RequestStatus status);
const AssistanceRequestArray *filter_by_priority(const AssistanceRequestArray * const arr, PriorityLevel priority);
const AssistanceRequestArray *filter_by_customer_name(const AssistanceRequestArray * const arr, char *customer_name);


#endif //FILTERS_H