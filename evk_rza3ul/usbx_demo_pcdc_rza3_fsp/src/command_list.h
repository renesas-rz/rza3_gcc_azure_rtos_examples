#ifndef COMMAND_LIST_H_
#define COMMAND_LIST_H_

#include "ux_api.h"
#include "ux_device_class_cdc_acm.h"

/* Defines the data size of the command and response. */
#define WORD_LENGTH (100)
/* Definition of the number of commands and responses. */
#define COMMAND_NUM (3)

/* A structure with two types of array elements that stores the command and its response. */
typedef struct data_command{
    UCHAR command_list[WORD_LENGTH];
    UCHAR response_list[WORD_LENGTH];
}COMMAND;

#endif  //COMMAND_LIST_H_
