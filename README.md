# GOFISH

A program that you can play Go Fish against.

Its purpose was to help me learn how to program in C and gain experience managing and finishing a project. This project definitely got messier as it got bigger.

I wanted to avoid relying on AI tools and tried to build every line of code with the intent of maintaining a deep understanding. In some cases, I succeeded, but that definitely does not mean I think this codebase is particularly "good" or clean. I'd like to come back and refactor it.

## Issues

**As of 8/20/2026**

**Bot Logic:**
I feel that the bot logic is improper and needs work. It's definitely the part I thought about the least as I hurried to finish the project. There was a lot of temptation to just use AI without thinking it through.

**Naming Conventions:**
I'd like to go through and choose better naming conventions for my code. Right now, I don't like all the naming conventions I went with because I feel like they are inconsistent.

**Game Architecture:**
I think that the game should be organized as a big state machine rather than what it is now. I'm also not sure about the way I handled the event stream and event log. I'd like to look further into that.

**Unsafe Code:**
I have had this program crash on other machines. I need to go through and clean up potential issues and try to understand why they are bad practices.

## Project Size

This section is generated from the current project files. Run `make stats` after adding, removing, or changing source files to refresh it.

<!-- CODEBASE-STATS:START -->
Generated with `make stats`.

| Metric | Count |
| --- | ---: |
| C source/header files | 17 |
| C source/header lines | 1457 |
| Lines including makefile | 1513 |

| File | Lines |
| --- | ---: |
| `src/bot.c` | 255 |
| `src/bot.h` | 47 |
| `src/deck.c` | 161 |
| `src/deck.h` | 47 |
| `src/event_log.c` | 65 |
| `src/event_log.h` | 15 |
| `src/event_stream.c` | 80 |
| `src/event_stream.h` | 37 |
| `src/game.c` | 321 |
| `src/game.h` | 47 |
| `src/main.c` | 12 |
| `src/player.c` | 66 |
| `src/player.h` | 24 |
| `src/stack.c` | 37 |
| `src/stack.h` | 20 |
| `src/tui.c` | 204 |
| `src/tui.h` | 19 |
| `makefile` | 56 |
<!-- CODEBASE-STATS:END -->
