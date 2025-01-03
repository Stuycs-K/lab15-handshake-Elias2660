compile serv cli: forking_server.o advancing_client.o pipe_networking.o colors.h
	@gcc -o cli advancing_client.o pipe_networking.o
	@gcc -o serv forking_server.o pipe_networking.o

server: serv
	@./serv

client: cli
	@./cli

advancing_client.o: advancing_client.c pipe_networking.h colors.h
	@gcc -c advancing_client.c

forking_server.o: forking_server.c pipe_networking.h colors.h
	@gcc -c forking_server.c

pipe_networking.o: pipe_networking.c pipe_networking.h colors.h
	@gcc -c pipe_networking.c

clean:
	@rm -f *.o
	@rm -f *~
	@rm -f cli
	@rm -f *.fifo
	@rm -f serv
	@rm -f mario
