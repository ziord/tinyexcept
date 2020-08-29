#ifndef TINYEXCEPT_TINYEXCEPT_DEF_H
#define TINYEXCEPT_TINYEXCEPT_DEF_H

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Exception {
    const char *cause;
} Exception;


enum {   // exception levels
    EXENTERED = 0,
    EXRAISED,
    EXHANDLED,
    EXFINALIZED
};


typedef
struct ExceptionFrame {
    Exception *exception;
    int ex_line;
    char *file_name;
    struct ExceptionFrame *prev_frame;
    jmp_buf env;
} ExceptionFrame;


static
void init_ExceptionFrame(ExceptionFrame *frame) {
    frame->exception = NULL;
    frame->ex_line = -1;
    frame->file_name = NULL;
    frame->prev_frame = NULL;
}


static
void init_exc_frame(
        ExceptionFrame *frame,
        Exception *exception,
        char *file, int line)
{
    frame->exception = exception;
    frame->ex_line = line;
    frame->file_name = file;
}


extern ExceptionFrame *EXStack;
Exception BaseException;

static
void init_EXStack() {
    init_exc_frame(EXStack, NULL, NULL, -1);
}


inline
static
void _exc__push_frame(ExceptionFrame *frame) {
    int is_new = 0;
    ExceptionFrame *e_frame = NULL;
    if (EXStack == NULL) {
        EXStack = malloc(sizeof(ExceptionFrame));
        init_EXStack();
        is_new = 1;
    }
    if (is_new)
        e_frame = EXStack;
    EXStack->prev_frame = frame;
    EXStack = frame;
    free(e_frame);
}


inline
static
void _exc__pop_frame() {
    EXStack = EXStack ? EXStack->prev_frame : EXStack;
}


inline
static
void exc_print_message(ExceptionFrame *frame,
        int should_abort)
{
    fprintf(stderr, "Traceback (most recent call last):\n");
    fprintf(stderr, "   File \"<%s>\", line <%d>\n",
            frame->file_name, frame->ex_line);
    if (frame->exception && frame->exception->cause)
        fprintf(stderr, "%s\n", frame->exception->cause);
    else
        fprintf(stderr, "Uncaught Exception at <%p>\n", frame->exception);
    should_abort ? fprintf(stderr, "\n\nAborting due to exception\n") : 0;
    fflush(stderr);
}

static
void raise_exception(
        Exception *exception,
        int line, char *filename,
        int should_abort,
        int _from_handler)
{
    assert(exception);  // exception cannot be NULL
    ExceptionFrame *curr_frame = EXStack;
    if (curr_frame == NULL) {
        if (_from_handler) {
            fprintf(stderr, "\nDuring handling of a previous exception, "
                            "another exception occurred:\n\n");
        }
        exc_print_message(&(ExceptionFrame)
                {.exception=exception, .ex_line=line, .file_name=filename}, should_abort);
        if (should_abort) abort();
        else return;
    }
    init_exc_frame(curr_frame, exception, filename, line);
    _exc__pop_frame();
    longjmp(curr_frame->env, EXRAISED);
}

static
void raise_exception_from(
        Exception *prev_exception,
        int prev_line, char *prev_filename,
        Exception *exception,
        int line, char *filename,
        int _from_handler)
{
    assert(prev_exception);
    assert(exception);
    raise_exception(prev_exception,
                    prev_line, prev_filename, 0, 0);
    if (_from_handler) {
        fprintf(stderr, "\nDuring handling of the above exception, "
                        "another exception occurred:\n\n");
    }else{
        fprintf(stderr, "\nThe above exception was the "
                        "direct cause of the following exception:\n\n");
    }
    raise_exception(exception, line, filename, 1, 0);
}

int __in_t_exc__handler;

#define _LINE_NO        __LINE__
#define _FILE_NAME      __FILE__


#define  get_message()  (e.cause)


#define _pop_if_entered   \
                    if (__exc__flag == EXENTERED) _exc__pop_frame();


#define try                                                                                 \
                    do{                                                                     \
                        volatile int __exc__flag;                                           \
                        Exception e;  int __exc_occ = 0;                                    \
                        ExceptionFrame __exc__frame;                                        \
                        init_ExceptionFrame(&__exc__frame);                                 \
                        _exc__push_frame(&__exc__frame);                                    \
                        __exc__flag = setjmp(__exc__frame.env);                             \
                        if (__exc__flag == EXENTERED){


#define except(exc, ...)                                                                    \
                            _pop_if_entered                                                 \
                        }else if ((__exc__frame.exception == &(exc)) ||                     \
                                  (&BaseException == &(exc))){                              \
                                                                                            \
                             __exc__flag = EXHANDLED;                                       \
                             __exc_occ = 1;                                                 \
                             __in_t_exc__handler = 1;                                       \
                             e.cause = __exc__frame.exception->cause;


#define else_                                                                               \
                            _pop_if_entered                                                 \
                        }if (__exc__flag != EXRAISED && __exc_occ != 1){                    \
                            __exc__flag = EXHANDLED;                                        \
                            __in_t_exc__handler = 1;

#define finally                                                                             \
                            _pop_if_entered                                                 \
                        }{                                                                  \
                            if (__exc__flag == EXENTERED)                                   \
                                __exc__flag = EXFINALIZED;                                  \
                            __in_t_exc__handler = 1;


#define end_try                                                                                     \
                            _pop_if_entered                                                         \
                        }                                                                           \
                        if (__exc__flag == EXRAISED)                                                \
                            raise_exception(__exc__frame.exception,                                 \
                            __exc__frame.ex_line, __exc__frame.file_name, 1, __in_t_exc__handler);  \
                        __in_t_exc__handler =  0;                                           \
                     }while (0);


#define raise(e)        raise_exception(&(e), _LINE_NO, _FILE_NAME, 1,                      \
                            __in_t_exc__handler)

// work on this

#define re_raise        raise_exception_from(__exc__frame.exception,                        \
                            __exc__frame.ex_line, __exc__frame.file_name,                   \
                            __exc__frame.exception, _LINE_NO, _FILE_NAME, 1)

#define raise_from(e, exc)                                                                  \
                        raise_exception_from(__exc__frame.exception,                        \
                            __exc__frame.ex_line, __exc__frame.file_name,                   \
                            &(exc), _LINE_NO, _FILE_NAME, 0)                                \

#define return_          switch(_exc__pop_frame(), 0);                                      \
                         return

#endif //TINYEXCEPT_TINYEXCEPT_DEF_H
