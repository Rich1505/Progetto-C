#include "ware.h"

#define MAX_CUSTOMER_NAME 50
#define MAX_DESCRIPTION 200

struct date
{
    int day;
    int month;
    int year;
};

struct assistance_request
{
    int request_code;
    char customer_name[MAX_CUSTOMER_NAME];
    DeviceType device_type;
    char description[MAX_DESCRIPTION];
    PriorityLevel priority_level;
    RequestStatus request_status;
    float estimated_cost;
    float final_cost;
    Date opening_date;
};