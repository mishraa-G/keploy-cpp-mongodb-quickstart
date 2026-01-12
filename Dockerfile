# -------- Build stage --------
FROM ubuntu:22.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    git \
    wget \
    pkg-config \
    libssl-dev \
    libsasl2-dev \
    libmongoc-dev \
    libbson-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

# -------- Runtime stage --------
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    libmongoc-1.0-0 \
    libbson-1.0-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY --from=builder /app/build/app .

EXPOSE 8080

CMD ["./app"]
