#!/bin/bash

python3 rebuild-maps.py
make -j$(nproc)