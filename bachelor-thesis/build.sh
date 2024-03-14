#!/bin/bash

if [[ -z "$(docker images -qa thesis-builder:latest)" ]]; then
    docker build -t thesis-builder .
fi

docker run -ti --rm --name thesis-builder -v "$(pwd)":/work thesis-builder:latest bash entrypoint.sh
