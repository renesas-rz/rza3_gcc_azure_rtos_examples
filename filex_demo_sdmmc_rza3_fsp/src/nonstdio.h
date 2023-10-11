#ifndef NON_STD_IO_H_
#define NON_STD_IO_H_

#include <stdbool.h>

#if defined(__GNUC__) || defined(__CC_ARM)
__attribute__((format(printf, 1, 2)))
#endif
#define io_thread_printf printf
#if defined(__GNUC__) || defined(__CC_ARM)
__attribute__((format(printf, 1, 2)))
#endif
void io_thread_blocking_printf(const char *fmt, ...);

void io_thread_initializing_waiting(void);
#endif /* IO_THREAD_H_ */
