FLAGS=-lportaudio -lncurses -lpthread -std=c++11

all: model

model: model_mainloop.cpp oo_model.cpp oo_model.hpp 01-playback.cpp 01-playback.hpp 
	g++ -omodel model_mainloop.cpp oo_model.cpp 01-playback.cpp $(FLAGS)
