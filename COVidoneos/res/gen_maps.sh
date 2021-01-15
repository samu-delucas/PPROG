#!/bin/sh

##########################################
## Script to generate bitmap images for ##
## the game maps from image files       ##
## located on /res/maps_src/*           ##
##########################################
## IMPORTANT: This scripts requires     ##
## chafa to be installed in order to    ##
## work properly.                       ##
##########################################

# Load config file
source ./game.config

for file in ./res/maps_src/*; do
    filename=$(basename ${file})
    chafa -s ${game_width} ${file} > ./res/maps/${filename%.*}.txt
done