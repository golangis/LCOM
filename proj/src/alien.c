#include "alien.h"

void (initAlien)(Alien* alien, int id, int x_min, int y_min) {
  alien->id = id;
  alien->x_min = x_min;
  alien->y_min = y_min;
  alien->x_max = x_min + 10;
  alien->y_max = y_min + 10;
  alien->isAlive = true;
}

AlienGroup* (initAliens)(){
  Alien* array = (Alien*) malloc (sizeof(Alien) * 30);
  AlienGroup* group = (AlienGroup*) malloc (sizeof(AlienGroup));
  group->size = 30;

  //for (int i = 0; i < 30; i++) 

  group->first = array;

  return group;
}

void (moveAliens)(AlienGroup* group, enum direction dir, int speed) {
  switch (dir) {
    case right:
      for (int i = 0; i < group->size; i++) {
        group->first[i].x_min += speed;
        group->first[i].x_max += speed;
      }
      break;
    case left:
      for (int i = 0; i < group->size; i++) {
        group->first[i].x_min -= speed;
        group->first[i].x_max -= speed;
      }
      break;
    case down:
      for (int i = 0; i < group->size; i++) {
        group->first[i].y_min += speed;
        group->first[i].y_max += speed;
      }
      break;
    default: break;
  }
}

void (drawAliens)(AlienGroup* group) {
  for (int i = 0; i < group->size; i++) drawAlien(&(group->first[i]));
}

void (dieAlien)(AlienGroup* group, Alien* alien){
  Alien* a;
  int i = 0;
  for (i = 0; i < group->size; i++) {
    if (group->first[i].id == alien->id) a = &(group->first[i]);
  }
  for (int x = i; x < group->size-1; x++) {
    group->first[x] = group->first[x+1];
  }
  free(a);
  group->size--;
}

void (drawAlien)(Alien* alien) {
  video_draw_rectangle(alien->x_min, alien->y_min, 10, 10, 0xffffff);
}
