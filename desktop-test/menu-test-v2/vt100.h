// Source: https://stackoverflow.com/questions/41519752/scanf-equivalent-with-arrow-key-support-in-c

#ifndef _VT100_H_
#define _VT100_H_

#include <stdio.h>

/* ------------------------------------------------------------------------- *
  VT100.H   (c)1989-1999, John Forkosh Associates        VT-100 Graphics
 * ------------------------------------------------------------------------- */
#define bel         printf("\007")
#define esc         printf("\033")
#define csi         printf("\033[")
#define clrscreen   printf("\033[2J")
#define lscreen     printf("\033[?5h")
#define dscreen     printf("\033[?5l")
#define rev_vid     printf("\033[7m")
#define blink       printf("\033[5m")
#define under       printf("\033[4m")
#define bold        printf("\033[1m")
#define norm_vid    printf("\033[0m")
#define wide_vid    printf("\033#6")
#define high_vid2   printf("\033#4")
#define high_vid1   printf("\033#3")
#define graphic     printf("\033(0")
#define no_graph    printf("\033(B")
#define no_window   printf("\033[1;24r")
#define no_att      printf("\033[0;22;24;25;27m")
#define cup(row,col)    printf("\033[%d;%dH",(row),(col))
#define stbm(top,bot)   printf("\033[%d;%dr",(top),(bot))
#define ri          printf("\033M")
#define el          printf("\033[K")
#define ed0         printf("\033[0J")
#define bar         graphic;printf("x");no_graph
#define wipe(row,col)   cup(row,col);printf("\033[J")
#define clr         wipe(0,0);norm_vid
/* ------------------------------------------------------------------------- *
   String equivalents ... Note: _cup() and _stbm() only accept literal args.
 * ------------------------------------------------------------------------- */
#define _bel        "\007"
#define _esc        "\033"
#define _csi        "\033["
#define _lscreen    "\033[?5h"
#define _dscreen    "\033[?5l"
#define _clrscreen  "\033[2J"
#define _rev_vid    "\033[7m"
#define _blink      "\033[5m"
#define _under      "\033[4m"
#define _bold       "\033[1m"
#define _norm_vid   "\033[0m"
#define _wide_vid   "\033#6"
#define _high_vid2  "\033#4"
#define _high_vid1  "\033#3"
#define _graphic    "\033(0"
#define _no_graph   "\033(B"
#define _no_window  "\033[1;24r"
#define _no_att     "\033[0;22;24;25;27m"
#define _cup(row,col)   "\033[" #row ";" #col "\110"
#define _stbm(top,bot)  "\033[" #top ";" #bot "\162"
#define _ri         "\033M"
#define _el         "\033[K"
#define _ed0        "\033[0J"
#define _bar        "\033(0x\033(B"
#define _wipe_clr   "\033[0;0H\033[2J"
#define _clr        "\033[0;0H\033[2J\033(B\033[0m\033[1;24r"
/* ------------------------------------------------------------------------- */

#endif /*_VT100_H_*/