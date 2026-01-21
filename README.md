# STM32 Learnings.

Slowly making my way through learning STM32 Microcontroller Programming. I am following 
[Learn STM32 Microcontroller Programming - Full Course for EE/CS Students and Beginners Version 2](https://www.youtube.com/watch?v=oD1qRFDBN74)

## Codebase Structure 
I didn't want to recreate a new codebase for every project. The main.c acts as a harness that hosts the different projects.
`projects.h` has a flag you can set to decide the project that is run at compile time.