#include <vector>
#include <chrono>
#include <thread>
#include <iostream>

#include "oo_model.hpp"

#include <ncurses.h>
using namespace std::chrono;

Corpo::Corpo(float massa, float velocidade, float posicao, float elasticidade, float amortecimento) {
  this->massa = massa;
  this->velocidade = velocidade;
  this->posicao = posicao;
  this->posicao_central = posicao;
  this->elasticidade = elasticidade;
  this->amortecimento = amortecimento;
  this->forca = 0;
}

void Corpo::update(float nova_velocidade, float nova_posicao, float nova_forca) {
  this->velocidade = nova_velocidade;
  this->posicao = nova_posicao;
  this->forca = nova_forca;
}

float Corpo::get_massa() {
  return this->massa;
}

float Corpo::get_velocidade() {
  return this->velocidade;
}

float Corpo::get_posicao() {
  return this->posicao;
}

float Corpo::get_posicao_central() {
  return this->posicao_central;
}

float Corpo::get_elasticidade() {
  return this->elasticidade;
}

float Corpo::get_amortecimento() {
  return this->amortecimento;
}

float Corpo::get_forca() {
  return this->forca;
}

ListaDeCorpos::ListaDeCorpos() {
  this->corpos = new std::vector<Corpo *>(0);
}

void ListaDeCorpos::hard_copy(ListaDeCorpos *ldc) {
  std::vector<Corpo *> *corpos = ldc->get_corpos();

  for (int k=0; k<corpos->size(); k++) {
    Corpo *c = new Corpo( (*corpos)[k]->get_massa(),\
                          (*corpos)[k]->get_posicao(),\
                          (*corpos)[k]->get_velocidade(),\
                          (*corpos)[k]->get_elasticidade(),\
                          (*corpos)[k]->get_amortecimento()
                        );
    this->add_corpo(c);
  }
}


void ListaDeCorpos::add_corpo(Corpo *c) {
  (this->corpos)->push_back(c);
}

std::vector<Corpo*> *ListaDeCorpos::get_corpos() {
  return (this->corpos);
}

Fisica::Fisica(ListaDeCorpos *ldc) {
  this->lista = ldc;
}

void Fisica::update(float deltaT) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();

  for (int i = 0; i < (*c).size(); i++) {
    float forca_elastica = (-1) * (*c)[i]->get_elasticidade() * ((*c)[i]->get_posicao() - (*c)[i]->get_posicao_central());
    float forca_amortecedor = (-1) * (*c)[i]->get_amortecimento() * (*c)[i]->get_velocidade();
    float new_acel = ( forca_amortecedor + forca_elastica + (*c)[i]->get_forca() ) / (*c)[i]->get_massa();
    float new_vel = (*c)[i]->get_velocidade() + (float)deltaT * new_acel/1000;
    float new_pos = (*c)[i]->get_posicao() + (float)deltaT * new_vel/1000;
    (*c)[i]->update(new_vel, new_pos, 0);
  }
}

void Fisica::choque(float forca) {
  // Atualiza parametros dos corpos!
  std::vector<Corpo *> *c = this->lista->get_corpos();
  for (int i = 0; i < (*c).size(); i++) {
    float new_vel = (*c)[i]->get_velocidade();
    float new_pos = (*c)[i]->get_posicao();
    (*c)[i]->update(new_vel, new_pos, forca);
  }
}

Tela::Tela(ListaDeCorpos *ldc, int maxI, int maxJ, float maxX, float maxY) {
  this->lista = ldc;
  this->lista_anterior = new ListaDeCorpos();
  this->lista_anterior->hard_copy(this->lista);
  this->maxI = maxI;
  this->maxJ = maxJ;
  this->maxX = maxX;
  this->maxY = maxY;
}

void Tela::init() {
  initscr();			       /* Start curses mode 		*/
	raw();				         /* Line buffering disabled	*/
  curs_set(0);           /* Do not display cursor */
}

void Tela::update() {
  int i;

  int n_cols;
  int n_lines;

  getmaxyx(stdscr,n_lines,n_cols); /*size of the terminal*/

  std::vector<Corpo *> *corpos_old = this->lista_anterior->get_corpos();

  // Apaga corpos na tela
  for (int k=0; k<corpos_old->size(); k++)
  {
    i = (int) ((*corpos_old)[k]->get_posicao()) * \
        (this->maxI / this->maxX);

    if((i < n_lines) && (k < n_cols) && (i > 0)){ /*Check if inside the terminal window*/
      move(i, k);   /* Move cursor to position */
      echochar(' ');  /* Prints character, advances a position */
    }
  }

  // Desenha corpos na tela
  std::vector<Corpo *> *corpos = this->lista->get_corpos();

  for (int k=0; k<corpos->size(); k++)
  {
    i = (int) ((*corpos)[k]->get_posicao()) * \
        (this->maxI / this->maxX);

    if((i < n_lines) && (k < n_cols) && (i > 0)){ /*Check if inside the terminal window*/
      move(i, k);   /* Move cursor to position */
      echochar('*');  /* Prints character, advances a position */
    }


    // Atualiza corpos antigos
    (*corpos_old)[k]->update(  (*corpos)[k]->get_velocidade(),\
                               (*corpos)[k]->get_posicao(),\
                               (*corpos)[k]->get_forca());
  }

  // Atualiza tela
  refresh();
}

void Tela::stop() {
  endwin();
}

Tela::~Tela() {
  this->stop();;
}


/*
class Teclado {
  private:
    char ultima_captura;
    int rodando;

  public:
    Teclado();
    ~Teclado();
    void stop();
    void init();
    char getchar();
}

*/

void threadfun (char *keybuffer, int *control)
{
  char c;
  while ((*control) == 1) {
    c = getch();
    if (c!=ERR) (*keybuffer) = c;
    else (*keybuffer) = 0;
    std::this_thread::sleep_for (std::chrono::milliseconds(10));
  }
  return;
}

Teclado::Teclado() {
}

Teclado::~Teclado() {
}

void Teclado::init() {
  // Inicializa ncurses
  raw();				         /* Line buffering disabled	*/
	keypad(stdscr, TRUE);	 /* We get F1, F2 etc..		*/
	noecho();			         /* Don't echo() while we do getch */
  curs_set(0);           /* Do not display cursor */

  this->rodando = 1;
  std::thread newthread(threadfun, &(this->ultima_captura), &(this->rodando));
  (this->kb_thread).swap(newthread);
}

void Teclado::stop() {
  this->rodando = 0;
  (this->kb_thread).join();
}

char Teclado::getchar() {
  char c = this->ultima_captura;
  this->ultima_captura = 0;
  return c;
}
