# --- Stage 1: Build Environment ---
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    libx11-dev \
    libgl1-mesa-dev \
    libglx-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /usr/src/app

COPY . .

RUN gcc -o turtle_crossing main.c -lX11 -lGL -lGLX

# --- Stage 2: Clean Runtime Environment ---
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libx11-6 \
    libgl1-mesa-glx \
    alsa-utils \
    ffmpeg \
    pulseaudio \
    libasound2-plugins \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /usr/src/app/turtle_crossing .
COPY highscore.txt .
COPY background.mp3 collision.wav next_level.wav .

RUN useradd -m developer && chown -R developer:developer /app

# Configure ALSA to pipe cleanly into PulseAudio inside the sandbox
RUN echo "pcm.!default {\n  type pulse\n}\nctl.!default {\n  type pulse\n}" > /etc/asound.conf

USER developer

CMD ["./turtle_crossing"]
