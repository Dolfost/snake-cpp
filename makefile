compiler=g++
name=snake.out

snake: 
	$(compiler) -g -Wall -Wextra -std=c++11 main.cpp include/core/init.cpp  include/core/log.cpp include/core/options.cpp include/game/game.cpp include/snake/draw.cpp include/snake/input.cpp include/snake/logic.cpp include/snake/setup.cpp -lncurses -o $(name)
	./$(name) --build-help

clean:
	rm $(name)
	



# g++ -g -Wall -Wextra -std=c++11 main.cpp -lncurses -o snake.out
# 
# ./snake.out --build-help
