#ifndef TUI_H
#define TUI_H
#include "game.h"

void tui_clearScreen();

void tui_newline(int howMuch);

void tui_startScreen();

void tui_winScreen(GameState *game);

void tui_displayTurn(GameState *game);

bool tui_askForCard(Player *player, char playerInput[], int playerInputSize);

#endif
