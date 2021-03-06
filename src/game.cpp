#include "../include/game.h"

Game::Game(Drawer* drawer){
  player = "Player01";
  
  field = new Field(this, drawer);
  nextTetr = NULL;
  activeTetr = NULL;
  this->drawer = drawer;
}

void Game::init() {
  //Starts the score
  score = 0;
  playing = true;
  paused = false;
  //draws the start screen;
  drawer->init(player, field);

  //initialize the tetrominoes
  nextTetr = new Tetromino(field, this, drawer);
  createNextTetr();

  controller = thread(&Game::listenKeys, this);
}

void Game::update() {
  if(!paused) {
    activeTetr->update();
  }
}

bool Game::isPlaying(){
  return playing;
}

void Game::increaseScore(int value) {
  score += value;
  drawer->updateScore(score);
}

void Game::gameOver(){
  playing = false;
  drawer->showGameOver();
  controller.join();
}

void Game::createNextTetr() {
  activeTetr = nextTetr;
  nextTetr = new Tetromino(field, this, drawer);
  drawer->updateNext(nextTetr);
  activeTetr->init();
}

void Game::listenKeys() {
  char key;
  while(playing) {
    key = getchar();
    switch(key) {
      case 'a':
        activeTetr->moveLeft();
        break;
      case 'd':
        activeTetr->moveRight();
        break;
      case 'w':
        activeTetr->rotate();
        break;
      case 's':
        activeTetr->speedUp();
        break;
      case 'p':
        if(paused) {
          paused = false;
          drawer->updateField(field);
          activeTetr->resume();
        } else {
          paused = true;
          drawer->showPause();
          activeTetr->pause();
        }
        break;
      default:
        break;
    }
  }
}

Game::~Game() {
  if (nextTetr != NULL) {
    delete nextTetr;
    delete activeTetr;
    delete field;
    delete drawer;
  }
}