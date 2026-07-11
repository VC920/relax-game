#ifndef GAME_H
#define GAME_H

void game_init();
void game_update(float dt);
void game_key_down(char key);
void game_key_up(char key);
void game_mouse_relative_pos(int x, int y);

#endif//GAME_H