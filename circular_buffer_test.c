#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

#include <stdint.h>

#define VALUE (0x5A5AA5A5)
#define BUF_SIZE (100U)

extern "C"
{
#include "circular_buffer.h"
}

int main(int argc, char *argv[])
{
    CommandLineTestRunner::RunAllTests(argc, argv);
    return 0;
}

/* 测试循环缓冲区创建 */
TEST_GROUP(circular_buffer_create)
{
    CIRCULAR_BUFFER cir;
    void setup()
    {
        cir = NULL;
    }

    void teardown()
    {
        ;
    }
};

TEST(circular_buffer_create, create_error_with_null_point)
{
    int32_t res = (0 > circular_buffer_create(NULL, BUF_SIZE) ? 0 : 1);
    CHECK_EQUAL(0, res);
}

TEST(circular_buffer_create, create_error_with_zero_buf_size)
{
    int32_t res = (0 > circular_buffer_create(&cir, 0) ? 0 : 1);
    CHECK_EQUAL(0, res);
}

TEST(circular_buffer_create, create_success_with_repeat_create)
{
    circular_buffer_create(&cir, BUF_SIZE);
    int32_t res = (0 > circular_buffer_create(&cir, BUF_SIZE) ? 0 : 1);
    CHECK_EQUAL(1, res);
}

TEST(circular_buffer_create, circular_buffer_no_used_data_after_create)
{
    circular_buffer_create(&cir, BUF_SIZE);
    CHECK_EQUAL(0, circular_buffer_get_used_size(cir));

    circular_buffer_create(&cir, 200);
    CHECK_EQUAL(0, circular_buffer_get_used_size(cir));
}

TEST(circular_buffer_create, circular_buffer_full_unused_data_after_create)
{
    circular_buffer_create(&cir, BUF_SIZE);
    CHECK_EQUAL(BUF_SIZE, circular_buffer_get_unused_size(cir));

    circular_buffer_create(&cir, 200);
    CHECK_EQUAL(200, circular_buffer_get_unused_size(cir));
}

/* 测试缓冲区销毁 */
TEST_GROUP(circular_buffer_destory)
{
    CIRCULAR_BUFFER cir;
    void setup()
    {
        cir = NULL;
        circular_buffer_create(&cir, BUF_SIZE);
    }

    void teardown()
    {
        ;
    }
};

TEST(circular_buffer_destory, destory_after_create)
{
    circular_buffer_destory(&cir);
    POINTERS_EQUAL(NULL, cir);
}

/* 测试缓冲区操作 */
TEST_GROUP(circular_buffer)
{
    CIRCULAR_BUFFER cir;
    void setup()
    {
        cir = NULL;
        circular_buffer_create(&cir, BUF_SIZE);
    }

    void teardown()
    {
        circular_buffer_destory(&cir);
    }
};

TEST(circular_buffer, point_no_empty_after_create)
{
    int32_t res = (NULL != cir ? 1 : 0);
    POINTERS_EQUAL(1, res);
}

TEST(circular_buffer, used_size_increment_after_write_data)
{
    circular_buffer_write_data(cir, VALUE);
    CHECK_EQUAL(1, circular_buffer_get_used_size(cir));
}

TEST(circular_buffer, write_and_read_data)
{
    circular_buffer_write_data(cir, VALUE);
    CHECK_EQUAL(1, circular_buffer_get_used_size(cir));
    LONGS_EQUAL(VALUE, circular_buffer_read_data(cir));
    CHECK_EQUAL(0, circular_buffer_get_used_size(cir));
}

TEST(circular_buffer, write_full_data)
{
    uint32_t i = 0;
    for (i = 0; i < BUF_SIZE; i++)
    {
        circular_buffer_write_data(cir, VALUE);
    }
    CHECK_EQUAL(0, circular_buffer_get_unused_size(cir));

    circular_buffer_write_data(cir, 0);
    CHECK_EQUAL(0, circular_buffer_get_unused_size(cir));
    LONGS_EQUAL(VALUE, circular_buffer_read_data(cir));
}

TEST(circular_buffer, read_empty_data)
{
    circular_buffer_clear_all(cir);
    circular_buffer_write_data(cir, VALUE);
    LONGS_EQUAL(VALUE, circular_buffer_read_data(cir));
    CHECK_EQUAL(0, circular_buffer_get_used_size(cir));

    CHECK_EQUAL(0, circular_buffer_read_data(cir));
    CHECK_EQUAL(0, circular_buffer_get_used_size(cir));
}

TEST(circular_buffer, clear_buffer_after_write_data)
{
    circular_buffer_write_data(cir, 100);
    circular_buffer_write_data(cir, 2000);
    circular_buffer_write_data(cir, 4000);
    circular_buffer_clear_all(cir);
    CHECK_EQUAL(0, circular_buffer_get_used_size(cir));
}

TEST_GROUP(circular_buffer_error)
{
    CIRCULAR_BUFFER cir;
    void setup()
    {
        cir = NULL;
    }

    void teardown()
    {
        ;
    }
};

TEST(circular_buffer_error, create_param_error)
{
    circular_buffer_create(NULL, BUF_SIZE);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_PARAM_ERR, circular_buffer_get_error_code(cir));

    circular_buffer_create(&cir, 0);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_PARAM_ERR, circular_buffer_get_error_code(cir));

    circular_buffer_create(NULL, 0);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_PARAM_ERR, circular_buffer_get_error_code(cir));
}

TEST(circular_buffer_error, destory_param_error)
{
    circular_buffer_create(&cir, BUF_SIZE);

    circular_buffer_destory(NULL);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_PARAM_ERR, circular_buffer_get_error_code(cir));

    CIRCULAR_BUFFER t_cir = NULL;
    circular_buffer_destory(&t_cir);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_PARAM_ERR, circular_buffer_get_error_code(cir));
}

/* 无法测出malloc失败效果 */
IGNORE_TEST(circular_buffer_error, mem_error)
{
    circular_buffer_create(&cir, 0xFFFFFFFF);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_MEM_ERR, circular_buffer_get_error_code(cir));
}

TEST(circular_buffer_error, read_empty_buffer_error)
{
    circular_buffer_create(&cir, BUF_SIZE);
    circular_buffer_read_data(cir);
    LONGS_EQUAL(CIRCULAR_BUFFER_E_READ_EMPTY, circular_buffer_get_error_code(cir));
}

TEST(circular_buffer_error, write_buff_buffer_error)
{
    uint32_t i = 0;
    circular_buffer_create(&cir, BUF_SIZE);
    for (i = 0; i < BUF_SIZE; i++)
    {
        circular_buffer_write_data(cir, VALUE);
    }
    circular_buffer_write_data
        LONGS_EQUAL(CIRCULAR_BUFFER_E_WRITE_FULL, circular_buffer_get_error_code(cir));
}