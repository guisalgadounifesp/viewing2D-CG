// Guilherme Salgado Alves
// RA: 140083

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 25
float x_view_min = -0.75, x_view_max = 0.75, y_view_min = -0.75,
      y_view_max = 0.75, position_x = 0, position_y = 0;
int final_click = 0, window_width = 600, window_height = 600;
int draw_polygon = 0, num_points = 0;

struct point {
  float x, y;
  struct point *next;
};

struct point *points_head = NULL;

void draw_outline() {
  glBegin(GL_LINE_LOOP);
  glVertex2f(x_view_min, y_view_max);
  glVertex2f(x_view_max, y_view_max);
  glVertex2f(x_view_max, y_view_min);
  glVertex2f(x_view_min, y_view_min);
  glEnd();
}

float intersection_x(float x, float y, float m, float value) {
  x = x + (value - y) / m;
  return x;
}

float intersection_y(float x, float y, float m, float value) {
  y = y + m * (value - x);
  return y;
}

void print_array(float **array, int size) {
  for (int i = 0; i < size; i++) {
    printf("x:%f y:%f\n", array[i][0], array[i][1]);
  }
}

void initialize() {
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  draw_outline();
}

int count_slots(float **arr) {
  for (int i = 0; i < MAX_POINTS; i++) {
    if (arr[i][0] == 0 && arr[i][1] == 0) {
      return i;
    }
  }
  return MAX_POINTS;
}

float **create_float_array(int rows, int cols) {
  float **array = (float **)malloc(rows * sizeof(float *));
  for (int i = 0; i < rows; i++) {
    array[i] = (float *)malloc(cols * sizeof(float));
    for (int j = 0; j < cols; j++) {
      array[i][j] = 0;
    }
  }
  return array;
}

float **clip_bottom(int size, float **arr) {
  float **new_arr = create_float_array(MAX_POINTS, 2);
  int pos = 0;
  for (int i = 0; i < size; i++) {
    float p1[2], p2[2];
    p1[0] = arr[i][0];
    p1[1] = arr[i][1];

    if (i == size - 1) {
      p2[0] = arr[0][0];
      p2[1] = arr[0][1];
    } else {
      p2[0] = arr[i + 1][0];
      p2[1] = arr[i + 1][1];
    }

    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float m = dy / dx;

    if (p1[1] > y_view_min && p2[1] > y_view_min) {
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[1] < y_view_min && p2[1] > y_view_min) {
      new_arr[pos][0] = intersection_x(p1[0], p1[1], m, y_view_min);
      new_arr[pos][1] = y_view_min;
      pos++;
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[1] > y_view_min && p2[1] < y_view_min) {
      new_arr[pos][0] = intersection_x(p2[0], p2[1], m, y_view_min);
      new_arr[pos][1] = y_view_min;
      pos++;
    }
  }
  return new_arr;
}

float **clip_top(int size, float **arr) {
  float **new_arr = create_float_array(MAX_POINTS, 2);
  int pos = 0;

  for (int i = 0; i < size; i++) {
    float p1[2], p2[2];
    p1[0] = arr[i][0];
    p1[1] = arr[i][1];
    if (i == size - 1) {
      p2[0] = arr[0][0];
      p2[1] = arr[0][1];
    } else {
      p2[0] = arr[i + 1][0];
      p2[1] = arr[i + 1][1];
    }
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float m = dy / dx;
    if (p1[1] < y_view_max && p2[1] < y_view_max) {
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[1] > y_view_max && p2[1] < y_view_max) {
      new_arr[pos][0] = intersection_x(p1[0], p1[1], m, y_view_max);
      new_arr[pos][1] = y_view_max;
      pos++;
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[1] < y_view_max && p2[1] > y_view_max) {
      new_arr[pos][0] = intersection_x(p2[0], p2[1], m, y_view_max);
      new_arr[pos][1] = y_view_max;
      pos++;
    }
  }
  return new_arr;
}

float **clip_right(int size, float **arr) {
  float **new_arr = create_float_array(MAX_POINTS, 2);
  int pos = 0;
  for (int i = 0; i < size; i++) {
    float p1[2], p2[2];
    p1[0] = arr[i][0];
    p1[1] = arr[i][1];
    if (i == size - 1) {
      p2[0] = arr[0][0];
      p2[1] = arr[0][1];
    } else {
      p2[0] = arr[i + 1][0];
      p2[1] = arr[i + 1][1];
    }
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float m = dy / dx;
    if (p1[0] < x_view_max && p2[0] < x_view_max) {
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[0] > x_view_max && p2[0] < x_view_max) {
      new_arr[pos][0] = x_view_max;
      new_arr[pos][1] = intersection_y(p1[0], p1[1], m, x_view_max);
      pos++;
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[0] < x_view_max && p2[0] > x_view_max) {
      new_arr[pos][0] = x_view_max;
      new_arr[pos][1] = intersection_y(p2[0], p2[1], m, x_view_max);
      pos++;
    }
  }
  return new_arr;
}

float **clip_left(int size, float **arr) {
  float **new_arr = create_float_array(MAX_POINTS, 2);
  int pos = 0;
  for (int i = 0; i < size; i++) {
    float p1[2], p2[2];
    p1[0] = arr[i][0];
    p1[1] = arr[i][1];
    if (i == size - 1) {
      p2[0] = arr[0][0];
      p2[1] = arr[0][1];
    } else {
      p2[0] = arr[i + 1][0];
      p2[1] = arr[i + 1][1];
    }
    float dx = p2[0] - p1[0];
    float dy = p2[1] - p1[1];
    float m = dy / dx;
    if (p1[0] > x_view_min && p2[0] > x_view_min) {
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[0] < x_view_min && p2[0] > x_view_min) {
      new_arr[pos][0] = x_view_min;
      new_arr[pos][1] = intersection_y(p1[0], p1[1], m, x_view_min);
      pos++;
      new_arr[pos][0] = p2[0];
      new_arr[pos][1] = p2[1];
      pos++;
    } else if (p1[0] > x_view_min && p2[0] < x_view_min) {
      new_arr[pos][0] = x_view_min;
      new_arr[pos][1] = intersection_y(p2[0], p2[1], m, x_view_min);
      pos++;
    }
  }
  return new_arr;
}

float **sutherland_hodgman(int size, float **arr) {
  int arr_size;
  float **new_arr;
  new_arr = clip_left(size, arr);
  arr_size = count_slots(new_arr);
  new_arr = clip_right(arr_size, new_arr);
  arr_size = count_slots(new_arr);
  new_arr = clip_bottom(arr_size, new_arr);
  arr_size = count_slots(new_arr);
  new_arr = clip_top(arr_size, new_arr);
  return new_arr;
}

void draw_polygon_func() {
  if (draw_polygon == 1) {
    int i = 0;
    float **points_arr = create_float_array(num_points, 2);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glColor3f(0.0f, 1.0f, 0.0f);
    draw_outline();
    glColor3f(1.0f, 0.0f, 0.0f);
    struct point *points_head_copy;
    points_head_copy = points_head;
    while (points_head_copy != NULL) {
      points_arr[i][0] = points_head_copy->x;
      points_arr[i][1] = points_head_copy->y;
      printf("%f %f\n", points_arr[i][0], points_arr[i][1]);
      points_head_copy = points_head_copy->next;
      i++;
    }
    float **arr = sutherland_hodgman(num_points, points_arr);
    glBegin(GL_POLYGON);
    for (i = 0; i < MAX_POINTS; i++) {
      if (arr[i][0] == 0 && arr[i][1] == 0)
        break;
      glVertex2f(arr[i][0], arr[i][1]);
    }
    glEnd();
  }
  glutSwapBuffers();
}

void mouse_callback(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      position_x = 2 * (x / (float)window_width) - 1;
      position_y = (2 * (y / (float)window_height) - 1) * (-1);
      struct point *coordinate = NULL;
      coordinate = malloc(sizeof(struct point));
      coordinate->x = position_x;
      coordinate->y = position_y;
      coordinate->next = NULL;
      if (points_head == NULL)
        points_head = coordinate;
      else {
        struct point *points_head_copy;
        points_head_copy = points_head;
        while (points_head_copy->next != NULL) {
          points_head_copy = points_head_copy->next;
        }
        points_head_copy->next = coordinate;
      }
      num_points++;
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN) {
      draw_polygon = 1;
      glutMouseFunc(NULL);
    }
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowPosition(50, 100);
  glutInitWindowSize(window_width, window_height);
  glutCreateWindow("Exercicio3");
  glutDisplayFunc(draw_polygon_func);
  glutMouseFunc(mouse_callback);
  initialize();
  glutMainLoop();
  return 0;
}
