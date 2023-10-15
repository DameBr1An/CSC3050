OBJ = tester.o phase1.o phase2.o 

tester:$(OBJ)
	g++ tester.o phase1.o phase2.o -o tester
tester.o:tester.cpp
	g++ -c tester.cpp -o tester.o
phase1.o:phase1.cpp 
	g++ -c phase1.cpp -o phase1.o
phase2.o:phase2.cpp 
	g++ -c phase2.cpp -o phase2.o
.PHONY:clean

clean:
	rm *.o tester