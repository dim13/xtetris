/*
# GENERIC X-WINDOW-BASED TETRIS
#
#	utils.c
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

#include	"tetris.h"

#include	"ticon.xbm"

#include	"bm-3d/rot00.xbm"
#include	"bm-3d/rot01.xbm"
#include	"bm-3d/rot02.xbm"
#include	"bm-3d/rot03.xbm"
#include	"bm-3d/rot04.xbm"
#include	"bm-3d/rot05.xbm"
#include	"bm-3d/rot06.xbm"
#include	"bm-3d/rot07.xbm"
#include	"bm-3d/rot08.xbm"
#include	"bm-3d/rot09.xbm"
#include	"bm-3d/rot10.xbm"
#include	"bm-3d/rot11.xbm"
#include	"bm-3d/rot12.xbm"
#include	"bm-3d/rot13.xbm"
#include	"bm-3d/rot14.xbm"
#include	"bm-3d/rot15.xbm"

#include	"bm-plain/rop00.xbm"
#include	"bm-plain/rop01.xbm"
#include	"bm-plain/rop02.xbm"
#include	"bm-plain/rop03.xbm"
#include	"bm-plain/rop04.xbm"
#include	"bm-plain/rop05.xbm"
#include	"bm-plain/rop06.xbm"
#include	"bm-plain/rop07.xbm"
#include	"bm-plain/rop08.xbm"
#include	"bm-plain/rop09.xbm"
#include	"bm-plain/rop10.xbm"
#include	"bm-plain/rop11.xbm"
#include	"bm-plain/rop12.xbm"
#include	"bm-plain/rop13.xbm"
#include	"bm-plain/rop14.xbm"
#include	"bm-plain/rop15.xbm"

Atom            delw;

static GC       revGC, bigGC, tinyGC, xorGC;
static GC       thingGCs[NUM_THINGS];

static char    *winName = "GENERIC TETRIS";
static char    *iconName = "TETRIS";

static int      titleLen, titleWidth, authorLen, authorWidth;
static int      titleX, titleY, authorX, authorY;
static int      sX;
static int      sLevelY, sRowsY, sScoreY;

static int      topRWidth, topWidth, topHeight, topMidX, topMidY;
static int      frameX, frameY, frameW, frameH;

static char    *bitmap_data_3d[NUM_BITMAPS] = {
    rot00_bits, rot01_bits, rot02_bits, rot03_bits, rot04_bits, rot05_bits,
    rot06_bits, rot07_bits, rot08_bits, rot09_bits, rot10_bits, rot11_bits,
    rot12_bits, rot13_bits, rot14_bits, rot15_bits
};

static char    *bitmap_data_plain[NUM_BITMAPS] = {
    rop00_bits, rop01_bits, rop02_bits, rop03_bits, rop04_bits, rop05_bits,
    rop06_bits, rop07_bits, rop08_bits, rop09_bits, rop10_bits, rop11_bits,
    rop12_bits, rop13_bits, rop14_bits, rop15_bits
};

static thing_t  possible[NUM_THINGS + NUM_BTHINGS][4] = {
    {
    {{{0, 0, 0, 0}, {4, 5, 5, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 0, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 0, 1, 0, 1, 1},
    {{{0, 0, 0, 0}, {4, 5, 5, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 0, 2, 0, 1, 1},
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 0, 3, 0, 1, 1}
    },

    {
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {4, 9, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 1, 0, 0, 2, 1},
    {{{2, 0, 0, 0}, {12, 5, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 1, 1, 0, 1, 0},
    {{{6, 1, 0, 0}, {10, 0, 0, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 1, 2, 0, 0, 0},
    {{{4, 5, 3, 0}, {0, 0, 8, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 1, 3, 0, 0, 2}
    },

    {
    {{{2, 0, 0, 0}, {10, 0, 0, 0}, {12, 1, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 2, 0, 0, 2, 0},
    {{{6, 5, 1, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 2, 1, 0, 0, 0},
    {{{4, 3, 0, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 2, 2, 0, 0, 1},
    {{{0, 0, 2, 0}, {4, 5, 9, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 2, 3, 0, 1, 2}
    },

    {
    {{{0, 2, 0, 0}, {4, 11, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 3, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {4, 13, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 3, 1, 0, 1, 1},
    {{{2, 0, 0, 0}, {14, 1, 0, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 3, 2, 0, 1, 0},
    {{{4, 7, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 3, 3, 0, 0, 1}
    },

    {
    {{{2, 0, 0, 0}, {12, 3, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 4, 0, 0, 1, 0},
    {{{0, 6, 1, 0}, {4, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 4, 1, 0, 0, 1},
    {{{2, 0, 0, 0}, {12, 3, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 4, 2, 0, 1, 1},
    {{{0, 6, 1, 0}, {4, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 4, 3, 0, 1, 1}
    },

    {
    {{{0, 2, 0, 0}, {6, 9, 0, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 5, 0, 0, 1, 0},
    {{{4, 3, 0, 0}, {0, 12, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 5, 1, 0, 0, 1},
    {{{0, 2, 0, 0}, {6, 9, 0, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 5, 2, 0, 1, 1},
    {{{4, 3, 0, 0}, {0, 12, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 5, 3, 0, 1, 1}
    },

    {
    {{{6, 3, 0, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 2, 6, 0, 0, 0, 0},
    {{{6, 3, 0, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 2, 6, 1, 0, 0, 0},
    {{{6, 3, 0, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 2, 6, 2, 0, 0, 0},
    {{{6, 3, 0, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 2, 6, 3, 0, 0, 0}
    },

    /* bonus mode blocks */

    {
    {{{0, 0, 0, 2}, {4, 5, 5, 9}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 7, 0, 0, 1, 2},
    {{{4, 3, 0, 0}, {0, 10, 0, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 7, 1, 0, 1, 1},
    {{{6, 5, 5, 1}, {8, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 7, 2, 0, 0, 1},
    {{{2, 0, 0, 0}, {10, 0, 0, 0}, {10, 0, 0, 0}, {12, 1, 0, 0}}, 0, 0, 4, 7, 3, 0, 2, 0}
    },

    {
    {{{2, 0, 0, 0}, {12, 5, 5, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 8, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {0, 10, 0, 0}, {4, 9, 0, 0}}, 0, 0, 4, 8, 1, 0, 2, 1},
    {{{4, 5, 5, 3}, {0, 0, 0, 8}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 8, 2, 0, 0, 2},
    {{{6, 1, 0, 0}, {10, 0, 0, 0}, {10, 0, 0, 0}, {8, 0, 0, 0}}, 0, 0, 4, 8, 3, 0, 1, 0}
    },

    {
    {{{0, 2, 0, 0}, {4, 13, 5, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 9, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {4, 11, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 9, 1, 0, 2, 1},
    {{{4, 5, 7, 1}, {0, 0, 8, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 9, 2, 0, 0, 2},
    {{{2, 0, 0, 0}, {14, 1, 0, 0}, {10, 0, 0, 0}, {8, 0, 0, 0}}, 0, 0, 4, 9, 3, 0, 1, 0}
    },

    {
    {{{0, 0, 2, 0}, {4, 5, 13, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 10, 0, 0, 1, 2},
    {{{0, 2, 0, 0}, {4, 11, 0, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 10, 1, 0, 1, 1},
    {{{4, 7, 5, 1}, {0, 8, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 10, 2, 0, 0, 1},
    {{{2, 0, 0, 0}, {10, 0, 0, 0}, {14, 1, 0, 0}, {8, 0, 0, 0}}, 0, 0, 4, 10, 3, 0, 2, 0}
    },

    {
    {{{4, 7, 1, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 11, 0, 0, 0, 1},
    {{{2, 0, 0, 0}, {14, 5, 1, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 11, 1, 0, 1, 0},
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {4, 13, 1, 0}, {0, 0, 0, 0}}, 0, 0, 3, 11, 2, 0, 2, 1},
    {{{0, 0, 2, 0}, {4, 5, 11, 0}, {0, 0, 8, 0}, {0, 0, 0, 0}}, 0, 0, 3, 11, 3, 0, 1, 2}
    },

    {
    {{{2, 0, 0, 0}, {12, 3, 0, 0}, {0, 10, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 12, 0, 0, 1, 1},
    {{{0, 6, 5, 1}, {4, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 12, 1, 0, 0, 1},
    {{{2, 0, 0, 0}, {10, 0, 0, 0}, {12, 3, 0, 0}, {0, 8, 0, 0}}, 0, 0, 4, 12, 2, 0, 2, 0},
    {{{0, 0, 6, 1}, {4, 5, 9, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 12, 3, 0, 1, 2}
    },

    {
    {{{4, 3, 0, 0}, {0, 12, 5, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 13, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {0, 10, 0, 0}, {6, 9, 0, 0}, {8, 0, 0, 0}}, 0, 0, 4, 13, 1, 0, 2, 1},
    {{{4, 5, 3, 0}, {0, 0, 12, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 4, 13, 2, 0, 0, 2},
    {{{0, 2, 0, 0}, {6, 9, 0, 0}, {10, 0, 0, 0}, {8, 0, 0, 0}}, 0, 0, 4, 13, 3, 0, 1, 0}
    },

    {
    {{{2, 0, 0, 0}, {12, 7, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 14, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {0, 14, 1, 0}, {4, 9, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 14, 1, 0, 1, 1},
    {{{0, 2, 0, 0}, {4, 13, 3, 0}, {0, 0, 8, 0}, {0, 0, 0, 0}}, 0, 0, 3, 14, 2, 0, 1, 1},
    {{{0, 6, 1, 0}, {4, 11, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 14, 3, 0, 1, 1}
    },

    {
    {{{4, 3, 0, 0}, {0, 14, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 15, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {6, 13, 1, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 15, 1, 0, 1, 1},
    {{{0, 2, 0, 0}, {4, 11, 0, 0}, {0, 12, 1, 0}, {0, 0, 0, 0}}, 0, 0, 3, 15, 2, 0, 1, 1},
    {{{0, 0, 2, 0}, {4, 7, 9, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 15, 3, 0, 1, 1}
    },

    {
    {{{6, 7, 1, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 16, 0, 0, 0, 1},
    {{{2, 0, 0, 0}, {14, 3, 0, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 16, 1, 0, 1, 0},
    {{{0, 6, 3, 0}, {4, 13, 9, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 16, 2, 0, 1, 1},
    {{{6, 3, 0, 0}, {12, 11, 0, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 16, 3, 0, 1, 1}
    },

    {
    {{{6, 3, 0, 0}, {12, 13, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 17, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {6, 11, 0, 0}, {12, 9, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 17, 1, 0, 1, 1},
    {{{4, 7, 3, 0}, {0, 12, 9, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 17, 2, 0, 0, 1},
    {{{6, 3, 0, 0}, {14, 9, 0, 0}, {8, 0, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 17, 3, 0, 1, 0}
    },

    {
    {{{0, 2, 0, 0}, {4, 15, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 18, 0, 0, 1, 1},
    {{{0, 2, 0, 0}, {4, 15, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 18, 1, 0, 1, 1},
    {{{0, 2, 0, 0}, {4, 15, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 18, 2, 0, 1, 1},
    {{{0, 2, 0, 0}, {4, 15, 1, 0}, {0, 8, 0, 0}, {0, 0, 0, 0}}, 0, 0, 3, 18, 3, 0, 1, 1}
    }
};

static int      checkUp[NUM_BITMAPS] = {
    0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7
};
static int      checkDown[NUM_BITMAPS] = {
    0, 1, 0, 1, 4, 5, 4, 5, 8, 9, 8, 9, 12, 13, 12, 13
};

static char    *thingFGs[NUM_THINGS] = {
    "Red", "ForestGreen", "Blue", "Magenta", "DarkTurquoise", "Brown", "Black"
};

static Pixmap   pms[NUM_BITMAPS];

static thing_t  curThing, nextThing;
static struct {
    int             pm_num, cid;
}               field[ROWS][COLS];

/* ------------------------------------------------------------------ */

int
nrand(int n)
{
    return (int) (LRAND() % n);
}

/* ------------------------------------------------------------------ */

unsigned long
getColor(char name[])
{
    XColor          tmp;

    if (!useColor) {
	return BlackPixel(display, screen_num);
    }
    if (XParseColor(display, colormap, name, &tmp) == 0) {
	(void) fprintf(stderr, "Tetris: invalid color '%s'.\n", name);
	return BlackPixel(display, screen_num);
    }
    if (XAllocColor(display, colormap, &tmp) == 0) {
	(void) fprintf(stderr, "Tetris: can't allocate color '%s'.\n", name);
	return BlackPixel(display, screen_num);
    }
    return tmp.pixel;
}

/* ------------------------------------------------------------------ */

void
inits(int argc, char *argv[])
{
    XSetWindowAttributes att;
    unsigned int    attvm;
    XTextProperty   wName, iName;
    XClassHint      classhints;
    XEvent          ev;
    XGCValues       gcv;
    unsigned long   gcvm;
    int             i, j;

    SRAND(time(NULL));

    for (i = 0; i < COLS; i++)
	for (j = 0; j < ROWS; j++)
	    if ((j >= ROWS - prefilled) && (nrand(2) == 0)) {
		field[j][i].pm_num = 0;
		field[j][i].cid = nrand(NUM_THINGS);
	    } else {
		field[j][i].pm_num = -1;
		field[j][i].cid = 0;
	    }

    titleLen = strlen(MSG_TITLE);
    titleWidth = XTextWidth(bigFont, MSG_TITLE, titleLen);
    authorLen = strlen(MSG_AUTHOR);
    authorWidth = XTextWidth(tinyFont, MSG_AUTHOR, authorLen);

    frameW = BOXSIZE * COLS;
    frameH = BOXSIZE * ROWS;
    topRWidth = BOXSIZE * THINGSIZE + OFFSET * 2;
    topHeight = frameH + OFFSET * 2 + 4;
    if (titleWidth > topRWidth)
	topRWidth = titleWidth;
    if (authorWidth > topRWidth)
	topRWidth = authorWidth;
    topMidX = frameW + OFFSET * 2 + 4;
    topMidY = topHeight / 2 + bigFont->ascent;
    topWidth = topMidX + topRWidth;
    frameX = frameY = OFFSET + 2;

    titleX = (topRWidth - titleWidth) / 2 + topMidX;
    titleY = OFFSET + 2 + bigFont->ascent;
    authorX = (topRWidth - authorWidth) / 2 + topMidX;
    authorY = OFFSET + 2 + bigFont->ascent + bigFont->descent + tinyFont->ascent;

    sX = topMidX + OFFSET;
    sScoreY = topHeight - OFFSET - 2 - tinyFont->descent;
    sRowsY = sScoreY - tinyFont->descent - tinyFont->ascent - 2;
    sLevelY = sRowsY - tinyFont->descent - tinyFont->ascent - 2;

    sizehints.width = (sizehints.min_width =
	(sizehints.max_width = topWidth));
    sizehints.height = (sizehints.min_height =
	(sizehints.max_height = topHeight));

    theCursor = XCreateFontCursor(display, XC_exchange);

    /* arrow keys */
    XRebindKeysym(display, XK_R10, NULL, 0,
	(unsigned char *) "j", sizeof(unsigned char));
    XRebindKeysym(display, XK_Left, NULL, 0,
	(unsigned char *) "j", sizeof(unsigned char));
    XRebindKeysym(display, XK_R8, NULL, 0,
	(unsigned char *) "k", sizeof(unsigned char));
    XRebindKeysym(display, XK_Up, NULL, 0,
	(unsigned char *) "k", sizeof(unsigned char));
    XRebindKeysym(display, XK_R11, NULL, 0,
	(unsigned char *) "k", sizeof(unsigned char));
    XRebindKeysym(display, XK_R12, NULL, 0,
	(unsigned char *) "l", sizeof(unsigned char));
    XRebindKeysym(display, XK_Right, NULL, 0,
	(unsigned char *) "l", sizeof(unsigned char));
    XRebindKeysym(display, XK_Down, NULL, 0,
	(unsigned char *) " ", sizeof(unsigned char));
    XRebindKeysym(display, XK_R14, NULL, 0,
	(unsigned char *) " ", sizeof(unsigned char));

    /* create windows */
    attvm = CWBackPixel | CWEventMask | CWDontPropagate | CWCursor;
    att.background_pixel = bg;
    att.event_mask = ExposureMask | KeyPressMask |
	StructureNotifyMask | FocusChangeMask;
    att.do_not_propagate_mask = KeyReleaseMask |
	ButtonPressMask | ButtonReleaseMask | PointerMotionMask |
	ButtonMotionMask | Button1MotionMask | Button2MotionMask |
	Button3MotionMask | Button4MotionMask | Button5MotionMask;
    att.cursor = theCursor;

    mainWin = XCreateWindow(display, DefaultRootWindow(display),
	sizehints.x, sizehints.y, topWidth, topHeight, 0,
	CopyFromParent, InputOutput, CopyFromParent, attvm, &att);

    attvm = CWBackPixel | CWBorderPixel | CWEventMask;
    att.border_pixel = fg;
    att.event_mask = ExposureMask;

    blockWin = XCreateWindow(display, mainWin,
	frameX-2, frameY-2, frameW, frameH, 2,
	CopyFromParent, InputOutput, CopyFromParent, attvm, &att);

    /* WM hints */
    XStringListToTextProperty(&winName, 1, &wName);
    XStringListToTextProperty(&iconName, 1, &iName);

    wmhints.icon_pixmap = XCreateBitmapFromData(display,
	mainWin, ticon_bits, ticon_width, ticon_height);
    classhints.res_name = "tetris";
    classhints.res_class = "Tetris";

    XSetWMProperties(display, mainWin, &wName, &iName,
	argv, argc, &sizehints, &wmhints, &classhints);

    delw = XInternAtom(display, "WM_DELETE_WINDOW", False);
    XSetWMProtocols(display, mainWin, &delw, 1);

    /* GC's */

    gcvm = GCForeground | GCBackground | GCFunction |
	GCFont | GCGraphicsExposures;

    gcv.function = GXcopy;
    gcv.foreground = fg;
    gcv.background = bg;
    gcv.font = bigFont->fid;
    gcv.graphics_exposures = False;
    bigGC = XCreateGC(display, mainWin, gcvm, &gcv);

    gcv.font = tinyFont->fid;
    tinyGC = XCreateGC(display, mainWin, gcvm, &gcv);

    gcv.foreground = bg;
    gcv.background = fg;
    revGC = XCreateGC(display, mainWin, gcvm, &gcv);

    gcv.background = bg;
    for (i = 0; i < NUM_THINGS; i++) {
	gcv.foreground = getColor(thingFGs[i]);
	if (gcv.foreground == bg)
	    gcv.foreground = fg;
	thingGCs[i] = XCreateGC(display, blockWin, gcvm, &gcv);
    }

    gcv.foreground = fg;
    gcv.function = GXxor;
    xorGC = XCreateGC(display, blockWin, gcvm, &gcv);

    /* pixmaps */

    if (use3D)
	for (i = 0; i < NUM_BITMAPS; i++) {
	    pms[i] = (Pixmap) XCreateBitmapFromData(display, blockWin,
		bitmap_data_3d[i], BOXSIZE, BOXSIZE);
	}
    else for (i = 0; i < NUM_BITMAPS; i++) {
	    pms[i] = (Pixmap) XCreateBitmapFromData(display, blockWin,
		bitmap_data_plain[i], BOXSIZE, BOXSIZE);
	}

    /* new things */
    newThing();
    newThing();

    /* the last thing is to wait for mapped */
    XMapWindow(display, blockWin);
    XMapRaised(display, mainWin);
    XNextEvent(display, &ev);
}

/* ------------------------------------------------------------------ */

void
newThing(void)
{
    curThing = nextThing;
    nextThing = possible[nrand(range)][nrand(4)];
    nextThing.xpos = nrand(COLS - nextThing.size + 1);
    nextThing.ypos = 2 - nextThing.size;
    nextThing.cid = (bonusMode) ? nrand(NUM_THINGS) : nextThing.px;
}

/* ------------------------------------------------------------------ */

void
drawTitle(void)
{
    XDrawString(display, mainWin, bigGC,
	titleX, titleY, MSG_TITLE, titleLen);
    XDrawString(display, mainWin, tinyGC,
	authorX, authorY, MSG_AUTHOR, authorLen);
}

/* ------------------------------------------------------------------ */

void
drawStatus(void)
{
    char            buf[30];

    (void) sprintf(buf, "Score: %d", score);
    XDrawImageString(display, mainWin, tinyGC, sX, sScoreY, buf, strlen(buf));

    (void) sprintf(buf, "Level: %d    ", level);
    XDrawImageString(display, mainWin, tinyGC, sX, sLevelY, buf, strlen(buf));

    (void) sprintf(buf, "Rows: %d", rows);
    XDrawImageString(display, mainWin, tinyGC, sX, sRowsY, buf, strlen(buf));
}

/* ------------------------------------------------------------------ */

void
drawBox(Window win, int pmid, int cid, int x, int y)
{
    XCopyPlane(display, pms[pmid], win, thingGCs[cid], 0, 0,
	BOXSIZE, BOXSIZE, x, y, (unsigned long) 1);
}

/* ------------------------------------------------------------------ */

static void
clearBox(int x, int y)
{
    XFillRectangle(display, blockWin, revGC, x, y, BOXSIZE, BOXSIZE);
}

/* ------------------------------------------------------------------ */

void
drawField(void)
{
    int             i, j;

    for (i = 0; i < COLS; i++)
	for (j = 0; j < ROWS; j++)
	    if (field[j][i].pm_num >= 0)
		drawBox(blockWin, field[j][i].pm_num, field[j][i].cid,
			i * BOXSIZE, j * BOXSIZE);
}

/* ------------------------------------------------------------------ */

void
drawThing(void)
{
    int             i, j;

    for (i = 0; i < curThing.size; i++)
	for (j = 0; j < curThing.size; j++)
	    if ((curThing.ypos + j >= 0) && curThing.map[j][i])
		drawBox(blockWin, curThing.map[j][i], curThing.cid,
			(curThing.xpos + i) * BOXSIZE,
			(curThing.ypos + j) * BOXSIZE);
}

/* ------------------------------------------------------------------ */

void
drawThingDiff(thing_t *old)
{
    int             i, j, ox, oy;

    for (i = 0; i < curThing.size; i++)
	for (j = 0; j < curThing.size; j++)
	    if ((curThing.ypos + j >= 0) && curThing.map[j][i])
		drawBox(blockWin, curThing.map[j][i], curThing.cid,
			(curThing.xpos + i) * BOXSIZE,
			(curThing.ypos + j) * BOXSIZE);

    for (i = 0; i < curThing.size; i++)
	for (j = 0; j < curThing.size; j++) {
	    ox = old->xpos + i - curThing.xpos;
	    oy = old->ypos + j - curThing.ypos;
	    if ((old->ypos + j >= 0) && old->map[j][i] && 
			((ox < 0) || (ox >= curThing.size) ||
			(oy < 0) || (oy >= curThing.size) ||
			!curThing.map[oy][ox]))
		clearBox((old->xpos + i) * BOXSIZE, (old->ypos + j) * BOXSIZE);
	}
}

/* ------------------------------------------------------------------ */

void
drawNext(void)
{
    int             x, y;
    int             i, j;

    x = topMidX + (topRWidth - nextThing.size * BOXSIZE) / 2;
    y = topMidY - nextThing.size * BOXSIZE / 2;
    for (i = 0; i < nextThing.size; i++)
	for (j = 0; j < nextThing.size; j++)
	    if (nextThing.map[j][i])
		drawBox(mainWin, nextThing.map[j][i], nextThing.cid,
		x + i * BOXSIZE, y + j * BOXSIZE);
}

/* ------------------------------------------------------------------ */

void
clearNext(void)
{
    XFillRectangle(display, mainWin, revGC,
	topMidX, topMidY - BOXSIZE * 2, topRWidth, BOXSIZE * 4);
}

/* ------------------------------------------------------------------ */

void
banner(char msg[])
{
    int             mlen = strlen(msg);
    int             w = XTextWidth(bigFont, msg, mlen);
    int             x = (topRWidth - w)/2 + topMidX;

    clearNext();
    XFillRectangle(display, mainWin, revGC,
	x - 60, topMidY - bigFont->ascent - 5,
	w + 120, bigFont->ascent + bigFont->descent + 10);
    XDrawString(display, mainWin, bigGC, x, topMidY, msg, mlen);
}

/* ------------------------------------------------------------------ */

void
putBox(void)
{
    int             i, j;
    int             x = curThing.xpos, y = curThing.ypos;

    for (i = 0; i < curThing.size; i++)
	for (j = 0; j < curThing.size; j++)
	    if ((y + j >= 0) && curThing.map[j][i]) {
		field[y + j][x + i].pm_num = curThing.map[j][i];
		field[y + j][x + i].cid = curThing.cid;
	    }
}

/* ------------------------------------------------------------------ */

Bool
overlapping(void)
{
    int             i, j;
    int             x = curThing.xpos, y = curThing.ypos;

    for (i = 0; i < curThing.size; i++)
	for (j = 0; j < curThing.size; j++)
	    if (curThing.map[j][i]) {
		if ((y + j >= ROWS) || (x + i < 0) || (x + i >= COLS))
		    return True;
		if ((y + j >= 0) && (field[y + j][x + i].pm_num >= 0))
		    return True;
	    }

    return False;
}

/* ------------------------------------------------------------------ */

Bool
atBottom(void)
{
    int             i, j;
    int             x = curThing.xpos, y = curThing.ypos;

    for (i = 0; i < curThing.size; i++)
	for (j = 0; j < curThing.size; j++)
	    if ((y + j >= 0) && curThing.map[j][i]) {
		if ((y + j >= ROWS - 1) || (x + i < 0) || (x + i >= COLS))
		    return True;
		if (field[y + j + 1][x + i].pm_num >= 0)
		    return True;
	    }

    return False;
}

/* ------------------------------------------------------------------ */

void
tryMove(move_t move)
{
    thing_t         old;
    Bool            canMove = False;

    old = curThing;

    switch (move) {
    case FALL:
	curThing.ypos ++;
	break;

    case DROP:
	do {
	    curThing.ypos ++;
	    score += level + prefilled;
	} while (!overlapping());
	curThing.ypos --;
	score += (bonusMode ? 3 : 1) * level;
	break;

    case ROTATE:
	curThing = possible[old.px][(old.py + rotOffset) % 4];
	/* xpos, ypos, cid got overwritten */
	curThing.xpos = old.xpos + old.gy - curThing.gy;
	curThing.ypos = old.ypos + old.gx - curThing.gx;
	curThing.cid = old.cid;
	break;

    case LEFT:
	curThing.xpos --;
	break;

    case RIGHT:
	curThing.xpos ++;
	break;

    case REFLECT:
	break;
    }

    if (overlapping()) {
	if (move == ROTATE) {
	    while (!canMove && (curThing.xpos + curThing.size >= COLS)) {
		curThing.xpos --;
		canMove = !overlapping();
    	    }
	    while (!canMove && (curThing.xpos < 0)) {
		curThing.xpos ++;
		canMove = !overlapping();
    	    }
	}
    } else
    	canMove = True;

    if (canMove)
	drawThingDiff(&old);
    else
	curThing = old;
}

/* ------------------------------------------------------------------ */

int
checkLines(void)
{
    int             lSet[ROWS], nset = 0;
    int             i, j, y;

    for (j = 0; j < ROWS; j++) {
	lSet[j] = 0;
	for (i = 0; i < COLS; i++)
	    if (field[j][i].pm_num >= 0)
		lSet[j] ++;
	if (lSet[j] == COLS)
	    nset ++;
    }

    if (nset) {
	for (i = 0; i < ((NUM_FLASHES / nset) % 2) * 2; i ++) {
	    for (j = 0; j < ROWS; j++) {
		if (lSet[j] == COLS)
		    XFillRectangle(display, blockWin, xorGC,
			0, j * BOXSIZE, frameW, BOXSIZE);
	    }
	    XFlush(display);
	}

	for (j = ROWS-1; j >= 0; j--) {
	    if (lSet[j] == COLS) {
		for (y = j; y > 0; y--)
		    for (i = 0; i < COLS; i++)
			field[y][i] = field[y-1][i];
		for (i = 0; i < COLS; i++)
		    field[0][i].pm_num = -1;

		XCopyArea(display, blockWin, blockWin, tinyGC,
			0, 0, frameW, j * BOXSIZE, 0, BOXSIZE);
		
		XFillRectangle(display, blockWin, revGC,
			0, 0, frameW, BOXSIZE);

		for (i = j; i > 0; i--)
		    lSet[i] = lSet[i-1];
		lSet[0] = 0;

		if (j > 0)
		    for (i = 0; i < COLS; i++) {
			int             tmp = field[j][i].pm_num;
			if ((tmp >= 0) && (tmp != checkDown[tmp])) {
			    field[j][i].pm_num = checkDown[tmp];
			    drawBox(blockWin, field[j][i].pm_num,
				field[j][i].cid, i * BOXSIZE, j * BOXSIZE);
			}
		    }

		j++;

		if (j < ROWS)
		    for (i = 0; i < COLS; i++) {
			int             tmp = field[j][i].pm_num;
			if ((tmp >= 0) && (tmp != checkUp[tmp])) {
			    field[j][i].pm_num = checkUp[tmp];
			    drawBox(blockWin, field[j][i].pm_num,
				field[j][i].cid, i * BOXSIZE, j * BOXSIZE);
			}
		    }

		XFlush(display);
	    }
	}

	if (beep) XBell(display, BVOLUME);
	XSync(display, False);
    }

    return nset;
}

/* ------------------------------------------------------------------ */

void
realTime(struct timeval *tv)
{
    while (tv->tv_usec < 0) {
	tv->tv_sec --;
	tv->tv_usec += MILLION;
    }
    while (tv->tv_usec >= MILLION) {
	tv->tv_sec ++;
	tv->tv_usec -= MILLION;
    }
}

/* ------------------------------------------------------------------ */
