/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1.5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 2;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 2;       /* vert inner gap between windows */
static const unsigned int gappoh    = 3;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 3;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayspacing = 2;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray        = 1;     /* 0 means no systray */
static const int showbar            = 3;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "Terminus (TTF):style=Medium:size=9",  // Stable: JetBrains Mono:size=12 ; Japanese: IPAGothic:style=Regular
					"FontAwesome:style=Regular:size=9:antialias=true:autohint=true",
};
static const char dmenufont[]       = "monospace:size=10";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_black[]       = "#000000";
static const char col_gray5[]       = "#9f9ea8";
static const char col_gray6[]       = "#1d2021";
static const char col_cyan[]        = "#005577";
static const char col_cyan2[]       = "#007777";
static const char col_cyan3[]       = "#006677";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray6, NULL },
	[SchemeSel]  = { col_gray4, col_cyan,  col_gray5 },
};

typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd1[] = {"st", "-n", "spterm", "-g", "120x34+460-200", NULL };
const char *spcmd2[] = {"st", "-n", "spcalc", "-g", "50x20", "-e", "bc", "-lq", NULL };
const char *spcmd3[] = {"st", "-n", "spmusic", "-g", "120x34+520-200", "-e", "ncmpcpp", NULL };
static Sp scratchpads[] = {
	/* name          cmd  */
	{"spterm",      spcmd1},
	{"spcalc",      spcmd2},
	{"spmusic",     spcmd3},
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",    NULL,     NULL,           1 << 4,         0,          0,           1,        -1 },
	{ "discord",    NULL,     NULL,           1 << 8,         0,          0,           1,        -1 },
	{ "Slack",    NULL,     NULL,           1 << 7,         0,          0,           1,        -1 },
	{ "zoom",    NULL,     NULL,           1 << 6,         0,          0,           1,        -1 },
	{ "termname",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Matplotlib",     NULL,      NULL,       0,            1, 0, 1,           -1 },
	{ "R_x11",          NULL,      NULL,       0,            1, 0, 1,           -1 },
	{ "gnuplot_qt",     NULL,      NULL,       0,            1, 0, 1,           -1 },
	{ "Qalculate-gtk",  NULL,      NULL,       0,            1, 0, 1,           -1 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
	{ NULL,		  "spterm",		NULL,		SPTAG(0),		1, 0, 1,			 -1 },
	{ NULL,		  "spcalc",		NULL,		SPTAG(1),		1, 0, 1,			 -1 },
	{ NULL,		  "spmusic",	NULL,		SPTAG(2),		1, 0, 1,			 -1 },
};

/* layout(s) */
static const float mfact     = 0.52; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,		NULL },
	{ "|M|",	centeredmaster },		/* Master in middle, slaves on sides */
	{ ">M>",	centeredfloatingmaster },
	{ "[@]",	spiral },		/* Fibonacci spiral */
	{ "[\\]",	dwindle },
	{ "[M]",      monocle },
	{ "[G]",      nogaps },  // tiling with no gaps
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]  = { "st", NULL };

#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */
	// Spawn keybindings
	{ MODKEY,                       XK_d,      spawn,          SHCMD("dmenu_run") },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY|ControlMask,  	        XK_Return, togglescratch,  {.ui = 0 } },
	{ MODKEY,           	        XK_c,	   togglescratch,  {.ui = 1 } },
	{ MODKEY,           	        XK_b,	   togglescratch,  {.ui = 2 } },
	{ MODKEY|Mod1Mask,				XK_Return, spawn,          SHCMD("st -g =140x54") },
/*	{ MODKEY|ControlMask,           XK_c,      spawn,          SHCMD("qalculate-gtk") }, */
	{ MODKEY,                       XK_w,      spawn,          SHCMD("brave") },
	{ MODKEY,                       XK_n,      spawn,          SHCMD("st -e vifm ~/") },
	{ MODKEY|ShiftMask,             XK_m,      spawn,          SHCMD("st -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ MODKEY|ShiftMask,             XK_n,      spawn,          SHCMD("st -e newsboat") },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("bmks") },
	{ MODKEY|ShiftMask,             XK_i,      spawn,          SHCMD("st -f 'DejaVu Sans Mono:pixelsize=17:antialias=true:autohint=true' -e weechat") },
	{ MODKEY,                       XK_p,      spawn,          SHCMD("pcmanfm") },
	{ MODKEY,                       XK_z,      spawn,          SHCMD("shutdownn") },
	{ MODKEY,                       XK_v,      spawn,          SHCMD("tvfzf") },
	{ MODKEY|ControlMask,           XK_s,      spawn,          SHCMD("scrotpick") },
	{ MODKEY|ShiftMask,             XK_q,      spawn,          SHCMD("lgout") },

	// Media keys
	{ 0,                       XF86XK_AudioMute,             spawn,          SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+10 dwmblocks") },
	{ 0,                       XF86XK_AudioRaiseVolume,      spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%; pkill -RTMIN+10 dwmblocks") },
	{ 0,                       XF86XK_AudioLowerVolume,      spawn,          SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%; pkill -RTMIN+10 dwmblocks") },
	{ 0,                       XF86XK_AudioPlay,             spawn,          SHCMD("mpc toggle; pkill -RTMIN+11 dwmblocks") },
	{ 0,                       XF86XK_AudioNext,     	     spawn,          SHCMD("mpc next; pkill -RTMIN+11 dwmblocks") },
	{ 0,                       XF86XK_AudioPrev,     	     spawn,          SHCMD("mpc prev; pkill -RTMIN+11 dwmblocks") },
	{ MODKEY,                  XK_semicolon, 	    	     spawn,          SHCMD("mpc shuffle; notify-send 'Music Player Daemon' 'Queue shuffled!'") },
	{ 0,                       XF86XK_MonBrightnessUp,       spawn,          SHCMD("xbacklight -inc 5; pkill -RTMIN+9 dwmblocks") },
	{ 0,                       XF86XK_MonBrightnessDown,     spawn,          SHCMD("xbacklight -dec 5; pkill -RTMIN+9 dwmblocks") },


	/* { MODKEY,                       XK_y,      togglebar,      {0} }, */
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      pushdown,       {0} },
	{ MODKEY|ShiftMask,             XK_k,      pushup,         {0} },
	{ MODKEY,                       XK_i,      focusmaster,    {0} },
	{ MODKEY|ControlMask,           XK_o,      incnmaster,     {.i = +1 } },
	{ MODKEY|ControlMask,           XK_u,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.02} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.02} },
	{ MODKEY,                       XK_space,  zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_l,      shiftviewclients,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_h,      shiftviewclients,     {.i = -1 } },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,					    XK_e,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[3]} },
	/* { MODKEY,                       XK_space,  setlayout,      {0} }, */
	{ MODKEY|ShiftMask,             XK_e,      togglefloating, {0} },
	{ MODKEY,                       XK_f,      togglefullscreen,  {0} },
	{ MODKEY|ShiftMask,             XK_f,      togglefakefullscreen,  {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_r,      quit,           {0} },
	/* { MODKEY,	                	XK_g,      togglegaps,	   {0} }, */
	{ MODKEY|ControlMask|ShiftMask, XK_h,      incrgaps,       {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_l,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_h,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_l,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_h,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_l,      incrigaps,      {.i = -1 } },
	{ MODKEY|ControlMask,           XK_0,      defaultgaps,    {0} },
	{ MODKEY|ControlMask|ShiftMask, XK_y,      incrihgaps,     {.i = +1 } },
	{ MODKEY|ControlMask|ShiftMask, XK_u,      incrihgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_y,      incrivgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_u,      incrivgaps,     {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_y,      incrohgaps,     {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_u,      incrohgaps,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_y,      incrovgaps,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_u,      incrovgaps,     {.i = -1 } },
    { 0,                       XF86XK_AudioLowerVolume, spawn,  SHCMD("pactl set-sink-volume 0 -5%; pkill -RTMIN+10 dwmblocks")},
	{ 0,                       XF86XK_AudioMute,        spawn,  SHCMD("pactl set-sink-mute 0 toggle; pkill -RTMIN+10 dwmblocks")},
	{ 0,                       XF86XK_AudioRaiseVolume, spawn,  SHCMD("pactl set-sink-volume 0 +5%; pkill -RTMIN+10 dwmblocks")},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
