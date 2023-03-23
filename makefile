git322: assignment3_260929325.cpp linked_list.cpp
	g++ -c assignment3_260929325.cpp linked_list.cpp
	g++ -o git322 assignment3_260929325.o linked_list.o

test:
	g++ -g -o git322 assignment3_260929325.cpp

testll:
	g++ -g -o ll linked_list.cpp

debug:
	rm *.o
	g++ -g -o git322 assignment3_260929325.cpp linked_list.cpp

clean:
	rm git322; rm *.o; make git322