# Game-of-Balance-on-Nexys4DDR
Game of Balance is an accelerometer based maze navigation game, with added features of score and life, that is built on Nexys 4 DDR development board.

* Designed and Developed firmware for Microblaze Softprocessor with the following features:
  - State diagram model for game logic using QPNano
  - Dynamic maze generation using depth-first search
  - Ball collision with objects like walls, score points, and life point
  - Configuring and updating the LCD Display
* Added FPGA hardware modules to read MEMS 3-axis On-Board Accelerometer data and to interface LCD display and other push buttons with MicroBlaze soft processor.
* This project was done as a part of coursework - ECE253 Embedded System Design (Hardware Software Interface).
* Contributors:
  - Gokul Prasath Nallasami (SM Design in QP Nano, Design idea for Dynamic Maze Generation, Ball Collision Logic)
  - Barath Kumar Ramaswami (Implementation of Dynamic Maze Generation, LCD Update Logic)
