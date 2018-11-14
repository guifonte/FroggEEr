#ifndef SERVER_HPP
#define SERVER_HPP
#define MAX_CONEXOES 1

class Server {
  public:
    Server();
    int init(unsigned int port);
    static void accept_connections(int socket_fd, int *connection_fd);
    static void run(int *socket_fd, char *key, int *connection_fd);
};

#endif