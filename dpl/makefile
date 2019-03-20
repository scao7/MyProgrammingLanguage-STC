OBJS= lexeme.o lexer.o type.o parser.o environment.o eval.o mylang.o

CFLAGS=
export CFLAGS
mylang: $(OBJS)
	gcc -Wall -g $(OBJS)  -std=c99 -o dpl

mylang.o: mylang.c parser.c environment.c
	gcc -Wall -g $(CFLAGS) -std=c99 -c mylang.c

lexeme.o: lexeme.c type.c
	gcc -Wall -g $(CFLAGS) -std=c99 -c lexeme.c

lexer.o: lexer.c lexeme.c type.c parser.c
	gcc -Wall -g $(CFLAGS) -std=c99 -c lexer.c

type.o: type.c
	gcc -Wall -g $(CFLAGS) -std=c99 -c type.c

parser.o: parser.c 
	gcc -Wall -g $(CFLAGS) -std=c99 -c parser.c

environment.o: environment.c lexeme.c
	gcc -Wall -g $(CFLAGS) -std=c99 -c environment.c

eval.o: eval.c lexeme.c type.c environment.c
	gcc -Wall -g $(CFLAGS) -std=c99 -c eval.c

run: mylang
	run.sh

clean:
	rm -f ./*.o ./stc

prettyPrint1:
	echo Original file:
	cat arrays.stc
	echo Pretty Printed version of the original:
	dpl -pp arrays.stc > test1.pp.1
	cat test1.pp.1	
	echo Pretty Printed version of the pretty printed version:
	dpl -pp test1.pp.1 > test1.pp.2
	cat test1.pp.2
	diff -s -q test1.pp.1 test1.pp.2

prettyPrint2:
	echo Original file:
	cat recursion.stc
	echo Pretty Printed version of the original:
	dpl -pp recursion.stc > test1.pp.1
	cat test1.pp.1	
	echo Pretty Printed version of the pretty printed version:
	dpl -pp test1.pp.1 > test1.pp.2
	cat test1.pp.2
	diff -s -q test1.pp.1 test1.pp.2

prettyPrint3:
	echo Original file:
	cat problem.stc
	echo Pretty Printed version of the original:
	dpl -pp problem.stc > test1.pp.1
	cat test1.pp.1	
	echo Pretty Printed version of the pretty printed version:
	dpl -pp test1.pp.1 > test1.pp.2
	cat test1.pp.2
	diff -s -q test1.pp.1 test1.pp.2

error1:
	cat error1.stc

error1x:
	./dpl error1.stc

error2:
	cat error2.stc

error2x:
	./dpl error2.stc

error3:
	cat error3.stc
error3x:
	./dpl error3.stc
error4:
	cat error4.stc 
error4x:
	./dpl error4.stc

error5:
	cat error5.stc

error5x:
	./dpl error5.stc

arrays:
	cat arrays.stc

arraysx:
	./dpl arrays.stc

conditionals:
	cat conditionals.stc

conditionalsx:
	./dpl conditionals.stc

recursion:
	cat recursion.stc

recursionx:
	./dpl recursion.stc

iteration:
	cat iteration.stc

iterationx:
	./dpl iteration.stc

functions:
	cat functions.stc

functionsx:
	./dpl functions.stc

lambda:
	cat lambda.stc 

lambdax:
	./dpl lambda.stc

objects:
	cat objects.stc

objectsx:
	cat objects.stc

problem:
	cat problem.stc

problemx:
	./dpl problem.stc

