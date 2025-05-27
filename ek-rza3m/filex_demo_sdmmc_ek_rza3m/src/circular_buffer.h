#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

typedef struct
{
    bool is_full;
    bool is_empty;
    uint8_t *p_buffer;
    size_t head;
    size_t tail;
    size_t max_size;
    uint32_t full_times;
} s_circular_buffer_t;

void circular_buffer_init(s_circular_buffer_t *self, uint8_t *p_data, size_t size);
size_t circular_buffer_put_data(s_circular_buffer_t *self, uint8_t *p_data, size_t size);
size_t circular_buffer_get_data(s_circular_buffer_t *self, uint8_t *p_data, size_t size);
size_t circular_buffer_get_remaining_capacity(s_circular_buffer_t *self);
#endif
