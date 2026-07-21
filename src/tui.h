#ifndef TUI_H
#define TUI_H
#include "game.h"

void tui_clearScreen();

void tui_newline(int howMuch);

void tui_startScreen();

void tui_winScreen(game_state *game);

void tui_displayTurn(game_state *game);

bool tui_askForCard(player *player, char playerInput[], size_t playerInputSize);

#endif