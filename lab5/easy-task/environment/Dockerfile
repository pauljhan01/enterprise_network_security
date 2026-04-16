FROM ubuntu:24.04

RUN apt-get update && apt-get install -y python3

WORKDIR /app
COPY setup.py .
RUN python3 setup.py
RUN rm setup.py

COPY test_outputs.py /app/test_outputs.py