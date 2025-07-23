FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y libcli11-dev libcurl4-openssl-dev && \
    apt-get clean

COPY immich-cli /app/
COPY build/auth.json /app/

WORKDIR /app
ENTRYPOINT /app/immich-cli --auth-loc=/app/auth.json upload
