# Keploy C++ + MongoDB Quickstart

This repository demonstrates how to use Keploy with a C++ REST API backed by MongoDB, without adding any SDKs or doing manual instrumentation.

The example shows how Keploy can record and replay real HTTP traffic for a C++ application using Docker.

## What this example does

- Simple C++ REST API using cpp-httplib
- MongoDB used for persistent storage
- Endpoints:
  - GET /health
  - POST /items
  - GET /items
- Docker Compose setup for the application and database
- Keploy record and replay working end to end

## Tech Stack

- Language: C++ (C++17)
- HTTP Server: cpp-httplib
- Database: MongoDB
- Mongo Driver: Official MongoDB C Driver (libmongoc)
- Containerization: Docker and Docker Compose
- Testing: Keploy (record and replay)

## Project Structure

.
├── src/
│   └── main.cpp
├── Dockerfile
├── docker-compose.yml
├── keploy.yml
└── README.md

## Running the Application with Docker

Create Docker network (one time):

docker network create keploy-network || true

Build and start the application:

docker compose up --build

You should see:

Server running on port 8080

## Testing the API

Health check:

curl http://localhost:8080/health

Response:

{"status":"ok"}

Create an item:

curl -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"name":"item1"}'

Response:

{"message":"Item created"}

List items:

curl http://localhost:8080/items

Response (example):

[{"_id":{"$oid":"..."},"name":"item1"}]

## Using Keploy

Keploy record and replay require a Linux environment.  
This quickstart was tested using GitHub Codespaces.

Install Keploy:

curl --silent -O -L https://keploy.io/install.sh
source install.sh

Verify installation:

keploy --version

## Record Traffic

keploy record -c "docker compose up --build" \
  --container-name app \
  --buildDelay 30

In another terminal, generate traffic:

curl http://localhost:8080/health

curl -X POST http://localhost:8080/items \
  -H "Content-Type: application/json" \
  -d '{"name":"item1"}'

curl http://localhost:8080/items

Stop recording using Ctrl+C.

Keploy will generate test artifacts automatically.

## Replay Tests

keploy test -c "docker compose up" \
  --container-name app \
  --delay 10 \
  --buildDelay 30

Expected result:
- Recorded test cases are replayed
- All tests pass successfully

## Notes

- This quickstart focuses on simplicity and clarity
- MongoDB is used via the official C driver for easier setup
- No SDKs or application code changes are required for Keploy

## Feedback

If you have suggestions or improvements, feel free to open an issue or pull request.
