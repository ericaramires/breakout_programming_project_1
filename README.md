# Breakout Game - Programming I Project

## About the Project
This is an academic project developed for the Programming I course in Electronic Engineering in 2015. The game was originally implemented using the Allegro 4 library, but was later adapted to Allegro 5 only to ensure compatibility and functionality on modern systems. The presented code preserves the logic and structure of the original implementation developed during the academic period, demonstrating the application of fundamental programming concepts in C.

## Activity Objective
The activity proposed in the course consisted of developing a complete Breakout-style game, where the player should control a moving bar to bounce a ball and destroy blocks arranged on the screen, with the following specifications:

## 📋 Project Specifications
- **Main Mechanics**: Implement a game where the player moves a bar horizontally
- **Lives System**: The player has limited lives, losing a life each time the ball falls
- **Objective**: Destroy all blocks without losing all lives
- **Scoring System**: Calculate points based on destroyed blocks
- **Data Persistence**: Record and display the records of the best players

## 🎯 Implemented Features
- **Classic Gameplay**: Authentic Breakout experience with paddle, ball, and destructible blocks
- **Responsive Controls**: Fluid paddle movement using directional keys
- **Realistic Physics**: Ball movement with precise collision detection
- **Lives System**: Life loss mechanics when the ball falls
- **Dynamic Scoring**: Real-time point calculation as blocks are destroyed
- **Persistent Records**: Automatic recording and display of best results

## 🎮 Game Controls

### Main Menu
- **(I)** - Start Game
- **(R)** - View Records
- **(M)** - Instructions Manual
- **(S)** - Exit

### During the Game
- **Arrow Keys ← →** - Move paddle left/right
- **Spacebar** - Launch the ball
- **ESC** - Exit game

## 🛠️ Compilation and Execution

### Prerequisites
```bash
# For Linux/WSL systems
sudo apt update
sudo apt install liballegro5-dev -y
```

### Compilation
```bash
gcc -o breakout_allegro5 main_allegro5.c -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_ttf
```

### Execution
```bash
./breakout_allegro5
```

### For Windows with WSL
```bash
wsl gcc -o breakout_allegro5 main_allegro5.c -lallegro -lallegro_main -lallegro_primitives -lallegro_font -lallegro_ttf
wsl ./breakout_allegro5
```

## 📁 Project Structure
```
Erica/
├── main_allegro5.c     # Main game source code
├── breakout_allegro5   # Compiled executable
└── README.md          # This documentation
```

## 🔧 Technical Requirements
- **Compiler**: GCC or compatible
- **Graphics Library**: Allegro 5
- **Operating System**: Linux, WSL or Unix-like systems
- **Dependencies**: liballegro5-dev package

## 📊 Technical Features
- ✅ **Graphical Interface**: Implemented with Allegro 5 library
- ✅ **Collision Detection**: Precise algorithms for ball-paddle-blocks interaction
- ✅ **Scoring System**: Automatic calculation based on player performance
- ✅ **Data Persistence**: Automatic recording of records to file
- ✅ **Game Loop**: Adequate structure for real-time games
- ✅ **Event Management**: Efficient processing of user input

## 📚 Programming Concepts Demonstrated
This project exemplifies the practical application of fundamental programming concepts:
- **Data Structures**: Organization of game information (player, ball, blocks)
- **Algorithms**: Implementation of collision detection and game physics
- **Event-Driven Programming**: Processing of user input
- **File Manipulation**: Records persistence system
- **Graphics Programming**: Visual interface and real-time rendering
- **Control Structures**: Game loops and conditionals for gameplay logic

## 🎓 Academic Context
Developed as a practical assignment for the Programming I course, this project demonstrates:
- Ability to implement a complete and functional system
- Application of good programming practices
- Integration of multiple concepts in a cohesive project
- Development of interactive software with graphical interface

---
*Project developed as part of the Programming I curriculum in Electronic Engineering.* 