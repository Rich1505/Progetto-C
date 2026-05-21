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
    DEVICE_SMARTPHONE,
    DEVICE_TABLET,
    DEVICE_LAPTOP,
    DEVICE_DESKTOP,
    DEVICE_PRINTER
} DeviceType;

typedef enum
{
    PRIORITY_LOW,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH
} PriorityLevel;

typedef enum
{
    STATUS_OPEN,
    STATUS_IN_PROGRESS,
    STATUS_CLOSED
} RequestStatus;

Date create_date(int, int, int);

AssistanceRequest *create_assistance_request(int, char *, DeviceType, char *, PriorityLevel, RequestStatus, float, float, Date);

int get_request_code(AssistanceRequest *);

char *get_customer_name(AssistanceRequest *);

DeviceType get_device_type(AssistanceRequest *);

char *get_description(AssistanceRequest *);

PriorityLevel get_priority_level(AssistanceRequest *);

RequestStatus get_request_status(AssistanceRequest *);

float get_estimated_cost(AssistanceRequest *);

float get_final_cost(AssistanceRequest *);

Date get_opening_date(AssistanceRequest *);

int set_request_code(AssistanceRequest *, int);

int set_customer_name(AssistanceRequest *, char *);

int set_device_type(AssistanceRequest *, DeviceType);

int set_description(AssistanceRequest *, char *);

int set_priority_level(AssistanceRequest *, PriorityLevel);

int set_request_status(AssistanceRequest *, RequestStatus);

int set_estimated_cost(AssistanceRequest *, float);

int set_final_cost(AssistanceRequest *, float);

int set_opening_date(AssistanceRequest *, Date);

#endif // WARE_H