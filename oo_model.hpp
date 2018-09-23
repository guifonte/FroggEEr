
#ifndef OO_MODEL_HPP
#define OO_MODEL_HPP

#include <thread>
class Player {
  private:
  float x;
  float y;

  public:
  Player(float x, float y);
  void update(float newX,float newY);
  float getX();
  float getY();
};

/*class Obstaculo {
  private:
  float x;
  float comprimento;

  public:
  Obstaculo(float x, float comprimento);
  void update(float x);
};

class Lane {
  private:
  float y;
  float velocidade;
  std::vector<Obstaculo*> *obstaculos;

  public:
  Lane(float y, int nivel);
};*/



/*class Corpo {
  private:
  float massa;
  float velocidade;
  float posicao;
  float posicao_central;
  float elasticidade;
  float amortecimento;
  float forca;

  public:
  Corpo(float massa, float velocidade, float posicao, float elasticidade, float amortecimento);
  void update(float nova_velocidade, float nova_posicao, float nova_forca);
  float get_massa();
  float get_velocidade();
  float get_posicao();
  float get_posicao_central();
  float get_elasticidade();
  float get_amortecimento();
  float get_forca();
};

class ListaDeCorpos {
 private:
    std::vector<Corpo*> *corpos;

  public:
    ListaDeCorpos();
    void hard_copy(ListaDeCorpos *ldc);
    void add_corpo(Corpo *c);
    std::vector<Corpo*> *get_corpos();
};*/

class Fisica {
  private:
    Lane *lane;

  public:
    Fisica(Lane *lane);
    //void add_lane(Corpo *c);
    //void choque(float forca);
    void update(float deltaT);
};

class Tela {
  private:
    //ListaDeCorpos *lista, *lista_anterior;
    Player *playerAtual, *playerAnterior;
    Lane *lane;
    int maxI, maxJ;
    float maxX, maxY;

  public:
    //Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY);
    Tela(Player *player, Lane *lane, int maxI, int maxJ, float maxX, float maxY);
    ~Tela();
    void stop();
    void init();
    void update();
};

void threadfun (char *keybuffer, int *control);

class Teclado {
  private:
    char ultima_captura;
    int rodando;

    std::thread kb_thread;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
};

#endif
