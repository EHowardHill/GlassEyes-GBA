#!/bin/bash

python3 generate-maps.py
make -j$(nproc)