#ifndef WARE_H
#define WARE_H

typedef struct assistance_request AssistanceRequest;

typedef struct date Date;

typedef enum {
    DEVICE_SMARTPHONE,
    DEVICE_TABLET,
    DEVICE_LAPTOP,
    DEVICE_DESKTOP,
    DEVICE_PRINTER
} DeviceType;

typedef enum {
    PRIORITY_LOW,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH
} PriorityLevel;

typedef enum {
    STATUS_OPEN,
    STATUS_IN_PROGRESS,
    STATUS_CLOSED
} RequestStatus;

#endif //WARE_H