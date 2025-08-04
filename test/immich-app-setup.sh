#!/bin/bash

# step 0 - setup
cd build

# step 1 - immich docker setup
if [ ! -f docker-compose.yml ]; then
    wget -O docker-compose.yml https://github.com/immich-app/immich/releases/latest/download/docker-compose.yml
fi

if [ ! -f .env ]; then
    wget -O .env https://github.com/immich-app/immich/releases/latest/download/example.env
fi

docker compose up -d

# step 2 - testing data set setup
if [ ! -f dog-breeds-image-dataset.zip ]; then
    curl -L -o dog-breeds-image-dataset.zip \
        https://www.kaggle.com/api/v1/datasets/download/darshanthakare/dog-breeds-image-dataset
fi

if [ ! -f workoutexercises-images.zip ]; then
    curl -L -o workoutexercises-images.zip \
        https://www.kaggle.com/api/v1/datasets/download/hasyimabdillah/workoutexercises-images
fi
