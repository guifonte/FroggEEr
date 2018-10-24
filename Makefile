FLAGS=-lportaudio -lncurses -lpthread -std=c++11

all: server client

server: 
	g++ -o froggeer_server Server/src/froggeer_server.cpp Server/src/model/* Server/src/view/* Server/src/controller/* $(FLAGS)

client:
	g++ -o froggeer_client Client/src/froggeer_client.cpp Client/src/model/* Client/src/view/* Client/src/controller/* $(FLAGS)

runServer:server
	./frogger_server
	
runClient:client
	./frogger_client

