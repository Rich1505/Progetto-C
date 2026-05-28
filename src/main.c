#include"file_manager.h"
#include"ware.h"
#include"sort.h"
#include<stdlib.h>
#include<stdio.h>
#include"search.h"
#include"cli.h"
#include"filter.h"

int main(void)
{
    AssistanceRequestArray *assistance_request_array = read_from_memory();

    sort_by_estimated_cost(assistance_request_array);

    filter_by_customer_name(assistance_request_array, "Mario Rossi");

    write_in_memory(assistance_request_array);

    return 0;
}