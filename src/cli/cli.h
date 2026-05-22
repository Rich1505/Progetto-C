#ifndef CLI_H
#define CLI_H
#include"ware.h"

void show_startup_screen(void);
void run_main_menu(AssistanceRequest *list);
void show_error_message(const char *message);
void show_success_message(const char *message);
void show_request_message(const char *message);

#endif //CLI_H