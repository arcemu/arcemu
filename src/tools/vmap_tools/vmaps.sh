#!/bin/sh
mkdir -p vmaps
./vmap_extractor
./vmap_assembler Buildings vmaps

