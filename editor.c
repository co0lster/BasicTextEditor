/** includes **/
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
/** defines **/

#define CTRL_KEY(k) ((k) & 0x1f) // macro

/** data **/

struct termios orig_termios;

/** terminal **/

void kill (const char *s)
{
  perror(s);// custom message to be printed before the error message itself

  exit(1);
}

void disableRawMode ()
{
  if (tcsetattr(STDIN_FILENO,TCSAFLUSH, &orig_termios) == -1)
  kill("tcsetattr");
}

void enalbeRawMode ()
{
  if(tcgetattr(STDIN_FILENO, &orig_termios)== -1) kill("tcgetattr") ; // wczytuje atrybuty terminala
  atexit(disableRawMode); // after the program is closed, restore atrributes

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT |ICRNL|INPCK|ISTRIP|IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN); // ECHO wyswietla wszystkie znaki jakie wpisuje
  // c_flag --> local flags
  // ECHO is a bitflag
  // ISIG enable singals
  // ICANON Canonical input
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;
  // cc - control characters
  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)== -1) kill("tcgetattr") ;  // drugi argument kiedy wporwadzic zmiany czeka na output a odrzuca input

}

void editorReadKey()
{
  int nread;
  char c;
  while ((nread = read(STDIN_FILENO, &c, 1 )) != 1)
  {
    if (nread == -1 && errno != EAGAIN) kill("read");
  }
  return c;
}

/**input **/

void editorProcessKeypress()
{
  char c = editorReadKey()
}

/** init **/

int main()
{
  enalbeRawMode();

  while (1)
  {
    char c = '\0';
    if (read(STDIN_FILENO, &c, 1) == -1 && errno!= EAGAIN) kill("read");
    if (iscntrl(c)) // checks if character is a control character
    {
      printf("%d\r\n",c);
    }else
        {
            printf("%d ('%c')\r\n",c,c );
        }
        if (c == CTRL_KEY('q')) break;
  }

  return 0;

}
