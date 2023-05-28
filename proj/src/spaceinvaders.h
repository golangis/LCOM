#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

/**
 * @file spaceinvaders.h
 * @brief Game logic functions. Stores the game elements and assures the interaction between them.
 * 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "framework/video/video.h"
#include "framework/mouse/mouse.h"
#include "framework/rtc/rtc.h"

#include "alien.h"
#include "player.h"
#include "shield.h"
#include "shot.h"

#include "direction.h"

extern char* video_mem;
extern uint8_t* vg_buffer;
extern unsigned h_res;
extern unsigned v_res;
extern unsigned bytes_per_pixel;

Player* ship;
Shield* shield1;
Shield* shield2;
Shield* shield3;
AlienGroup* aliens;
Shot* shots;
int wave;

enum direction last_alien_mov;
unsigned int updates;

/**
 * @brief A struct to represent a score by its number of points and the date and time it was achieved.
 * 
 */
typedef struct {
  int points;
  char datetime[17];
} Score;

/**
 * @brief Initializes the game elements.
 * 
 */
void (init_game)();

Score* (loadScores)();
Score (buildScore)(int points, rtc_time* time);
bool (processScore)(Score score, Score* array);
/*
leitura sacada de https://www.geeksforgeeks.org/relational-database-from-csv-files-in-c/
*/
void (storeScores)(Score* array);
/*
escrita sacada de https://www.programiz.com/c-programming/c-file-input-output
*/
void (draw)();
void (update)(int* no_lives);
void (drawMainMenu)();
void (drawHighscores)(Score* scores);
void (drawGameOverMenu)();
void (reload_aliens)();

void (initScores)();

enum state {
  game,
  mainMenu,
  highscores,
  quit,
  gameOverMenu
};

#endif
