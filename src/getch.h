#include "termios.h"
#include <stdio.h>
#include <bits/unistd.h>
#include <asm-generic/ioctls.h>
#include <sys/ioctl.h>

#define CMIN 1

#ifdef CTIME
#undef CTIME
#endif

#define CTIME 1

int getch(void)
{
    char ch;
    int error;
    static struct termios Otty, Ntty;

    fflush(stdout);
    tcgetattr(0, &Otty);
    Ntty = Otty;

    Ntty.c_iflag = 0; /* input mode */
    Ntty.c_oflag = 0; /* output mode */

/* Change this conditional to enable echo of input character */
#if 1
    Ntty.c_lflag = 0; /* line settings (no echo) */
#else
    Ntty.c_lflag = ECHO; /* line settings (echo) */
#endif
    Ntty.c_cc[VMIN] = CMIN;   /* minimum time to wait */
    Ntty.c_cc[VTIME] = CTIME; /* minimum characters to wait for */

#if 0
/*
* use this to flush the input buffer before blocking for new input
*/
#define FLAG TCSAFLUSH
#else
/*
 * use this to return a char from the current input buffer, or block if
 * no input is waiting.
 */
#define FLAG TCSANOW
#endif

    if (0 == (error = tcsetattr(0, FLAG, &Ntty)))
    {
        error = read(0, &ch, 1);            /* get char from stdin */
        error += tcsetattr(0, FLAG, &Otty); /* restore old settings */
    }

    return (error == 1 ? (int)ch : -1);
}

int kbhit(void)
{
    int cnt = 0;
    int error;
    static struct termios Otty, Ntty;

    tcgetattr(0, &Otty);
    Ntty = Otty;

    Ntty.c_iflag = 0;         /* input mode */
    Ntty.c_oflag = 0;         /* output mode */
    Ntty.c_lflag &= ~ICANON;  /* raw mode */
    Ntty.c_cc[VMIN] = CMIN;   /* minimum time to wait */
    Ntty.c_cc[VTIME] = CTIME; /* minimum characters to wait for */

    if (0 == (error = tcsetattr(0, TCSANOW, &Ntty)))
    {
        error += ioctl(0, FIONREAD, &cnt);
        error += tcsetattr(0, TCSANOW, &Otty);
    }

    return (error == 0 ? cnt : -1);
}