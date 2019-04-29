#ifndef _CIRCULAR_BUFFER_H_
#define _CIRCULAR_BUFFER_H_

#include <stdint.h>

enum CIRCULAR_BUFFER_ERROR_CODE
{
    CIRCULAR_BUFFER_S_OK         = 0,
    CIRCULAR_BUFFER_E_PARAM_ERR  = 0x80000101,
    CIRCULAR_BUFFER_E_MEM_ERR    = 0x80000102,
    CIRCULAR_BUFFER_E_READ_EMPTY = 0x80000103,
    CIRCULAR_BUFFER_E_WRITE_FULL = 0x80000104,
};

typedef struct CIRCULAR_BUFFER_STRUCT *CIRCULAR_BUFFER;

int32_t circular_buffer_create(CIRCULAR_BUFFER *p_cir, uint32_t buf_size);

int32_t circular_buffer_destory(CIRCULAR_BUFFER *p_cir);

uint32_t circular_buffer_get_used_size(CIRCULAR_BUFFER cir);

uint32_t circular_buffer_get_unused_size(CIRCULAR_BUFFER cir);

void circular_buffer_write_data(CIRCULAR_BUFFER cir, int32_t value);

int32_t circular_buffer_read_data(CIRCULAR_BUFFER cir);

void circular_buffer_clear_all(CIRCULAR_BUFFER cir);

uint32_t circular_buffer_get_error_code(CIRCULAR_BUFFER cir);

#endif /* _CIRCULAR_BUFFER_H_ */