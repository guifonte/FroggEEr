#ifndef SERVER_HPP
#define SERVER_HPP
#define MAX_CONEXOES 2
#include <fcntl.h> /* Added for the nonblocking socket */

#include<string>
#include<cstring>

using namespace std;

class Server {
  public:
    Server();
    static void init(int *socket_fd, unsigned int port);
    static void accept_connections(int *socket_fd, int *connection_fd);
    static void run(int *socket_fd, char *key, int *connection_fd, int *info, std::string *player_info);
};

#endif