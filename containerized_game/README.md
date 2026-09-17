![Native C Turtle Crossing Gameplay](https://github.com/sergio-a-juarez-1/C-practice/blob/main/Images/turtle_crossing.gif?raw=true)

# 🐢 Native C Turtle Crossing Game

A lightweight, high-performance **arcade crossing game** written in pure C. This game replaces heavy framework dependencies by hooking directly into low-level Linux display systems and rendering graphics natively through hardware acceleration.

It is inspired by classic arcade crossing mechanics and acts as a robust, native C port of the popular Python "Turtle Crossing" clone.

---

## 🚀 Engineering Highlights

*   **Zero Third-Party Dependencies:** Built completely without external game engines or frameworks like Raylib, SDL2, or Allegro. 
*   **Native X11 Engine Layer:** Links directly into the native Linux X11 display server to process asynchronous hardware keyboard inputs.
*   **Hardware-Accelerated OpenGL:** Integrates raw `glX` context piping to perform fast bounding-box (AABB) collision evaluations and shape updates at a locked **60 FPS** frame rate.
*   **Fail-Safe Geometric Dashboard:** Solves the problem of missing or corrupted font file paths by utilizing vector progress blocks that render directly inside the active viewport matrix.
*   **Zero Memory Leaks:** Standardized structure routines ensure all graphics pipelines and window resources clear fully upon closing.

---

## 🎮 How To Play

1.  **Objective:** Guide your turtle from the bottom of the screen to the top green finish line while dodging oncoming highway traffic blocks.
2.  **Controls:**
    *   `Up / Down / Left / Right Arrow Keys` — Control the turtle's movement vectors.
    *   `Shift + R Key` — Instantly clears the canvas board, resets the difficulty, and starts over at Level 1 after a collision crash.
    *   `Escape Key` — Safely closes all display contexts and exits cleanly to the terminal.
3.  **Difficulty Scaling:** Each time you successfully hit the green finish line, you level up. Your progress is marked in the dashboard and traffic velocity variables scale up natively.

---

## 🛠️ Build and Local Deployment

To run this desktop game on any standard Ubuntu, Debian, or Linux Mint environment, ensure you have the core system graphic development dependencies installed.

### 1. Install System Requirements
```bash
sudo apt update
sudo apt install build-essential libgl1-mesa-dev libx11-dev
```

### 2. Compilation
Compile using standard hardware-accelerated link flags:
```bash
gcc game.c -o native_turtle -lGL -lX11
```

### 3. Run the Game
```bash
./native_turtle
```

---

## 📂 Project Architecture

The core architecture uses custom data vectors map layout constraints into an OpenGL pipeline:

```c
typedef struct {
    float x, y;
} Vector2;

typedef struct {
    Vector2 pos;
    float speed;
    unsigned long color;
} Car;
```
*   **Rendering Loop:** Handled on a `while(true)` poll check via `XPending()` to keep thread usage minimal.
*   **Collision Model:** Employs dual-axis AABB boundary overlap math to manage pixel-perfect frame hits.
