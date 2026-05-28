#ifndef WARE_H
#define WARE_H

#define MAX_CUSTOMER_NAME 50
#define MAX_DESCRIPTION 200
#define WARE_INVALID_REQUEST_CODE -1

typedef struct assistance_request AssistanceRequest;
typedef struct assistance_request_array AssistanceRequestArray;

typedef struct date
{
    int day;
    int month;
    int year;
} Date;

typedef enum
{
    DEVICE_ERROR = -1,
    DEVICE_SMARTPHONE = 0,
    DEVICE_TABLET,
    DEVICE_LAPTOP,
    DEVICE_DESKTOP,
    DEVICE_PRINTER
} DeviceType;

typedef enum
{
    PRIORITY_ERROR = -1,
    PRIORITY_LOW = 0,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH
} PriorityLevel;

typedef enum
{
    STATUS_ERROR = -1,
    STATUS_OPEN = 0,
    STATUS_IN_PROGRESS,
    STATUS_CLOSED
} RequestStatus;

Date create_date(int day, int month, int year);

AssistanceRequest *create_assistance_request(int request_code, char *customer_name, DeviceType device_type, char *description,
                                             PriorityLevel priority_level, RequestStatus request_status, float estimated_cost, float final_cost, Date opening_date);

AssistanceRequestArray *create_assistance_request_array(AssistanceRequest **array, int size, int capacity);

int insert_assistance_request(AssistanceRequestArray *assistance_request_array, AssistanceRequest *assistance_request);

AssistanceRequestArray *copy_assistance_request_array(AssistanceRequestArray *src);

AssistanceRequest *copy_assistance_request(AssistanceRequest *src);

void free_assistance_request(AssistanceRequest *request);

void free_assistance_request_array(AssistanceRequestArray *arr);

// GETTER
int get_request_code(const AssistanceRequest *request);

const char *get_customer_name(const AssistanceRequest *request);

DeviceType get_device_type(const AssistanceRequest *request);

const char *get_description(const AssistanceRequest *request);

PriorityLevel get_priority_level(const AssistanceRequest *request);

RequestStatus get_request_status(const AssistanceRequest *request);

float get_estimated_cost(const AssistanceRequest *request);

float get_final_cost(const AssistanceRequest *request);

Date get_opening_date(const AssistanceRequest *request);

AssistanceRequest **get_assistance_request_array_ptr(AssistanceRequestArray *assistance_request_array);

int get_assistance_request_array_size(const AssistanceRequestArray * const assistance_request_array);

int get_assistance_request_array_capacity(AssistanceRequestArray *assistance_request_array);

// SETTER
int set_description(AssistanceRequest *request, const char *description);

int set_request_status_opened(AssistanceRequest *request);

int set_request_status_in_progress(AssistanceRequest *request);

int set_request_status_closed(AssistanceRequest *request);

int set_estimated_cost(AssistanceRequest *request, float estimated_cost);

int set_final_cost(AssistanceRequest *request, float final_cost);

#endif // WARE_H