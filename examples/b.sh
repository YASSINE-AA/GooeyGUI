#!/bin/bash

cd .. && cd build && make && sudo make install && cd .. && cd examples && gcc plots.c -o plots -lGooeyGUI -I/usr/local/include/Gooey/ -lasan -fsanitize=address,undefined -lm && ./plots
