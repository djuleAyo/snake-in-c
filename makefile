snake: snake.c
	gcc -Wall -Wextra $^ -o $@ -lGL -lGLU -lglut
