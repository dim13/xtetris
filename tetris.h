/*
# GENERIC X-WINDOW-BASED TETRIS
#
#	tetris.h
#
###
#
#  Copyright (C) 1992 - 95              Q. Alex Zhao, azhao@cc.gatech.edu
#
#			All Rights Reserved
#
#  Permission to use, copy, modify, and distribute this software and
#  its documentation for any purpose and without fee is hereby granted,
#  provided that the above copyright notice appear in all copies and
#  that both that copyright notice and this permission notice appear in
#  supporting documentation, and that the name of the author not be
#  used in advertising or publicity pertaining to distribution of the
#  software without specific, written prior permission.
#
#  This program is distributed in the hope that it will be "playable",
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
*/

/*** Common headers ***/

#include	<stdio.h>
#include	<math.h>
#include	<pwd.h>
#include	<errno.h>

#include	<X11/Xlib.h>
#include	<X11/Xutil.h>
#include	<X11/Xresource.h>
#include	<X11/cursorfont.h>
#include	<X11/keysym.h>
#include	<X11/Xos.h>
#include	<X11/Xfuncs.h>
#include	<X11/Xfuncproto.h>

#ifndef	X_NOT_STDC_ENV
#include	<stdlib.h>
#else
char    *getenv(
#if	NeedFunctionPrototypes
    char *
#endif
);
#endif

#ifdef	X_NOT_POSIX
int      getuid(
#if	NeedFunctionPrototypes
    void
#endif
);
struct passwd *getpwuid(
#if	NeedFunctionPrototypes
    int
#endif
);
#endif

#ifdef	_AIX
#include	<sys/select.h>
#endif

/*** for lseek ***/

#ifndef	SEEK_SET
#define	SEEK_SET	L_SET
#endif

#ifndef	SEEK_CUR
#define	SEEK_CUR	L_INCR
#endif

#ifndef	SEEK_END
#define	SEEK_END	L_XTND
#endif

/*** random generator ***/

#if	defined(HAS_48)		/* Use lrand48() and srand48() */
#define	LRAND()		lrand48()
#define	SRAND(X)	srand48((long) (X))
long     lrand48(
#if	NeedFunctionPrototypes
    void
#endif
);

#else
#if	defined(HAS_RANDOM)	/* Use random() and srandom() */

#define	LRAND()		random()
#define	SRAND(X)	srandom((unsigned int) (X))
long     random(
#if	NeedFunctionPrototypes
    void
#endif
);

#else				/* Use rand() and srand() */

#define	LRAND()		((long) rand())
#define	SRAND(X)	srand(X)

#endif
#endif

/*** macros ***/

#define	ZLIM(X, Y)	(((int) X) < (Y) ? ((int) X) : (Y))

/*** constants ***/

#ifndef	SCOREFILE
#define	SCOREFILE	"/usr/games/lib/tetris.scores"
#endif

#define	BIGFONT		"12x24"
#define	TINYFONT	"6x12"
#define	BVOLUME		-90
#define NUM_FLASHES	16

#define BONUSIN     25000
#define BONUSOUT    1000
#define SAMPLERATE  5000000

#define	MILLION		1000000
#define	MAXSCORES	3
#define	SHOWSCORES	15
#define	NAMELEN		12
#define	FILENAMELEN	1024

#define	MSG_PAUSED	"PAUSED"
#define	MSG_END		"GAME OVER"
#define	MSG_TITLE	"TETRIS"
#define	MSG_AUTHOR	"by Q. Alex Zhao"

#define	NUM_LEVELS	19
#define	ROWS		20
#define	COLS		10

#define	BOXSIZE		30
#define	OFFSET		20
#define	THINGSIZE	4
#define	NUM_THINGS	7
#define	NUM_BTHINGS	12
#define NUM_BITMAPS	16

/*** types ***/

typedef enum {FALL, DROP, ROTATE, LEFT, RIGHT, REFLECT} move_t;

typedef struct {
    char            myname[NAMELEN], myhost[NAMELEN], mydate[27];
    char            score[10];
    char            level[4];
    char            rows[5];
}               score_t;
#define	SCORESIZE	sizeof(score_t)

typedef struct {
    int             map[THINGSIZE][THINGSIZE];
    int             xpos, ypos;
    int             size, px, py, cid, gx, gy;
}               thing_t;

/*** variables defined in "tetris.c" ***/

Display *display;
int      screen_num;
Visual  *visual;
Bool     useColor;
Bool     use3D;
Colormap colormap;
Window   mainWin, blockWin;
Cursor   theCursor;
XFontStruct *bigFont, *tinyFont;
unsigned long fg, bg;

XSizeHints sizehints, iconsizehints;
XWMHints wmhints;

char     myHome[FILENAMELEN], scorefile[FILENAMELEN];
int      startlevel, level, prefilled, score, rows, range, rotOffset;
Bool     showNext, beep, bonusMode;
score_t  myscore;

/*** variables defined in "utils.c" ***/

Atom     delw;

/*** variables defined in "playing.c" ***/

/*** functions ***/

unsigned long getColor(char []);
void     showScores(int);
void     inits(int, char *[]);
void     playing(void);
void     realTime(struct timeval *);
void     newThing(void);
Bool     evGotNewThing(Bool);
void     redrawAll(void);
void     drawTitle(void);
void     drawStatus(void);
void     drawField(void);
void     drawThing(void);
void     drawThingDiff(thing_t *);
void     drawNext(void);
void     gameOver(void);
void     banner(char []);
void     clearNext(void);
void     putBox(void);
void     tryMove(move_t);
Bool     atBottom(void);
Bool     overlapping(void);
int      checkLines(void);
void     drawBox(Window, int, int, int, int);
int      nrand(int);

