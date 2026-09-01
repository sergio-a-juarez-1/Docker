# Jenkins Automation Masterclass & Docker Environments

Welcome to the official repository for the **Jenkins & Docker Infrastructure** training courses! This combined repository brings together production-grade container orchestration configurations and complete, multi-branch, parameter-driven automation pipelines. 

This layout maps exactly to your workspace directories, combining your architecture blueprints with the hands-on **Jenkins in Docker** and **Git Warp-9** curriculums.

---

## 📊 Combined Course Metrics
* **Total Curriculum Scope:** 16 Sections • 47 Lectures • 1h 55m Runtime (Git Warp-9) + Integrated Jenkins Labs
* **Core Core Competency:** Deploy self-healing, containerized CI/CD controllers, manage sidecar Docker-in-Docker (DinD) architectures, and engineer declarative, parameterized Jenkinsfiles with Groovy extensions.
* **Target Environment:** Docker Engine, Linux/macOS Shells, Jenkins LTS Core, and Git plumbing frameworks.

---

## 📂 Consolidated Repository Structure

```text
├── Jenkins_In_Docker/        # Root CI/CD system infrastructure & base architecture blueprints
│   ├── Dockerfile            # Custom Jenkins LTS layer with Docker-CE-CLI & bundled plugins
│   ├── README.md             # Local setup instructions for the Jenkins host controller
│   └── Jenkins-Course/       # Multi-stage validation labs and lecture pipeline scripts
│       ├── challenge2/       # Multi-stage software validation and test automation scripts
│       ├── challenge3/       # Advanced error containment and script pipeline executions
│       ├── func/             # Custom Groovy functions and reusable declarative pipeline logic
│       ├── variables/        # Environment string registers, credential helpers, and scope tracking
│       └── params/           # Core runtime configuration entrypoints
│           ├── boolean/      # Conditional execution blocks (e.g., conditional deployments)
│           ├── choice/       # Dynamic dropdown selections for target environments
│           ├── input/        # Intercept stages that pause pipelines for manual approval
│           ├── launch/       # Downstream instantiation scripts and trigger operations
│           └── remoteJob/    # Cross-server remote API pipeline invocation setups
└── Warp-9/                   # High-velocity advanced workflow sandboxes and repository content
```

---

## 🛠️ Infrastructure Getting Started

Follow these steps to establish the dedicated Docker bridge network, fire up the privileged sidecar Docker-in-Docker (DinD) container daemon, and launch your automated Jenkins controller.

### 1. Create a Docker Network
Isolate your continuous integration components on a dedicated bridge network:
```bash
docker network create jenkins
```

### 2. Run the Docker-in-Docker (DinD) Container
To allow your Jenkins pipelines to spin up containerized workloads, execute a privileged sidecar Docker daemon instance:
```bash
docker run --name jenkins-docker --rm --detach \
  --privileged --network jenkins --network-alias docker \
  --env DOCKER_TLS_CERTDIR=/certs \
  --volume jenkins-docker-certs:/certs/client \
  --volume jenkins-data:/var/jenkins_home \
  --publish 2376:2376 \
  docker:dind --storage-driver overlay2
```

### 3. Build and Run the Parameterized Jenkins Controller
Navigate to your infrastructure workspace, build the custom image embedded with `docker-ce-cli`, `blueocean`, and `docker-workflow`, and launch it:
```bash
# Move to workspace and build image
cd Jenkins_In_Docker
docker build -t myjenkins-blueocean:2.568.2-1 .

# Launch the orchestrator container
docker run --name jenkins-blueocean --rm --detach \
  --network jenkins --env DOCKER_HOST=tcp://docker:2376 \
  --env DOCKER_CERT_PATH=/certs/client --env DOCKER_TLS_VERIFY=1 \
  --publish 8080:8080 --publish 50000:50000 \
  --volume jenkins-data:/var/jenkins_home \
  --volume jenkins-docker-certs:/certs/client:ro \
  myjenkins-blueocean:2.568.2-1
```

---

## 📘 Detailed Syllabus Breakdown

### 🐳 Infrastructure Architecture & Host Virtualization
* **Docker-in-Docker Plumbing:** Linking independent container environments together safely via network aliases (`--network-alias docker`) and mutually verified TLS certificate volumes.
* **Controller Customization:** Building highly tailored core dependencies using internal security certificate injections (`update-ca-certificates`) and bundling required toolsets right out of the box using the `jenkins-plugin-cli`.
* **Hardware Benchmarking:** Allocating system resource envelopes to handle simultaneous code checkouts and pipeline builds without exhausting host capacity:

| Metric | Minimum Requirements | Small Team Recommended |
| :--- | :--- | :--- |
| **RAM** | 256 MB | 4 GB+ |
| **Storage** | 1 GB (10 GB for containers) | 50 GB+ |

### 📜 Declarative Pipelines & Build Orchestration
* **Syntax Engineering:** Writing pipeline scripts built on strict lifecycle hooks: setting up execution `agents`, processing operational `stages`, handling repository clones, and implementing automated `post`/`clean up` stages.
* **Workspace Directives:** Isolating target build directories via the `dir` wrapper step to execute multi-tier configurations (such as building projects with explicit Maven layers) without overlapping environments.
* **Source Control Integration:** Tracking changes over time using Polling SCM strategies and creating zero-touch **MultiBranch Pipelines** that automatically discover and build newly pushed feature tracks.

### 🎛️ Runtime Parameters, Variables & Logical Security
* **Interactive Form Intercepts:** Hardening execution pipelines through conditional gates:
  * `boolean`: Dynamically toggling unit tests or production branch expansions.
  * `string` & `choice`: Ingesting custom target parameters or runtime environment flags via dropdown lists.
  * `input`: Halting active jobs mid-execution until an operator provides explicit manual validation.
* **Variable Scope and Groovy Extensions:** Expanding pipeline logic by managing explicit credential injections, using `if` conditional branches to handle error scenarios, and passing data variables across running sub-jobs.
* **Git Warp-9 Frameworks:** Integrating advanced workflow control steps into local and remote developer setups to ensure swift history rebasing and clean commit workflows.

---

## 🧼 System Housekeeping & Cleanup
When you need to tear down your local sandbox environment and clean up all allocated storage spaces, run the following cleanup script:
```bash
# 1. Stop running containers
docker stop jenkins-blueocean jenkins-docker

# 2. Purge persistent volumes to reclaim host space
docker volume rm jenkins-data jenkins-docker-certs
```
