#!/bin/sh

SESSION="CFP"

SAMPLE="sample.cpp"
FILES=".clang-format $SAMPLE"
PREVIEW="clear; clang-format $SAMPLE | head -n64"

tmux new-session -d -s "$SESSION" "$EDITOR .clang-format"
tmux split-window -h "ls $FILES | entr sh -c '$PREVIEW'"
tmux select-pane -L
tmux attach-session -t "$SESSION"
