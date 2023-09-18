#!/bin/bash
# a simple script to run clang-format and cpplint
clang-format --style=GNU -i main.c
cpplint main.c

