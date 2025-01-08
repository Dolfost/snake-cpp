#!/usr/bin/env sh 

if [ -z "$1" ]; then
	echo "No operation specified!"
	exit 1
fi

aptdeps="cmake git"
case "$1" in
	'docs'|'pack')
		aptdeps="$aptdeps libncurses-dev"
		;;
esac

# installing dependencies
sudo apt install $aptdeps

SCRIPT_DIR="$(dirname "$(readlink -f "$0")")"
export REPO="$SCRIPT_DIR/.."

cmake -P "$SCRIPT_DIR/cmake/$1.cmake"
