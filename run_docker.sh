#!/bin/bash

docker build -t fuse . && docker run -it --rm --privileged fuse sh
