/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 2;        /* border pixel of windows */
static const unsigned int snap           = 32;       /* snap pixel */
static const int swallowfloating        = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih         = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv         = 10;       /* vert inner gap between windows */
static const unsigned int gappoh         = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov         = 10;       /* vert outer gap between windows and screen edge */
static       int smartgaps               = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */
static const unsigned int systraypinning = 0;        /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;        /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 2;        /* systray spacing */
static const int systraypinningfailfirst = 1;        /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;        /* 0 means no systray */
static const char *fonts[]               = { "RobotoMono Nerd Font:size=10" };
static const char dmenufont[]            = "RobotoMono Nerd Font:size=12";
static const char col_gray1[]            = "#222222";
static const char col_gray2[]            = "#444444";
static const char col_gray3[]            = "#bbbbbb";
static const char col_gray4[]            = "#eeeeee";
static const char col_gray5[]            = "#41474a";
static const char col_cyan[]             = "#005577";
static const char *colors[][3]           = {

  /*               fg         bg         border   */
  [SchemeNorm] = { col_gray3, col_gray1, col_gray1 },
  [SchemeSel]  = { col_gray4, col_gray5, col_gray5 },
  [SchemeHid]  = { col_cyan,  col_gray1, col_gray5 },
};

#define ICONSPACING 5         /* space (pixels) between icon and title */
#define ICONSIZE (bh - 4)     /* adaptively preserve 2 pixels each side of win icon */

static const char *const autostart[] = {
	"sh", "-c", "~/.local/bin/autostart.sh", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
  /* xprop(1):
   *  WM_CLASS(STRING) = instance, class
   *  WM_NAME(STRING) = title
   */
  /* class       instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
  { "Gimp",      NULL,     NULL,           0,         1,          0,          0,         -1 },
  { "Firefox",   NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
  { "Alacritty", NULL,     NULL,           0,         0,          1,          0,         -1 },
  { NULL,        NULL,     "Event Tester", 0,         0,          0,          1,         -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "movestack.c"

static const Layout layouts[] = {
  /* symbol     arrange function */
  { "[]=",      tile },    /* first entry is default */
  { "|||",      col },
  { "[M]",      monocle },
  { "[@]",      spiral },
  { "[\\]",     dwindle },
  { "H[]",      deck },
  { "TTT",      bstack },
  { "===",      bstackhoriz },
  { "HHH",      grid },
  { "###",      nrowgrid },
  { "---",      horizgrid },
  { ":::",      gaplessgrid },
  { "|M|",      centeredmaster },
  { ">M>",      centeredfloatingmaster },
  { "><>",      NULL },    /* no layout function means floating behavior */
  { NULL,       NULL },
};

/* key definitions */
#define MODKEY Mod4Mask
#define ALTKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
  { MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
  { MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
  { MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */

// static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, "-l", "15", "-p", "Run:", NULL };
static const char *dmenucmd[] = { "dmenu_desktop", dmenumon, dmenufont, col_gray1, col_gray3, col_cyan, col_gray4, "--test-flag", NULL };

// static const char *dmenudesktopcmd[] = { "j4-dmenu-desktop", dmenuarg, "--term='alacritty'", "--no-generic", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
// static const char *browsercmd[]  = { "librewolf", NULL };

static const Key keys[] = {
  /* modifier                           key        function           argument */
  { MODKEY,                             XK_Return, spawn,             {.v = termcmd } },
  { MODKEY|ShiftMask,                   XK_Return, spawn,             {.v = dmenucmd} },
  // { MODKEY,                             XK_q,      spawn,             {.v = browsercmd } },
  { MODKEY,                             XK_b,      togglebar,         {0} },
  { MODKEY,                             XK_j,      focusstackvis,     {.i = +1 } },
  { MODKEY,                             XK_k,      focusstackvis,     {.i = -1 } },
  { MODKEY|ALTKEY,			XK_j,      focusstackhid,     {.i = +1 } },
  { MODKEY|ALTKEY,                   	XK_k,      focusstackhid,     {.i = -1 } },
  { MODKEY|ShiftMask,			XK_j,      movestack,	      {.i = +1 } },
  { MODKEY|ShiftMask,             	XK_k,      movestack,         {.i = -1 } },
  { MODKEY,                             XK_i,      incnmaster,        {.i = +1 } },
  { MODKEY,                             XK_d,      incnmaster,        {.i = -1 } },
  { MODKEY,                             XK_h,      setmfact,          {.f = -0.05} },
  { MODKEY,                             XK_l,      setmfact,          {.f = +0.05} },
  { MODKEY|ShiftMask,                   XK_h,      setcfact,          {.f = +0.25} },
  { MODKEY|ShiftMask,                   XK_l,      setcfact,          {.f = -0.25} },
  { MODKEY|ShiftMask,                   XK_o,      setcfact,          {.f =  0.00} },
  { MODKEY|ALTKEY,                      XK_Return, zoom,              {0} },
  { MODKEY|ALTKEY,                      XK_u,      incrgaps,          {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_u,      incrgaps,          {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_i,      incrigaps,         {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_i,      incrigaps,         {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_o,      incrogaps,         {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_o,      incrogaps,         {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_6,      incrihgaps,        {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_6,      incrihgaps,        {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_7,      incrivgaps,        {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_7,      incrivgaps,        {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_8,      incrohgaps,        {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_8,      incrohgaps,        {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_9,      incrovgaps,        {.i = +5 } },
  { MODKEY|ALTKEY|ControlMask,          XK_9,      incrovgaps,        {.i = -5 } },
  { MODKEY|ALTKEY,                      XK_0,      togglegaps,        {0} },
  { MODKEY|ALTKEY|ControlMask,          XK_0,      defaultgaps,       {0} },
  { MODKEY,                             XK_Tab,    view,              {0} },
  { MODKEY,                             XK_w,      killclient,        {0} },
  { MODKEY,                             XK_t,      setlayout,         {.v = &layouts[0]} },
  { MODKEY,                             XK_f,      setlayout,         {.v = &layouts[1]} },
  { MODKEY,                             XK_m,      setlayout,         {.v = &layouts[13]} },
  { MODKEY,                             XK_space,  setlayout,         {0} },
  { MODKEY|ShiftMask,                   XK_space,  togglefloating,    {0} },
  { MODKEY|ControlMask,                 XK_space,  togglealwaysontop, {0} },
  { MODKEY,                             XK_0,      view,              {.ui = ~0 } },
  { MODKEY|ShiftMask,                   XK_0,      tag,               {.ui = ~0 } },
  { MODKEY,                             XK_comma,  focusmon,          {.i = -1 } },
  { MODKEY,                             XK_period, focusmon,          {.i = +1 } },
  { MODKEY|ShiftMask,                   XK_comma,  tagmon,            {.i = -1 } },
  { MODKEY|ShiftMask,                   XK_period, tagmon,            {.i = +1 } },
  { MODKEY,                             XK_s,      show,              {0} },
  { MODKEY|ControlMask|ShiftMask,       XK_s,      showall,           {0} },
  { MODKEY|ControlMask|ShiftMask,       XK_h,      hide,              {0} },
  { MODKEY|ControlMask,                 XK_comma,  cyclelayout,       {.i = -1 } },
  { MODKEY|ControlMask,                 XK_period, cyclelayout,       {.i = +1 } },
  TAGKEYS(                              XK_1,                          0)
  TAGKEYS(                              XK_2,                          1)
  TAGKEYS(                              XK_3,                          2)
  TAGKEYS(                              XK_4,                          3)
  TAGKEYS(                              XK_5,                          4)
  TAGKEYS(                              XK_6,                          5)
  TAGKEYS(                              XK_7,                          6)
  TAGKEYS(                              XK_8,                          7)
  TAGKEYS(                              XK_9,                          8)
  { MODKEY|ShiftMask,                   XK_q,      quit,              {0} },
  { MODKEY|ControlMask|ShiftMask,       XK_q,      quit,              {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
  /* click                event mask      button          function        argument */
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
  { ClkWinTitle,          0,              Button2,        zoom,           {0} },
  { ClkWinTitle,          0,              Button1,        togglewin,      {0} },
  { ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
  { ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
  { ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
  { ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
  { ClkTagBar,            0,              Button1,        view,           {0} },
  { ClkTagBar,            0,              Button3,        toggleview,     {0} },
  { ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
  { ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
