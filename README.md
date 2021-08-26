# dpl
This is a project that using the c programming language to build own programming language.

My programming language grammar is similar to javascript, I name it with dpl with the extension .stc. 

dpl is the exacutable file that compile the .stc extension file.

`./dpl filname.stc` to compile the scripts.

Using the makefile and .stc file to understand my grammar

This language is similar to C with the dynamic variable definition like python.
Hello world program:
```
println("Hello world!"); // println
```
comments
```
// this is comments 
```
variable definition
```
x = 1; // define variable 
a = array(size); // define array
```
loop:
```
while(true){ code block } // while loop 
for (i=1;i<10;i++){ code block} // for loop
```
condition:
```
if {code block} else {code block} // if else condition 
```
function
```
function f( parameter){code block} // function definition
f(); //call a function
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

```
//check conditional 

function b(a) {
    if (a == 0) {
        println("a = ",a);
    } else {
        println("a = ", a);
    }
}

println("b(0) is ");
b(0);
println("b(10) is ");
b(10); 
println("roll" == "tide");
println("roll" == "roll");

//check conditional
```
```
sum = 0;

fp = open("sum.dat");

i = readInt(fp);
while(!(eof(fp)))
{
    sum = sum + i;
    i = readInt(fp);
}
println(sum);
close(fp);
```

```
//the problem can't work well, because of the issue about readin files
// but my language can handle the sum of array
a = array(9);
a[0] = 1; a[1] = 2; a[2] = 3;a[3] = 4;a[4] = 5; a[5]=6;a[6]= 7;a[7]= 8;a[8]= 9;
println("the array capacity is ", arraySize(a));
println(a);

i=0;
sum =0;
for(i =0; i< 9;i = i+1){
	sum = sum + a[i];
}
print("the sum is ");
println(sum);
```
