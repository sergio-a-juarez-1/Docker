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

### 1. Build the Image
Run the multi-stage build command inside your project root folder to compile the source code and assemble the clean game environment:
```bash
docker build -t native-c-game .
```

### 2. Run the Game (With Silent Terminal Graphics & Sound)
Execute this command sequence to temporarily authorize connection endpoints, link system resources, mount your score persistence file, and launch the application cleanly:
```bash
# Authorize local container connections to your screen monitor quietly
xhost +local:docker >/dev/null 2>&1

# Launch the game container with graphic, hardware audio, and storage maps
docker run -it --rm \
  --env DISPLAY=$DISPLAY \
  --volume /tmp/.X11-unix:/tmp/.X11-unix:ro \
  --device /dev/snd \
  --env PULSE_SERVER=unix:/run/user/1000/pulse/native \
  --volume /run/user/1000/pulse/native:/run/user/1000/pulse/native \
  -v $(pwd)/highscore.txt:/app/highscore.txt \
  native-c-game:latest

# Remove local container access permissions upon exit safely
xhost -local:docker >/dev/null 2>&1
```
*(Note: If your local Linux user ID is something other than the default `1000`, replace the two `1000` integers in the path links above with your custom UID, which can be verified by running `id -u` in your terminal).*

### 💡 Pro-Tip: Fast Alias Execution
To play the game without copying the long command each time, add this short-key script alias directly into your local terminal prompt session:
```bash
alias frogger="xhost +local:docker >/dev/null 2>&1; docker run -it --rm --env DISPLAY=\$DISPLAY --volume /tmp/.X11-unix:/tmp/.X11-unix:ro --device /dev/snd --env PULSE_SERVER=unix:/run/user/1000/pulse/native --volume /run/user/1000/pulse/native:/run/user/1000/pulse/native -v \$(pwd)/highscore.txt:/app/highscore.txt native-c-game:latest; xhost -local:docker >/dev/null 2>&1"
```
Now you can launch the fully mixed arcade setup cleanly anytime simply by executing:
```bash
frogger
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
*   **Process Sandbox:** Background music threads are tracked via dynamic
