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

    run_main_menu(assistance_request_array);

    return 0;
}