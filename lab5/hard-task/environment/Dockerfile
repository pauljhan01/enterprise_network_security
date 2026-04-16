FROM ubuntu:24.04

# Installing build dependencies required because the C program relies on cmake and make
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY mimalloc-main /app/mimalloc-main
COPY inline_asm.h /app/inline_asm.h
COPY Makefile /app/Makefile
COPY nospec.h /app/nospec.h
COPY spectre.c /app/spectre.c
COPY spectre.h /app/spectre.h