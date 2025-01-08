#!/usr/bin/env sh 

if [ -z "$1" ]; then
	echo "No operation specified!"
	exit 1
fi

# installing homebrew 
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
eval "$(/opt/homebrew/bin/brew shellenv)"

# installing dependencies
brew install --formula cmake git ncurses

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
export REPO="$SCRIPT_DIR/.."

cmake -P "$SCRIPT_DIR/cmake/$1.cmake"
