#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include <time.h>

#define STRING_SIZE 50

WINDOW *create_window_box(int h, int w, int y, int x, char title[STRING_SIZE]);
char *update_time_string(char * string, int size);
char *update_countdown_string(char * string, int size, int time[]);
char *update_countup_string(char * string, int size, int time[]);
int print_ascii_number_singleton(WINDOW* window, char *string[], int x);
int wprint_ascii_number(WINDOW* window, char *string);

/* Cada número es un array de cadenas. Entonces NUMERO[i], i = 0, 1, 2
   va a imprimir al i-cadena dentro de NUMERO */

char* EMPTY[] = {
    "   ",
    "   ",
    "   "
};

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

char **SYMBOLS[] = {ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, COLON, EMPTY};

int main(int argc, char *argv[]){

  WINDOW *main_win;

  int startx, starty;
  int gap, width, height;
  int user_input;
  int time_up_slash_down[3];
  int c;

  char time_string[STRING_SIZE];
  char window_title[STRING_SIZE];

  bool is_help = false;

  extern char *optarg;

  enum {
    CLOCK_MODE,
    COUNTDOWN_MODE,
    COUNTUP_MODE } mode = CLOCK_MODE;

  initscr();
  cbreak();

  keypad(stdscr, TRUE);

  refresh();

  gap = 2;
  width = COLS - gap;
  height = LINES - gap;

  starty = gap/2;
  startx = gap/2;

  while ((c = getopt(argc, argv, ":cud:h")) != -1){
    switch (c){
    case 'c':

      mode = CLOCK_MODE;
      strcpy(window_title, "CLOCK");

      break;

    case 'u':

      mode = COUNTUP_MODE;

      time_up_slash_down[0] = 0;
      time_up_slash_down[1] = 0;
      time_up_slash_down[2] = 0;
      strcpy(window_title, "COUNTUP");

      break;

    case 'd':

      mode = COUNTDOWN_MODE;

      time_up_slash_down[0] = 0;
      time_up_slash_down[1] = atoi(optarg);
      time_up_slash_down[2] = 0;
      strcpy(window_title, "COUNTDOWN");

    break;

    case 'h':
      is_help = true;
      break;

  /* Let's default to clock mode */

    default:

      mode = CLOCK_MODE;

      strcpy(window_title, "CLOCK");

      break;

    }

  }

  main_win = create_window_box(height, width, starty, startx, window_title);

  /* Press q to quit */

    while(user_input != 'q' && !is_help){

      /* Here goes menu implementation */

      timeout(1000);

      switch(mode){

      case CLOCK_MODE:

    	update_time_string(time_string, STRING_SIZE);
    	wprint_ascii_number(main_win, time_string);
    	box(main_win, 0, 0);
    	wrefresh(main_win);

	    break;

      case COUNTDOWN_MODE:
    	update_countdown_string(time_string, STRING_SIZE, time_up_slash_down);
    	wprint_ascii_number(main_win, time_string);
    	box(main_win, 0, 0);
    	wrefresh(main_win);

    	break;

      case COUNTUP_MODE:

    	update_countup_string(time_string, STRING_SIZE, time_up_slash_down);
    	wprint_ascii_number(main_win, time_string);
    	box(main_win, 0, 0);
    	wrefresh(main_win);

    	break;
      }

      user_input = getch();
    }

  wrefresh(main_win);

  endwin();
  if (is_help)
      printf("Usage:cclock -c,-u,-d [num]\n");

  return 0;
}

WINDOW *create_window_box(int h, int w, int y, int x, char title[STRING_SIZE]){
  WINDOW *win_local;

  win_local = newwin(h, w, y, x);
  box(win_local, 0, 0);
  waddstr(win_local, title);
  wrefresh(win_local);

  return win_local;
}

char *update_countup_string(char * string, int size, int time[]){
  int i,j;
  char hour[13], min[13], sec[13];
  char *times[] = {hour, min, sec};

  // If second or minute > 59 increase the minute/hour by one
  for (i = 1; i < 3; i ++){
      if (time[i] > 59){
          time[i] = 0;
          time[i - 1]++;
      }
  }

  for (j = 0; j < 3; j++){
    if (time[j] < 10)
        sprintf(times[j], "%d%d", 0, time[j]);
    else
        sprintf(times[j], "%d", time[j]);
  }

   snprintf(string, size, "%s:%s:%s", hour, min, sec);

   time[2]++;

   return string;
}

char *update_countdown_string(char * string, int size, int time[]){
    int i;
    char min[13], sec[13];
    char *times[] = {min, sec};

    // Make the screen flicker when time's up
    if (time[2] == 0 && time[1] == 0){
        if (strcmp(string, "00:00") == 0)
            snprintf(string, size, "%s:%s", "  ", "  ");
        else
            snprintf(string, size, "%s:%s", "00", "00");

        return string;

    }

    // Decrease hour/minute when 60 seconds elapse
    if (time[2] <= 0 && time[1] != 0){
        time[2] = 59;
        time[1]--;
    }

    // Format string
    for(i = 0; i < 2; i++){
        if (time[i + 1] < 10)
            sprintf(times[i], "%d%d", 0, time[i+1]);
        else
            sprintf(times[i], "%d", time[i+1]);
    }

    snprintf(string, size, "%s:%s", min, sec);
    time[2]--;

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

int wprint_ascii_number(WINDOW* window, char *string) {
// Prints the numbers in string one by one according to SYMBOLS
  int j, k;
  int int_string;
  int string_size;

  string_size = strlen(string);
  k = COLS/2 - (2) * string_size;

  for(j = 0; j < string_size; j++){
      switch (string[j]) {
        case ':':
            int_string = 10;
            break;
        case ' ':
            int_string = 11;
            break;
        default:
            int_string = string[j] - '0';
            break;
      }
      // if (string[j] != ':'){
      //   int_string = string[j] - '0';
      // } else {
      //     int_string = 10;
      // }
      print_ascii_number_singleton(window, SYMBOLS[int_string], k);

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
