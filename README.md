# Jenkins in Docker

This repository contains documentation, configurations, and a custom Dockerfile to build and run **Jenkins** inside a Docker environment with full support for Docker-in-Docker (DinD) and pre-installed core plugins like Blue Ocean.

---

## Table of Contents

- [Features](#features)
- [Prerequisites](#prerequisites)
- [Hardware Recommendations](#hardware-recommendations)
- [Getting Started (macOS \& Linux)](#getting-started-macos--linux)
  - [1. Create a Docker Network](#1-create-a-docker-network)
  - [2. Run the Docker-in-Docker (DinD) Container](#2-run-the-docker-in-docker-dind-container)
  - [3. Build the Custom Jenkins Image](#3-build-the-custom-jenkins-image)
  - [4. Run the Jenkins Container](#4-run-the-jenkins-container)
- [Configuration and Customs](#configuration-and-customs)
  - [Custom CA Certificates](#custom-ca-certificates)
- [Post-Installation Setup](#post-installation-setup)
- [Troubleshooting](#troubleshooting)

---

## Features

- **Docker-in-Docker Support**: Allows Jenkins nodes to run Docker commands and spin up containers.
- **Pre-installed Plugins**: Comes bundled with `blueocean`, `docker-workflow`, and `json-path-api`.
- **Docker CLI Integrated**: The custom Jenkins image installs the official `docker-ce-cli`.

---

## Prerequisites

- **Docker Engine**: Installed and running.
- **Linux Users**: Ensure Docker is configured to run as a non-root user.

### Hardware Recommendations

| Metric | Minimum Requirements | Small Team Recommended |
| :--- | :--- | :--- |
| **RAM** | 256 MB | 4 GB+ |
| **Storage** | 1 GB (10 GB for containers) | 50 GB+ |

---

## Getting Started (macOS & Linux)

Follow these steps to establish the Docker network, sidecar DinD container, and Jenkins server:

### 1. Create a Docker Network
Open your terminal and isolate your components on a dedicated bridge network:
```bash
docker network create jenkins
```

### 2. Run the Docker-in-Docker (DinD) Container
To allow Jenkins to run Docker commands, spin up a secure, privileged Docker daemon container:
```bash
docker run --name jenkins-docker --rm --detach \
  --privileged --network jenkins --network-alias docker \
  --env DOCKER_TLS_CERTDIR=/certs \
  --volume jenkins-docker-certs:/certs/client \
  --volume jenkins-data:/var/jenkins_home \
  --publish 2376:2376 \
  docker:dind --storage-driver overlay2
```
**Key Flags Explained:**
- `--privileged`: Required for DinD to function properly.
- `--network-alias docker`: Exposes this container's daemon to the Jenkins container via the `docker` hostname.
- `--env DOCKER_TLS_CERTDIR=/certs`: Enforces secure TLS verification across containers.

### 3. Build the Custom Jenkins Image
Create a file named `Dockerfile` and copy the code below. This sets up the base LTS image, installs the Docker CLI, and adds plugins.

```dockerfile
FROM jenkins/jenkins:2.568.2-jdk21
USER root
RUN apt-get update && apt-get install -y lsb-release ca-certificates curl && \
    install -m 0755 -d /etc/apt/keyrings && \
    curl -fsSL https://docker.com -o /etc/apt/keyrings/docker.asc && \
    chmod a+r /etc/apt/keyrings/docker.asc && \
    echo "deb [arch=\$(dpkg --print-architecture) signed-by=/etc/apt/keyrings/docker.asc] \
    https://docker.com \((. /etc/os-release && echo \"\)VERSION_CODENAME\") stable" \
    | tee /etc/apt/sources.list.d/docker.list > /dev/null && \
    apt-get update && apt-get install -y docker-ce-cli && \
    apt-get clean && rm -rf /var/lib/apt/lists/*
USER jenkins
RUN jenkins-plugin-cli --plugins "blueocean docker-workflow json-path-api"
```

Build your customized image with a tag:
```bash
docker build -t myjenkins-blueocean:2.568.2-1 .
```

### 4. Run the Jenkins Container
Launch the built image. Connect it to your `jenkins` network and link the shared certificates and data volumes:
```bash
docker run --name jenkins-blueocean --rm --detach \
  --network jenkins --env DOCKER_HOST=tcp://docker:2376 \
  --env DOCKER_CERT_PATH=/certs/client --env DOCKER_TLS_VERIFY=1 \
  --publish 8080:8080 --publish 50000:50000 \
  --volume jenkins-data:/var/jenkins_home \
  --volume jenkins-docker-certs:/certs/client:ro \
  myjenkins-blueocean:2.568.2-1
```

---

## Configuration and Customs

### Custom CA Certificates
If your environment interacts with internal tools using self-signed certificates, utilize one of these methods to mount custom CA certificates into your environment:

#### Option 1: Docker Compose with Init Container (Recommended)
An initialization container can process and update your certificates into the shared Java Truststore volume before Jenkins boots up.

#### Option 2: Custom Docker Image
Alternatively, copy your root certificates directly into your base build layer and use traditional Debian commands (`update-ca-certificates`) to trust them across the image layer.

---

## Post-Installation Setup

1. **Access Jenkins**: Open your web browser and navigate to `http://localhost:8080`.
2. **Unlock Jenkins**: Retrieve the initial administrator password from the container's logs:
   ```bash
   docker logs jenkins-blueocean
   ```
3. **Setup Wizard**: Complete the guided setup to establish your initial admin user profile.

---

## Troubleshooting

- **Check Container Logs**: If your builds or instances crash, monitor live console outputs:
  ```bash
  docker logs -f jenkins-blueocean
  ```
- **Verify DinD TLS Mismatch**: Ensure your `DOCKER_HOST` environment value points precisely to the sidecar container `tcp://docker:2376` over the unified bridge network.
