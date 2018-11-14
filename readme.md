Made by Cynthia Baran and Guilherme Fontebasso

This game only runs on Linux. To install it, you must install the required dependencies:
```
sudo apt-get install libasound-dev portaudio19-dev
sudo apt-get install libjsoncpp-dev
```

To build the game, run
```
git clone https://github.com/guifonte/FroggEEr.git
cd FroggEEr
make
```

To run the server, run
```
./froggeer_server
```

To run the client and play the game, run
```
./froggeer_client
```

To move your star, use the `wasd` keys.
To quit the game, press `q` and then `Ctrl+C`
