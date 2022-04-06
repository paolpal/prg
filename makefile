# make rule primaria con dummy target ‘all’--> non crea alcun file all ma fa un complete build
# che dipende dai target client e server scritti sotto
all: client server
# make rule per il client
client: client.c client/*.h client/implementation/*.c
	gcc -Wall client/implementation/*.c client.c  -o dev
# make rule per il server
# protocols.c networking.c access.c
server: server.c server/implementation/*.c server/*.h
	gcc -Wall -o serv server.c server/implementation/*.c
# pulizia dei file della compilazione (eseguito con ‘make clean’ da terminale)
clean:
	rm *o dev serv
