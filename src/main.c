#include"file_manager.h"
#include"ware.h"
#include"sort.h"
#include<stdlib.h>
#include<stdio.h>
#include"search.h"

int main(void)
{

    AssistanceRequestArray *assistance_request_array = read_from_memory();
    write_in_memory(assistance_request_array);

    sort_by_estimated_cost(assistance_request_array);

    write_in_memory(assistance_request_array);

    AssistanceRequest *req = search_by_request_code(assistance_request_array, 103);

    return 0;
}