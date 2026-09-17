![Turtle Crossing Gameplay](https://github.com/sergio-a-juarez-1/C-practice/blob/main/Images/turtle_crossing.gif?raw=true)

# 🐢 Native C Turtle Crossing Game (Containerized)

A lightweight, high-performance **arcade crossing game** written in pure C and fully containerized via Docker. This architecture decouples the workspace from host operating system dependencies by compiling and running the game inside a sandbox, while natively forwarding hardware acceleration, desktop display layers, and localized audio sockets back to your Linux desktop environment.

---

## 🚀 Engineering & Containerization Highlights

*   **Multi-Stage Build Pipeline:** Optimizes final production footprint by separating the compiler tools (`gcc` and development libraries) from the runtime layers. The application compiles in a heavy construction container, then moves only the finished executable into a lightweight runtime image.
*   **X11 & Xwayland Server Forwarding:** Connects your host desktop display server with the container sandbox via UNIX socket mounting (`/tmp/.X11-unix`). On modern security-isolated distributions, it bridges authorization layers to map low-level graphical window contexts right to your active monitor.
*   **Audio Socket Bridging (PulseAudio & Pure PipeWire):** Solves container sound isolation issues by dynamically anchoring the container's standard ALSA calls into your host system's audio user path. It maps native legacy PulseAudio paths as well as modern enterprise PipeWire user runtime abstractions.
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

To run this desktop game on your Linux environment, you do not need to install local graphical libraries. Ensure you have **Docker** installed and running on your host system.

### 1. Clone and Prepare the Game Directory
Clone the repository into your preferred folder and safely copy the workspace into your unprivileged user home directory:
```bash
git clone https://github.com/sergio-a-juarez-1/Docker.git
mkdir -p ~/containerized_game
cp -r Docker/containerized_game/* ~/containerized_game/
cd ~/containerized_game
```
### 2. Initialize the Scoreboard File (Optional)
The `highscore.txt` file is already tracked in this repo. However, if it is deleted or you are spinning this up in a blank workspace directory, manually re-initialize the file to prevent Docker's engine daemon from creating a ghost directory mount error:
```bash
echo "1" > ~/containerized_game/highscore.txt
```

### 3. Build the Image
Run the multi-stage build command inside your target folder to compile the C source code and assemble the runtime layer:
```bash
docker build -t native-c-game .
```

---

## 🐧 4. OS-Specific Shortcuts & Execution

Because modern distributions handle graphical authorization cookies and audio engines differently, select the setup commands that match your specific operating system architecture.

### Option A: Red Hat Enterprise Linux (RHEL 8 / 9 / 10)
Modern Red Hat isolates the `root` context from your graphical desktop. Furthermore, modern variants like **RHEL 10** completely drop legacy PulseAudio packages and shift exclusively to an absolute **PipeWire** ecosystem. 

1. **Grant Docker Permissions to your Local User:**
   Ensure your non-root local user account belongs to the docker engine group:
   ```bash
   sudo usermod -aG docker $USER
   newgrp docker
   ```
2. **Append the RHEL-Optimized Alias:**
   This configuration opens up host network namespaces to bypass missing `xhost` dependencies, maps dynamic Xwayland system authorization directories, and binds your user's parent `/run/user/1000` runtime directory to safely capture dynamic PipeWire sockets on RHEL 10 kernels:
   ```bash
   echo 'alias turtle="docker run -it --rm --net=host --ipc=host --env DISPLAY=\$DISPLAY --env XAUTHORITY=\$XAUTHORITY --volume \$XAUTHORITY:\$XAUTHORITY:ro --volume /tmp/.X11-unix:/tmp/.X11-unix:ro --device /dev/snd --env PULSE_SERVER=unix:/run/user/1000/pulse/native --env XDG_RUNTIME_DIR=/run/user/1000 --volume /run/user/1000:/run/user/1000:ro -v /home/r/containerized_game/highscore.txt:/app/highscore.txt native-c-game:latest"' >> ~/.bashrc
   source ~/.bashrc
   ```
   *(Note: Adjust `/home/r/...` in the absolute path above to reflect your machine's exact unprivileged username)*

### Option B: Ubuntu / Debian-Based Distributions
Standard Debian-based environments can utilize direct `xhost` local container client validation toggles over traditional X11 sockets.

1. **Append the Ubuntu-Optimized Alias:**
   ```bash
   echo 'alias turtle="xhost +local:docker >/dev/null 2>&1; docker run -it --rm --env DISPLAY=\$DISPLAY --volume /tmp/.X11-unix:/tmp/.X11-unix:ro --device /dev/snd --env PULSE_SERVER=unix:/run/user/1000/pulse/native --volume /run/user/1000/pulse/native:/run/user/1000/pulse/native -v \$(pwd)/highscore.txt:/app/highscore.txt native-c-game:latest; xhost -local:docker >/dev/null 2>&1"' >> ~/.bashrc
   source ~/.bashrc
   ```

### 5. Launch the Game
Once your respective alias is appended and sourced, simply open any terminal window from any location on your host system and execute:
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
rm -rf ~/containerized_game/highscore.txt

# 2. Re-initialize a physical, raw flat text integer file natively
echo "1" > ~/containerized_game/highscore.txt

# 3. Relaunch the environment securely
turtle
```

### Error: `permission denied while trying to connect to the docker API`
This occurs if your active shell terminal session hasn't refreshed its group context table since adding your user profile metadata to the engine group. Simply reload your shell groups manually without logging out by running:
```bash
newgrp docker
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
*   **Vector Alphanumeric Typography Engine:** Features a custom typography engine that mathematically draws characters out of raw matrix line arrays (`GL_LINES`), solving the issue of containerized font corruption or system asset mismatches entirely.
*   **Process Sandbox:** Background music threads are tracked via dynamic state tracking process hooks, allowing for a clean audio layout termination check upon close events.
