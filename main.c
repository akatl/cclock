#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <time.h>

#define STRING_SIZE 50

WINDOW *create_window_box(int h, int w, int y, int x, char *title);
char *update_time_string(char * string, int size);
char *update_countdown_string(char * string, int size, int time[]);
char *update_countup_string(char * string, int size, int time[]);
int print_ascii_number_singleton(WINDOW* window, char *string[], int x);
int wprint_ascii_number(WINDOW* window, char *string, int size);

/* Cada número es un array de cadenas. Entonces NUMERO[i], i = 0, 1, 2  va a imprimir al i-cadena dentro de NUMERO */

char* COLON[] = {
  " . ",
  "   ",
  " . "
};

char* ZERO[] = {
  " _ ",
  "/ \\",
  "\\_/"
};

char* ONE[] = {
  " ",
  " /| ",
  "  | "
};

char* TWO[] = {
  "__ ",
  " _)",
  "/__"
};

char* THREE[] = {
  "__ ",
  " _)",
  "__)"
};
char* FOUR[] = {
  " ",
  "|_|",
  "  |"
};

char* FIVE[] = {
  " __",
  "|_ ",
  "__)"
};

char* SIX[] = {
  " __",
  "/_",
  "\\_)"
};

char* SEVEN[] = {
  "___",
  "  /",
  " / "
};

char* EIGHT[] = {
  " _ ",
  "(_)",
  "(_)"
};

char* NINE[] = {
  " _",
  "(_\\",
  " _/"
};

int main(int argc, char *argv[]){

  WINDOW *main_win;

  int startx, starty;
  int gap, width, height;
  int user_input;
  int time_up_slash_down[3];
  int c;

  char time_string[STRING_SIZE];

  extern char *optarg;

  enum { CLOCK_MODE, COUNTDOWN_MODE, COUNTUP_MODE } mode = CLOCK_MODE;

  initscr();
  cbreak();

  keypad(stdscr, TRUE);

  gap = 2;
  width = COLS - gap;
  height = LINES - gap;

  starty = gap/2;
  startx = gap/2;

  refresh();

  main_win = create_window_box(height, width, starty, startx, "Clock");

  while ((c = getopt(argc, argv, "cud:")) != -1){
    switch (c){
    case 'c':

      mode = CLOCK_MODE;

      break;

    case 'u':

      mode = COUNTUP_MODE;
      time_up_slash_down[0] = 0;
      time_up_slash_down[1] = 0;
      time_up_slash_down[2] = 0;
      break;

    case 'd':

      mode = COUNTDOWN_MODE;
      waddstr(main_win, optarg);

      time_up_slash_down[0] = 0;
      time_up_slash_down[1] = atoi(optarg);
      time_up_slash_down[2] = 0;

    break;

    default:

      mode = CLOCK_MODE;

      break;

    }

  }

  /* Let's default to clock mode */

    while(1){

      /* Here goes menu implementation */

      timeout(1000);

      switch(mode){

      case CLOCK_MODE:

	update_time_string(time_string, STRING_SIZE);
	wprint_ascii_number(main_win, time_string, 11);
	box(main_win, 0, 0);
	wrefresh(main_win);

	break;
      case COUNTDOWN_MODE:

	update_countdown_string(time_string, STRING_SIZE, time_up_slash_down);
	wprint_ascii_number(main_win, time_string, 11);
	box(main_win, 0, 0);
	wrefresh(main_win);

	time_up_slash_down[2]--;
	break;

      case COUNTUP_MODE:

	update_countup_string(time_string, STRING_SIZE, time_up_slash_down);
	wprint_ascii_number(main_win, time_string, 11);
	box(main_win, 0, 0);
	wrefresh(main_win);

	time_up_slash_down[2]++;
	break;
      }

      user_input = getch();
    }

  wrefresh(main_win);

  endwin();

  return 0;
}

WINDOW *create_window_box(int h, int w, int y, int x, char *title){
  WINDOW *win_local;

  win_local = newwin(h, w, y, x);
  box(win_local, 0, 0);
  waddstr(win_local, title);
  wrefresh(win_local);

  return win_local;
}

char *update_countup_string(char * string, int size, int time[]){

  char hour[13], min[13], sec[13];

  if (time[2] >= 60){ 		/* if sec >= 60 incr. min by one */
    time[2] = 0;
    time[1]++;
  }

  if (time[1] >= 60){ 		/* if min >= 60 incr. hour by one */
    time[1] = 0;
    time[0]++;
  }

  if (time[2] < 10)
    sprintf(sec, "%d%d", 0, time[2]);
  else
    sprintf(sec, "%d", time[2]);

  if (time[1] < 10)
    sprintf(min, "%d%d", 0, time[1]);
  else
    sprintf(min, "%d", time[1]);

  if (time[0] < 10)
    sprintf(hour, "%d%d", 0, time[0]);
  else
    sprintf(hour, "%d", time[0]);

  snprintf(string, size, "%s:%s:%s", hour, min, sec);

   return string;
}

char *update_countdown_string(char * string, int size, int time[]){

  char min[13], sec[13];

  if (time[2] <= 0){ 		/* if sec <= 60 decr. min by one */
    time[2] = 59;
    time[1]--;
  }

  if (time[2] < 10)
    sprintf(sec, "%d%d", 0, time[2]);
  else
    sprintf(sec, "%d", time[2]);

  if (time[1] < 10)
    sprintf(min, "%d%d", 0, time[1]);
  else
    sprintf(min, "%d", time[1]);

  snprintf(string, size, "%s:%s", min, sec);

   return string;
}

char *update_time_string(char * string, int size){

  time_t local_t = time(NULL);
  struct tm *local_clock = localtime(&local_t);

  char hour[13], min[13], sec[13];

  if (local_clock->tm_hour < 10)
    sprintf(hour, "%d%d", 0, local_clock->tm_hour);
  else
    sprintf(hour, "%d", local_clock->tm_hour);

  if (local_clock->tm_min < 10)
    sprintf(min, "%d%d", 0, local_clock->tm_min);
  else
    sprintf(min, "%d", local_clock->tm_min);

  if (local_clock->tm_sec < 10)
    sprintf(sec, "%d%d", 0, local_clock->tm_sec);
  else
    sprintf(sec, "%d", local_clock->tm_sec);

  snprintf(string, size, "%s:%s:%s", hour, min, sec);

   return string;
}

int wprint_ascii_number(WINDOW* window, char *string, int size) {

  int j, k;

  k = COLS/2 - (2) * strlen(string);

  for(j = 0; j < size; j++){
      switch(string[j]){
      case '0':
	print_ascii_number_singleton(window, ZERO, k);
	break;
      case '1':
	print_ascii_number_singleton(window, ONE, k);
	break;
      case '2':
	print_ascii_number_singleton(window, TWO, k);
	break;
      case '3':
	print_ascii_number_singleton(window, THREE, k);
	break;
      case '4':
	print_ascii_number_singleton(window, FOUR, k);
	break;
      case '5':
	print_ascii_number_singleton(window, FIVE, k);
	break;
      case '6':
	print_ascii_number_singleton(window, SIX, k);
	break;
      case '7':
	print_ascii_number_singleton(window, SEVEN, k);
	break;
      case '8':
	print_ascii_number_singleton(window, EIGHT, k);
	break;
      case '9':
	print_ascii_number_singleton(window, NINE, k);
	break;
      case ':':
	print_ascii_number_singleton(window, COLON, k);
      }
      k += 4;
  }

  return 0;
}

int print_ascii_number_singleton(WINDOW* window, char *string[], int x){
  /* int x actúa como la columna donde se empieza a imprimir *string */
  int i;

  /* Aqui recorremos cada i-cadena dentro de *string y la imprimimos de una. */
  /* Ej para *UNO el ciclo es como sigue:  */
  /* UNO[0] imprime "  __"  en la fila i = 0 */
  /* UNO[1] imprime " /  \\" en la fila i = 1 */
  /* UNO[2] imprime " \\__/" en la fila i = 2 */

  for(i = 0; i < 3; i++){
    mvwprintw(window, (LINES / 2) - 3 + i, x,  "%s\n", string[i]);
  }

  return 0;
}

