#!/bin/bash
app="phystech-life-simulator"
docker build -t ${app} .
docker run -i -t ${app}