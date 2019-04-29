#include <stdlib.h>

#include "circular_buffer.h"

#define CIRCULAR_BUF_ADD(x, diff, size) ((x) = ((x) + (diff)) % (size + 1))
#define CIRCULAR_BUF_DIFF(s, e, size) ((e) >= (s) ? ((e) - (s)) : ((e) + (size) + 1 - (s)))

struct CIRCULAR_BUFFER_STRUCT
{
    int32_t *buf;
    uint32_t buf_size;
    uint32_t buf_w;
    uint32_t buf_r;
};

static uint32_t error_code = CIRCULAR_BUFFER_S_OK;

int32_t circular_buffer_create(CIRCULAR_BUFFER *p_cir, uint32_t buf_size)
{
    CIRCULAR_BUFFER p = NULL;

    if (NULL == p_cir || 0 == buf_size)
    {
        error_code = CIRCULAR_BUFFER_E_PARAM_ERR;
        return -1;
    }

    p = malloc(sizeof(struct CIRCULAR_BUFFER_STRUCT));
    if (NULL == p)
    {
        error_code = CIRCULAR_BUFFER_E_MEM_ERR;
        return -2;
    }

    p->buf = malloc((buf_size + 1) * sizeof(int32_t));
    if (NULL == p->buf)
    {
        error_code = CIRCULAR_BUFFER_E_MEM_ERR;
        return -3;
    }
    p->buf_size = buf_size;
    p->buf_w = 0;
    p->buf_r = 0;

    *p_cir = p;

    return 0;
}

int32_t circular_buffer_destory(CIRCULAR_BUFFER *p_cir)
{
    if (NULL == p_cir || NULL == *p_cir || NULL == (*p_cir)->buf)
    {
        error_code = CIRCULAR_BUFFER_E_PARAM_ERR;
        return -1;
    }

    free((*p_cir)->buf);
    free(*p_cir);
    *p_cir = NULL;
    return 0;
}

uint32_t circular_buffer_get_used_size(CIRCULAR_BUFFER cir)
{
    uint32_t used_size = CIRCULAR_BUF_DIFF(cir->buf_r, cir->buf_w, cir->buf_size);
    return used_size;
}

uint32_t circular_buffer_get_unused_size(CIRCULAR_BUFFER cir)
{
    uint32_t unused_size = cir->buf_size - circular_buffer_get_used_size(cir);
    return unused_size;
}

void circular_buffer_write_data(CIRCULAR_BUFFER cir, int32_t value)
{
    cir->buf[cir->buf_w] = value;
    CIRCULAR_BUF_ADD(cir->buf_w, 1, cir->buf_size);
    /* 如果缓冲区已写满，则覆盖写入 */
    if (cir->buf_w == cir->buf_r)
    {
        error_code = CIRCULAR_BUFFER_E_WRITE_FULL;
        CIRCULAR_BUF_ADD(cir->buf_r, 1, cir->buf_size);
    }
}

int32_t circular_buffer_read_data(CIRCULAR_BUFFER cir)
{
    int32_t res = cir->buf[cir->buf_r];
    /* 如果缓冲区已空，则读指针不再更新，同时读取数据为0 */
    if (cir->buf_w == cir->buf_r)
    {
        error_code = CIRCULAR_BUFFER_E_READ_EMPTY;
        return 0;
    }

    CIRCULAR_BUF_ADD(cir->buf_r, 1, cir->buf_size);
    return res;
}

void circular_buffer_clear_all(CIRCULAR_BUFFER cir)
{
    cir->buf_w = 0;
    cir->buf_r = 0;
}

uint32_t circular_buffer_get_error_code(CIRCULAR_BUFFER cir)
{
    return error_code;
}