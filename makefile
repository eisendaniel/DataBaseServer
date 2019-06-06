dbserver: dbserver.o
	g++ -o dbserver server.o movieTable.o

dbserver.o: server.cc server.hh movieTable.cc movieTable.hh
	g++ -c server.cc movieTable.cc

clean:
	rm -rf *.o dbserver