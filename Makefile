lab1:	lab1.c
	gcc -o lab1 lab1.c
	./lab1

lab2:	lab2.c
	gcc -o lab2 lab2.c
	./lab2

lab3:	lab3.c
	gcc -o lab3 lab3.c
	./lab3

lab4:	lab4.c
	gcc -o lab4 lab4.c
	./lab4

lab5:	lab5.c
	gcc -o lab5 lab5.c
	./lab5

lab6:	lab6.c
	gcc -o lab6 lab6.c
	./lab6 todo_list.txt

lab7:	main.c list.c file.c
	gcc -o lab7 main.c list.c file.c
	./lab7

lab8:	main.c list.c file.c
	gcc -o lab8 main.c list.c file.c
	./lab8

lab9:	main.c file.c list.c
	gcc -o lab9 main.c list.c file.c -lpthread
	./lab9 todo_list.txt out.bin

lab10:	main.c file.c list.c undo.c
	gcc -o lab10 main.c list.c file.c undo.c -lpthread
	./lab10 todo_list.txt out.bin

# The clean command has been added by TA
clean:
	rm -f lab1 lab2 lab3 lab4 lab5 lab6 lab7 lab8 lab9 lab10