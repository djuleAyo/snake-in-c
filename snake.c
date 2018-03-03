#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#define GRID_DIM 50
#define DOT_SIZE 10

typedef enum {
  DOT_SNAKE,
  DOT_SNAKE_HEAD,
  DOT_FRUIT,
  DOT_MUSHROOM
} dot_type;

typedef enum {
  DIRECTION_UP,
  DIRECTION_DOWN,
  DIRECTION_LEFT,
  DIRECTION_RIGHT
} direction_enum;

//times 2 since we save 2 coords for every dot
int snake[GRID_DIM * GRID_DIM * 2];
int snakeLen = 1;
int headX = GRID_DIM / 2;
int headY = GRID_DIM / 2;
int frootX, frootY;

dot_type grid[GRID_DIM * GRID_DIM];

double pace = 50;

direction_enum direction = DIRECTION_RIGHT;

int snakeHas(int x, int y) {
  for (int i = 0; i < snakeLen; i++) {
    if( snake[i * 2] == x && snake[i * 2 + 1] == y)
      return 1;
  }
  return 0;   
}

void snakeMove() {
  int newX, newY;
  newX = headX;
  newY = headY;

  switch(direction) {
  case DIRECTION_UP:
    newY = (headY + 1 + GRID_DIM) % GRID_DIM;
    break;
  case DIRECTION_DOWN:
    newY = (headY - 1 + GRID_DIM) % GRID_DIM;
    break;
  case DIRECTION_LEFT:
    newX = (headX - 1 + GRID_DIM) % GRID_DIM;
    break;
  case DIRECTION_RIGHT:
    newX = (headX + 1 + GRID_DIM) % GRID_DIM;
    break;
  }

  if(snakeHas(newX, newY)) {
    printf("score: %d\n", snakeLen);
    snakeLen = 1;
    getFroot();
  } else if(frootX == newX && frootY == newY) {
    snakeLen++;
    getFroot();
    goto move;
  } else {
  move:
    headX = newX;
    headY = newY;

    int tmpX = snake[0];
    int tmpY = snake[1];
    int tmp;
    
    snake[0] = headX;
    snake[1] = headY;
    
    for (int i = 1; i < snakeLen; i++) {
      tmp = snake[i * 2];
      snake[i * 2] = tmpX;
      tmpX = tmp;

      tmp = snake[i * 2 + 1];
      snake[i * 2 + 1] = tmpY;
      tmpY = tmp;
    }

  }
}


void display() {
  glClear(GL_COLOR_BUFFER_BIT);

  //drawSnake
  glColor3f(1, 1, 1);
  glBegin(GL_POINTS);
  for(int i = 0; i < snakeLen; i++) {
    glVertex3f(1 / (float)GRID_DIM * 2 * snake[i * 2] - 1,
	       1 / (float)GRID_DIM * 2 * snake[i * 2 + 1] - 1 , 0);
  }
  glEnd();

  //draw froot
  glColor3f(1, 0, 0);
  glBegin(GL_POINTS);
  glVertex3f((1 / (float) GRID_DIM) * 2 * frootX - 1,
	     (1 / (float) GRID_DIM) * 2 * frootY - 1 ,
	     0);
  glEnd();

  
  glutSwapBuffers();
}

void tick(int id) {
  snakeMove();
  glutPostRedisplay();
  glutTimerFunc(pace, tick, 1);
}

void keyboard(unsigned char c, int x, int y) {
  switch(c) {
  case '.':
    if(direction == DIRECTION_DOWN) return;
    direction = DIRECTION_UP;
    break;

  case 'e':
    if(direction == DIRECTION_UP) return;
    direction = DIRECTION_DOWN;
    break;

  case 'o':
    if(direction == DIRECTION_RIGHT) return;
    direction = DIRECTION_LEFT;
    break;

  case 'u':
    if(direction == DIRECTION_LEFT) return;
    direction = DIRECTION_RIGHT;
    break;
  }
}

void getFroot() {
  frootX = (int) ((rand() / (float)RAND_MAX) * GRID_DIM);
  frootY = (int) ((rand() / (float)RAND_MAX) * GRID_DIM);
}

int main(int argc, char **argv) {

  srand(time(NULL));
  getFroot();
  
  snake[0] =  (int) (GRID_DIM / 2);
  snake[1] =  (int) (GRID_DIM / 2);
  
  glutInit(&argc, argv);

  glutInitWindowSize(GRID_DIM *DOT_SIZE, GRID_DIM * DOT_SIZE);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Snake");

  glutDisplayFunc(display);
  glutTimerFunc(pace, tick, 1);
  glutKeyboardFunc(keyboard);
  glutInitDisplayMode(GL_DOUBLE | GL_RGB);
  
  glClearColor(0, 0, 0, 1);
  glPointSize(DOT_SIZE);
  
  glutMainLoop();

}
