#ifndef WARE_H
#define WARE_H

#define MAX_CUSTOMER_NAME 50
#define MAX_DESCRIPTION 200
#define WARE_INVALID_REQUEST_CODE -1

typedef struct assistance_request AssistanceRequest;

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

// SETTER
int set_request_code(AssistanceRequest *request, int request_code);

int set_customer_name(AssistanceRequest *request, const char *customer_name);

int set_device_type(AssistanceRequest *request, DeviceType device_type);

int set_description(AssistanceRequest *request, const char *description);

int set_priority_level(AssistanceRequest *request, PriorityLevel priority_level);

int set_request_status(AssistanceRequest *request, RequestStatus request_status);

int set_estimated_cost(AssistanceRequest *request, float estimated_cost);

int set_final_cost(AssistanceRequest *request, float final_cost);

int set_opening_date(AssistanceRequest *request, Date opening_date);

#endif // WARE_H