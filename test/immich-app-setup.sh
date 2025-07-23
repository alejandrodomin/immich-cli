#!/bin/bash

cd build

if [ ! -f docker-compose.yml ]; then
    wget -O docker-compose.yml https://github.com/immich-app/immich/releases/latest/download/docker-compose.yml
fi

if [ ! -f .env ]; then
    wget -O .env https://github.com/immich-app/immich/releases/latest/download/example.env
fi

sudo docker compose up -d
