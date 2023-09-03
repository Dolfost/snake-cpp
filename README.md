# snake-cpp

Hello and welcome to the home of the ncurses snake game written in [neovim][nvim] by one enthusiast in his spare time!

### Game features
- [x] Speed
- [x] Performance
- [x] Beauty
- [x] Portability
- [x] Tries to be *POSIX-compliant*
- [x] "Window" system
- [x] Game itself
- [x] Scoreboard
- [x] Help window
- [x] Log windows
- [x] Terminal options

## Screenshots
![Pause](screenshots/gamepause.png "Pause") | ![Game over](screenshots/over.png "Game over") 
:---:|:---:
![Standby](screenshots/standby.png "Standby") | ![Scoreboard](screenshots/scoreboard.png "Scoreboard") 
![Help](screenshots/help.png "Help") | ![Log](screenshots/log.png "Log")

![Sidelog](screenshots/sidelog.png "Sidelog")


## How to install
### Installing dependencies
Game relies on the
 - [newcurses][ncurses] C library, so You have to install it on Your machine.
 - [make][make] program. Usually it is preinstalled.
 - [g++][compiler] compiler. 
##### Mac OS
If You use MacOS You can install [newcurses][ncurses] with [homebrew][brew] simply by
    
    $ brew install ncurses

##### Unix-like OS
Short answer: _It depends._  
For example, if You are using Ubuntu You can install [newcurses][ncurses] and [g++][compiler] by

    $ sudo apt-get install libncurses5-dev libncursesw5-dev clang

If You say _I use Arch btw_ often, You can try

    $ sudo pacman -S ncurses g++

If Your distro isn't mentioned here, try to use [google](https://www.google.com):

    how to install ncurses in *put Your distro name here*

    how to install g++ in *put Your distro name here*

### Getting the sources
Enter desired directory and run 

    $ git clone https://github.com/Dolfost/snake-cpp
    $ cd snake-cpp

It will create the directory with name `snake-cpp` and enter it. Then You have to compile the game.

### Compiling the game
To build the game You can:

    $ make snake

It will create the game executable `snake.out` and build the help pad for current terminal.

## Running the game
- Open the terminal window not smaller than 42 lines by 80 columns
- To start the game You can execute
    ```
    $ ./shanke.out
    ```
    in the git directory root. 

    If You are new to the game (probably You are), I recommend You to start game with the `--help` or `-h` option, so You can get used to it.
    ```
    $ ./snake.out --help
    ```
## Contributing
If You find any bugs or unexpected behaviors or just want to
contribute to the snake-cpp, You are welcome at the [issues tab][issue].



[nvim]: https://github.com/neovim/neovim "Go to nvim github page"
[ncurses]: https://en.wikipedia.org/wiki/Ncurses "Go to ncurses wikipedia page"
[brew]: https://brew.sh "Visit homebrew homepage"
[issue]: https://github.com/Dolfost/snake-cpp/issueso "Go to issues tab"
[make]: https://en.wikipedia.org/wiki/Make_(software) "Go to make wikipedia page"
[compiler]: https://en.wikipedia.org/wiki/GNU_Compiler_Collection "Go to GNU compiler collection wikipedia page"
