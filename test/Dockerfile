FROM ubuntu:latest

RUN apt-get update && \
    apt-get install -y libcli11-dev libcurl4-openssl-dev && \
    apt-get install -y unzip && \
    apt-get clean

COPY immich-cli /app/
COPY build/auth.json /app/
COPY build/dog-breeds-image-dataset.zip /app/

RUN unzip /app/dog-breeds-image-dataset.zip -d /app/dataset/

WORKDIR /app
ENTRYPOINT /app/immich-cli --auth-loc=/app/auth.json upload
