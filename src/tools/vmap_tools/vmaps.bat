@echo off
mkdir vmaps
vmap_extractor.exe
vmap_assembler Buildings vmaps
pause
