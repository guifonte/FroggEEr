FLAGS=-lportaudio -lncurses -lpthread -std=c++11

all: model client

model: model_mainloop.cpp oo_model.cpp oo_model.hpp 01-playback.cpp 01-playback.hpp 
	g++ -o frogger model_mainloop.cpp oo_model.cpp 01-playback.cpp $(FLAGS)

client: client.cpp oo_model.cpp oo_model.hpp 
	g++ -o frogger_client client.cpp oo_model.cpp 01-playback.cpp $(FLAGS)
