# dpl
This is a project that using the c programming language to build own programming language.

My programming language grammar is similar to javascript, I name it with dpl with the extension .stc. 

dpl is the exacutable file that compile the .stc extension file.

`./dpl filname.stc` to compile the scripts.

Using the makefile and .stc file to understand my grammar

There are also some test cases

```
//anonymous function test

println("test the lambda function :");
println(
    lambda(x) {
        x - 1;
    } (6)
);
println(
    lambda(y){ 
        y + 3;
    }(4)
);

//anonymous function test
```
