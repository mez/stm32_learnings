# STM32 Learnings.

Slowly making my way through learning STM32 Microcontroller Programming. I am following 
[Learn STM32 Microcontroller Programming - Full Course for EE/CS Students and Beginners Version 2](https://www.youtube.com/watch?v=oD1qRFDBN74)

## Codebase Structure 
I didn't want to recreate a new codebase for every project. The main.c acts as a harness that hosts the different projects.
`Core/Inc/projects.h` has a flag called `ACTIVE_PROJECT` you can set to decide the project that is run at compile time.

## Board Spec Info

I am using the [NUCLEO-F446RE
](https://www.st.com/en/evaluation-tools/nucleo-f446re.html) below are the reference manual and schematics for your convenience!

* [Reference Manual](https://www.st.com/resource/en/reference_manual/rm0390-stm32f446xx-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)

* [Schematics](https://www.st.com/resource/en/schematic_pack/mb1136-default-c04_schematic.pdf)

## Notes
1. I recommend using the VS Code plugin to develop for the STM32. The CubeIDE is terrible and based on the eclipse IDE. What is this 2001?
2. Getting the env setup and going was a hassle, so once you get passed this stage you are a long way already!