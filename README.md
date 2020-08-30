### tinyexcept

A header only C library for experimenting with exceptions, inspired by David Hanson.

****tinyexcept**** is modelled completely after Python's exception mechanism, with a few _exceptions_ :)
<br/>


### Installation
- Clone this repo
- Add both header files found in [include folder](https://github.com/ziord/tinyexcept/blob/master/include) to your include/source folder. 
- Include `tinyexcept.h` in your source file(s) for out of the box usage.
- For custom integration/extension, include `tinyexcept_def.h` in your header file(s) for extension, and `tinyexcept.h` in your source file(s).
<br/>

### Usage
tinyexcept allows creation of custom `Exception` objects, via the `Exception` `struct`.

**Examples**

```c
#include "tinyexcept.h"
// create Exception object
Exception BadStuff = {"Got bad stuff"};

int get_stuff(int x){
    if (x < 50){
        return x + 1;
    }else{
        raise(BadStuff);  // raise an exception
    }
}
```
And the caller of this function would handle this exception like this:

```c
try{
    int my_stuff = get_stuff(50);
}except(BadStuff, e){
    printf("%s\n", e.cause);
}end_try
```
The `e` argument to `except` is optional:

```c
try{
    int my_stuff = get_stuff(50);
}except(BadStuff){
    printf("%s\n", get_message());
}end_try
```
`get_message()` returns the cause of the exception, synonymous to `e.cause`.

`end_try` is _necessary_ for cleanup and implementation correctness.

tinyexcept also provides `BaseException` which can be used as a "catch-all" or generic `Exception` object.

```c
try{
    int my_stuff = get_stuff(50);
}except(BaseException){
    printf("%s\n", get_message());
}end_try
```

And of course there's `re_raise`, `raise_from`, `else_`, `return_` and `finally`.

`re_raise` and `raise_from` works within an `except` block (with an _exception_ for nested `try` blocks within an `except` block. See [tests](https://github.com/ziord/tinyexcept/blob/master/tests) for more details).

`re_raise` typically re-raises a previously raised exception, within that exception's `except` block:
 
```c
try{
    int my_stuff = get_stuff(50);
}except(BaseException){
    printf("%s\n", get_message());
    re_raise;   // oh no, bad stuff :(
}end_try
```
`raise_from` acts like Python's nifty exception statement:
```python 
raise Exception from AnotherException
```
That is:

```c
try{
    int my_stuff = get_stuff(50);
}except(BaseException, e){
    raise_from(e, AnotherException);   // raise AnotherException from previous exception 
}end_try
```

`else_`'s body is executed when no exception takes place:


 ```c
try{
    int my_stuff = get_stuff(10);
}except(BadStuff){
    printf("wheew! close one\n");
}else_{
    printf("wheew! Nothing happended\n");
}end_try
```

`finally` can be used for cleanups.

 ```c
try{
    int my_stuff = get_stuff(100);
}finally{
    printf("Cleaning up stuffs...\n");
}end_try
```

However, `finally` comes with a caveat. If exceptions are `re_raise`d or `raise`d inside an `except` block, `finally`'s block of code won't get executed.  
<br/>

`return_` should be used within `try`/`except`/`else_`/`finally`/`end_try` blocks to return values or return from functions, as it provides necessary cleanup before exiting the function.
That is:

```c
int get_value(){
    int my_stuff;
    try{
        my_stuff = get_stuff(100);
    }except(BaseException){
        my_stuff = 20;
        return_ my_stuff;
    }end_try
}
```

Or exiting:

```c
void do_something(){
    try{
        int my_stuff = get_stuff(100);
        printf("doing something... %d\n", my_stuff);
    }except(BaseException){
        printf("couldn't do anything\n");
        return_;
    }end_try
}
```
<br/>

### Limitations
This is all purely experimental, and not advisable to use in production code. 
Nesting exceptions may suck, but it kinda works.

Other issues not stated here may arise.
tinyexcept only works with custom exceptions, that is, one created by the user, or some other user.

Undefined behaviour **is not handled** by tinyexcept, your program is still susceptible to undefined behaviour.   
<br/>

### License

[MIT](https://github.com/ziord/tinyexcept/blob/master/LICENSE.txt)

