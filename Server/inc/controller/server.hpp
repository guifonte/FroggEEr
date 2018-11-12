#ifndef SERVER_HPP
#define SERVER_HPP
#define MAX_CONEXOES 4

class Server {
  public:
    Server();
    int init(unsigned int port);
    static void run(char *key);
    static int connection_fd[MAX_CONEXOES];
    static int conexao_usada[MAX_CONEXOES];
};

#endif