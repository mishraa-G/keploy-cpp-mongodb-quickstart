# Keploy C++ + MongoDB Quickstart

This repository demonstrates how to use **Keploy** with a **C++ REST API** backed by **MongoDB**, without adding any SDKs or doing manual instrumentation.

It shows how Keploy can **record and replay real HTTP traffic** for a C++ application using **Docker**.

---

## What this example does

* Simple **C++ REST API** using `cpp-httplib`
* **MongoDB** used for persistent storage
* REST endpoints:

  * `GET /health`
  * `POST /items`
  * `GET /items`
* **Docker Compose** setup for the application and database
* **Keploy record and replay** working end-to-end

---

## Tech Stack

* **Language:** C++ (C++17)
* **HTTP Server:** cpp-httplib
* **Database:** MongoDB
* **Mongo Driver:** Official MongoDB C Driver (`libmongoc`)
* **Containerization:** Docker & Docker Compose
* **Testing:** Keploy (record & replay)

---

## Project Structure

```
.
├── app/
│   └── src/
│       └── main.cpp
├── docker/
│   ├── Dockerfile
│   └── docker-compose.yml
├── keploy/
│   ├── keploy.yml
│   ├── test-set-0/
│   └── mocks/
├── README.md
└── .gitignore
```

---

## Local setup (without Docker)

This section explains how to build and run the C++ application locally without Docker.
**Note:** MongoDB must also be running locally.

---

## Prerequisites

### Ubuntu / Debian

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake pkg-config \
  libssl-dev libsasl2-dev libbson-dev libmongoc-dev mongodb
```

Start MongoDB:

```bash
sudo systemctl start mongodb
sudo systemctl enable mongodb
```

---

### macOS (Homebrew)

```bash
brew update
brew install cmake pkg-config openssl mongo-c-driver
brew tap mongodb/brew
brew install mongodb-community@6.0
brew services start mongodb-community@6.0
```

---

## Build and run the application

From the repository root:

```bash
mkdir -p build
cd build
cmake ..
make -j$(nproc || echo 2)
cd ..
./build/app
```

You should see:

```
Server running on port 8080
```

---

## Test the API locally

### Health check

```bash
curl http://localhost:8080/health
```

Expected output:

```json
{"status":"ok"}
```

---

### Create an item

```bash
curl -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"name":"item1"}'
```

Expected output:

```json
{"message":"Item created"}
```

---

### List items

```bash
curl http://localhost:8080/items
```

Expected output (example):

```json
[{"_id":{"$oid":"..."},"name":"item1"}]
```

---

## Running the application with Docker

### Create Docker network (one time)

```bash
docker network create keploy-network || true
```

---

### Build and start the application

```bash
docker compose up --build
```

You should see:

```
Server running on port 8080
```

---

## Testing the API (Docker)

You can run the same curl commands as above:

```bash
curl http://localhost:8080/health

curl -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"name":"item1"}'

curl http://localhost:8080/items
```

---

## Using Keploy

> **Note:** Keploy record and replay require a **Linux environment** because they rely on **eBPF**.
> This quickstart was tested using **GitHub Codespaces**.

---

## Install Keploy

```bash
curl --silent -O -L https://keploy.io/install.sh
source install.sh
```

Verify installation:

```bash
keploy --version
```

---

## Record traffic

Start the application with Keploy recording enabled:

```bash
keploy record -c "docker compose up --build" \
  --container-name app \
  --buildDelay 30
```

In another terminal, generate traffic:

```bash
curl http://localhost:8080/health

curl -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"name":"item1"}'

curl http://localhost:8080/items
```

Stop recording using **Ctrl + C**.
Keploy will automatically generate test artifacts inside the `keploy/` directory.

---

## Replay tests

```bash
keploy test -c "docker compose up" \
  --container-name app \
  --delay 10 \
  --buildDelay 30
```

Expected result:

* Recorded test cases are replayed
* All tests pass successfully

---

## Notes

* This quickstart focuses on **simplicity and clarity**
* MongoDB is used via the **official C driver** for easier setup
* **No SDKs or application code changes** are required for Keploy

---

## Feedback

If you have suggestions or improvements, feel free to open an issue or submit a pull request.
