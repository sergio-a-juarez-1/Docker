![Turtle Crossing Gameplay](https://github.com/sergio-a-juarez-1/C-practice/blob/main/Images/turtle_crossing.gif?raw=true)

# 🐢 Native C Turtle Crossing Game (Containerized)

A lightweight, high-performance **arcade crossing game** written in pure C and fully containerized via Docker. This architecture decouples the workspace from host operating system dependencies by compiling and running the game inside a sandbox, while natively forwarding hardware acceleration, desktop display layers, and localized audio sockets back to your Linux desktop environment.

---

## 🚀 Engineering & Containerization Highlights

*   **Multi-Stage Build Pipeline:** Optimizes final production footprint by separating the compiler tools (`gcc` and development libraries) from the runtime layers. The application compiles in a heavy construction container, then moves only the finished executable into a lightweight runtime image.
*   **X11 Window Server Forwarding:** Connects your host desktop display server with the container sandbox via read-only UNIX socket mounting (`/tmp/.X11-unix`). This allows low-level `Xlib` window events to map seamlessly right to your active monitor window manager.
*   **PulseAudio Sound Socket Bridging:** Solves container sound isolation issues by bridging the container's standard ALSA calls right into your local user path configuration socket (`/run/user/1000/pulse/native`). This allows simultaneous background audio loops (`ffplay`) and retro crash effects (`aplay`) to mix perfectly over your host speakers.
*   **Persistent High Score Memory:** Features high score stability across app lifecycle runs. By mapping a single file volume binding (`-v`), the game updates `highscore.txt` directly to your physical hard drive in real-time, bypassing typical container data loss when closing the window.
*   **Secure Non-Root Privilege Model:** Implements isolation best practices by building a locked-down, unprivileged `developer` system user path inside the image structure. The engine drops root privileges before launching display contexts.

---

## 🎮 How To Play

1.  **Objective:** Guide your turtle from the bottom of the screen to the top green finish line while dodging oncoming traffic. Can you beat all **10 Levels** to achieve victory?
2.  **Controls:**
    *   `Arrow Keys` — Control the turtle's movement vectors.
    *   `R Key` — Instantly resets game parameters, flushes the canvas board, and restarts the audio loops after a collision or victory screen.
    *   `Escape Key` — Safely kills background audio processes, closes display contexts, and exits cleanly to the terminal window.
3.  **Difficulty & Progression:** Each time you successfully hit the green finish line, you level up. Traffic speed scales up natively, and the panel colorspace dynamically shifts.

---

## 🐳 Docker Deployment & Execution

To run this desktop game on any standard Linux environment, you do not need to install local graphical libraries. Ensure you have **Docker** installed and running on your host system.

### 1. Clone and Navigate to the Game Directory
Clone the repository into your preferred folder (e.g., `~/Downloads/`) and change directories into the specific containerized workspace:
```bash
git clone https://github.com/sergio-a-juarez-1/Docker.git
cd Docker/containerized_game/
```

### 2. Initialize the Scoreboard File   [OPTIONAL]
This step is not strictly necessary if you clone the repository exactly as instructed. The `highscore.txt` file is already tracked and included in this GitHub repo, so it will automatically land in your folder as a real file the second you run `git clone`. 

However, keeping this step as an optional reference is excellent defensive engineering practice. If a user deletes the file by accident, resets their codebase, or runs the launch command in a blank directory, it will trigger a folder-mounting daemon crash. If you ever need to manually regenerate it, run:
```bash
echo "1" > highscore.txt
```

<h3>3. Build the Image</h3>

Run the multi-stage build command inside the folder to compile the C source code and assemble the runtime layer:
```bash
docker build -t native-c-game .
```

### 4. Create a Permanent Execution Shortcut
Aliases disappear as soon as you close your terminal window. To make a shortcut stick around forever so you can run the game with a single command from **any folder path on your computer**, append the path to your shell profile configuration:

```bash
echo 'alias turtle="xhost +local:docker >/dev/null 2>&1; docker run -it --rm --env DISPLAY=\$DISPLAY --volume /tmp/.X11-unix:/tmp/.X11-unix:ro --device /dev/snd --env PULSE_SERVER=unix:/run/user/1000/pulse/native --volume /run/user/1000/pulse/native:/run/user/1000/pulse/native -v $(pwd)/highscore.txt:/app/highscore.txt native-c-game:latest; xhost -local:docker >/dev/null 2>&1"' >> ~/.bashrc
```

Apply the profile updates to your active shell interface:
```bash
source ~/.bashrc
```

You can now open any fresh terminal window from any folder location on your machine and simply play the game by executing:
```bash
turtle
```

---

## 🛠 Infrastructure Troubleshooting

### Error: `Are you trying to mount a directory onto a file (or vice-versa)?`
If you hit an OCI runtime mount error stating `not a directory` when typing `turtle`, it means the wrapper alias was executed before a physical text file existed on your computer at that location. Docker incorrectly generated an empty directory folder named `highscore.txt` instead.

**To completely wipe the broken environment and reset your scoreboard, run this cleanup sequence:**
```bash
# 1. Delete the accidental ghost directory folders created by the engine daemon
rm -rf highscore.txt

# 2. Re-initialize a physical, raw flat text integer file natively
echo "1" > highscore.txt

# 3. Relaunch the environment securely
turtle
```

---

## 📂 Project Architecture

The core architecture maps raw custom data structures cleanly into an hardware-accelerated OpenGL pipeline inside the sandbox container boundary limits:

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
*   **Rendering Loop:** Handled via tight `while(true)` poll check validation via `XPending()` to keep engine thread overhead minimal.
*   **Vector Alphanumeric Typography Engine:** Features a custom typography engine that mathematically draws characters out of raw matrix line arrays (`GL_LINES`), solving the issue of containerized font corruption or system asset mismatches entirely.
*   **Process Sandbox:** Background music threads are tracked via dynamic state tracking process hooks, allowing for a clean audio layout termination check upon close events.
