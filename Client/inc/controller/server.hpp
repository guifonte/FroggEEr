#ifndef SERVER_HPP
#define SERVER_HPP

class Server {
  public:
    Server();
    int init(unsigned int port);
    static void run(int *socket_fd, char *key, int *connection_fd);
};

#endif