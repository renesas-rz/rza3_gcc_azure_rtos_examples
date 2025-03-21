#include "circular_buffer.h"

void circular_buffer_init(s_circular_buffer_t *self, uint8_t *p_data, size_t max_size)
{
    self->is_full = false;
    self->is_empty = true;
    self->head = 0;
    self->tail = 0;
    self->full_times = 0;
    self->max_size = max_size;
    self->p_buffer = p_data;
}

/* return the actual stored data size */
size_t circular_buffer_put_data(s_circular_buffer_t *self, uint8_t *p_data, size_t size)
{
    if (self->is_full)
        return size;

    self->is_empty = false;

    for (size_t i = 0; i < size; i++)
    {
        *(self->p_buffer + self->head) = *(p_data + i);
        self->head++;

        if (self->head == self->max_size)
            self->head = 0;

        if (self->head == self->tail)
        {
            self->is_full = true;
            self->full_times++;
            return size - i;
        }
    }
    return 0;
}

/* return the actual retrieved data size */
size_t circular_buffer_get_data(s_circular_buffer_t *self, uint8_t *p_data, size_t size)
{
    if (self->is_empty)
        return 0;

    self->is_full = false;

    for (size_t i = 0; i < size; i++)
    {
        *(p_data + i) = *(self->p_buffer + self->tail);
        self->tail++;

        if (self->tail == self->max_size)
            self->tail = 0;

        if (self->tail == self->head)
        {
            self->is_empty = true;
            /* i is index, plus 1 to return as size */
            return i + 1;
        }

    }
    return size;
}

size_t circular_buffer_get_remaining_capacity(s_circular_buffer_t *self)
{
    if (self->head < self->tail)
        return self->tail - self->head;

    return self->max_size - self->head + self->tail;
}
