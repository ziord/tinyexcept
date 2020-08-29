#ifndef TINYEXCEPT_TEST_H
#define TINYEXCEPT_TEST_H

#include "defs.h"

// test try-except
void test_try_except();

// test try-except - return_
int test_try_except_return();

// test try-except with BaseException
void test_try_except_w_be();

// test try-except-finally
void test_try_except_finally();

// test try-except-else
void test_try_except_else();

// test try-except-else-finally
void test_try_except_else_finally();

// test try-except-re_raise
void test_try_except_re_raise();

// test try-except-raise_from
void test_try_except_re_from();

// test try-except-raise- finally
void test_try_except_raise_finally();

// test try-except-re_raise- finally
void test_try_except_re_raise_finally();

void test_try_except_re_raise_finally2();

void test_try_except_finally2();

void test_tinyexcept();

#endif //TINYEXCEPT_TEST_H
