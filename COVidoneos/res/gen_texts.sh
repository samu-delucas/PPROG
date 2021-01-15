#!/bin/sh

# Load config file
source ./game.config

for file in ./res/text_src/${language}/*.txt; do
    cp -f ${file} ./res/text/${file##*/}
done