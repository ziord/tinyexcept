#include "defs.h"

// test try-except
void test_try_except() {
    try {
           unsigned int x = get_data(60);
        }
    except(BadDataRequest, e)
        {
            printf("%s\n", e.cause);
        }
    end_try
}

// test try-except with BaseException
void test_try_except_w_be() {
    try{
            unsigned int x = get_data(60);
        }
    except(BaseException)
        {
            printf("%s\n", get_message());
        }
     end_try
}

// test try-except-finally
void test_try_except_finally(){
    try{
        unsigned int x = get_data(60);
    }except(BadAlloc){  // use of wrong Exception handler to observe effect of finally
        printf("%s\n", get_message());
    }finally{
        printf("In finally!\n");
    }end_try
}

// test try-except-else
void test_try_except_else(){
    char* x = NULL;
    try{
        x = allocate_mem(60);
    }except(BadAlloc){
        printf("%s\n", get_message());
    }else_{
        printf("In else!\n");
        free(x);
    }end_try
}

// test try-except-else-finally
void test_try_except_else_finally(){
    char* x = NULL;
    try{
        x = allocate_mem(60);
    }except(BadDataRequest){
        printf("%s\n", get_message());
    }else_{
        printf("In else again!\n");
    }finally{
        printf("cleaning up!\n");
        free(x);
    }end_try
}

// test try-except-re_raise
void test_try_except_re_raise(){
    char* x = NULL;
    try{
        x = allocate_mem(60000000000000);
    }except(BadAlloc){
        printf("%s\n", get_message());
        re_raise;
    }end_try
}

// test try-except-raise_from
void test_try_except_re_from(){
    try{
        unsigned int x = get_data(60);
    }except(BadDataRequest, e){
        printf("%s\n", get_message());
        raise_from(e, BaseException);
    }end_try
}

// test try-except-raise- finally
void test_try_except_raise_finally(){
    char* x = NULL;
    try{
        x = allocate_mem(60000000000000);
    }except(BadAlloc, e){
        printf("%s\n", get_message());
        raise(e);
    }finally{
        printf("Couldn't clean up...\n");
    }end_try
}

// test try-except-re_raise- finally
void test_try_except_re_raise_finally(){
    try{
       unsigned int x = get_data(68);
    }except(BadDataRequest, e){
        printf("%s\n", get_message());
        re_raise;
    }finally{
        printf("Couldn't clean up...\n");
    }end_try
}

void test_try_except_re_raise_finally2(){
    char* x = NULL;
    try{
        x = allocate_mem(60000000000000);
    }except(BadAlloc, e){
        printf("%s\n", get_message());
        try{
            // re_raise/raise_from won't work here (inside a new
            //      try/except/end_try block within an except block)
            // the try/except/end_try block introduces a new exception scope.
            // the work around is to use raise (which works anywhere)
            // with the Exception object variable e
            raise(e);
        }except(BaseException){
            printf("Caught raise again!\n");
        }end_try
    }finally{
        printf("clean up...\n");
    }end_try
}

void test_try_except_finally2(){
    char* x = NULL;
    try{
        x = allocate_mem(60000000000000);
    }except(BadDataRequest, e){
        printf("%s\n", get_message());
    }finally{
        printf("clean up...\n");
        raise(BaseException);
    }end_try
}

void test_tinyexcept(){
    test_try_except();
    test_try_except_w_be();
    test_try_except_else();
    test_try_except_else_finally();

    // the commented functions would cause exceptions

    test_try_except_finally();
    test_try_except_re_raise();
    test_try_except_re_from();
    test_try_except_raise_finally();
    test_try_except_re_raise_finally();
    test_try_except_re_raise_finally2();
    test_try_except_finally2();
}