/* ============================================================================
 * dwm config.h
 * ==========================================================================*/

/* ---------------------------- appearance --------------------------------- */
static unsigned int borderpx = 1;              // window border pixel width
static const unsigned int maximalistborderpx = 2; // border width used in Maximalist Mode
static const int notchbezelhi = 14;  // % lighten applied to notch ColBg for the top/left highlight line
static const int notchbezello = -22; // % darken applied to notch ColBg for the bottom/right shadow line
static const unsigned int dockapppad = 2; // px of pywal-bezeled tile shown around a wrapped real dockapp's icon window
static const int dockclearance = 67; // Width reserved on the right edge for the dockapp column while Maximalist Mode is on
static const double maximalistresizestep = 0.20; // Percentage (as decimal) resizemaximalist() grows/shrinks the focused window by in Maximalist Mode
static const int resizestep = 10; // pixels resizeedge() grows/shrinks a floating window's edge by per keypress (arrow keys)
static unsigned int snap = 3;                  // distance to screen edge where windows snap
static const unsigned int gappih = 20;         // horizontal gap between windows (inner)
static const unsigned int gappiv = 20;         // vertical gap between windows (inner)
static const unsigned int gappoh = 20;         // horizontal gap to screen edge (outer)
static const unsigned int gappov = 30;         // vertical gap to screen edge (outer)
static int smartgaps = 0;
static const int swallowfloating = 0;
static int showbar = 1;                        // If bar in normal mode shows
static const int showtitle = 1;                // If window title shows
static const int showtags = 1;                 // If tags show
static const int showlayout = 1;               // If layout icon shows at bar
static const int showstatus = 1;               // If status shows
static const int showfloating = 0;
static int topbar = 1;                         // 1 Bar on top, 0 Bar on bottom

static char dmenufont[] = "Iosevka Nerd Font:size=12";
static const char *fonts[] = {
    "Iosevka Nerd Font:size=12",
    "NotoColorEmoji:pixelsize=12:antialias=true:autohint=true"
};

// Colours if xrdb isn't loaded
static char normbgcolor[]     = "#1f1c14";
static char normbordercolor[] = "#1f1c14";
static char normfgcolor[]     = "#B5976E";
static char selfgcolor[]      = "#1f1c14";
static char selbordercolor[]  = "#928c82";
static char selbgcolor[]      = "#B5976E";
static char termbgcolor[]     = "#1f1c14"; // mirrors *.background (the resource terminals read) via loadxrdb(); keeps the unfocused notch the same color as terminal backgrounds

// Colours if xrdb is loaded
static char *colors[][3] = {
    [SchemeNorm]      = {normfgcolor,   normbgcolor, normbordercolor},
    [SchemeSel]       = {selbgcolor,    selfgcolor,  selbordercolor},
    [SchemeStatus]    = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeTagsSel]   = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeTagsNorm]  = {selbordercolor,normbgcolor, normbgcolor},
    [SchemeInfoSel]   = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeInfoNorm]  = {normfgcolor,   normbgcolor, normbgcolor},
    [SchemeNotchSel]  = {selfgcolor,    selbgcolor,  selbordercolor},
    [SchemeNotchNorm] = {normfgcolor,   termbgcolor, normbordercolor},
};

/* ---------------------------- shortcuts/macros ---------------------------- */
#define STATUSBAR  "dwmblocks"
#define HOME       "/home/atego"  // easy way to change who owns some configs (keybinds)
#define MODKEY     Mod4Mask       // Windows key
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }
static const char maximaliststatefile[] = HOME "/.cache/dwm/maximalist_state"; // stores Maximalist Mode on/off so it survives reboots, not just dwm restarts
static const char focushoverstatefile[] = HOME "/.cache/dwm/focusonhover_state"; // stores the focus-on-hover setting so it survives reboots

/* ---------------------------- tags ---------------------------------------- */
static const char *tags[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};

/* ---------------------------- layouts -------------------------------------
 * vanitygaps.c is included here because it provides the spiral/dwindle
 * layout functions referenced in layouts[] below.
 * XF86keysym.h is included here because it provides the media-key symbols
 * referenced in keys[] further down.
 * --------------------------------------------------------------------------*/
#include "vanitygaps.c"
#include <X11/XF86keysym.h>

static const float mfact = 0.5;     // size of master area
static const int nmaster = 1;        // number of clients in master area
static const int resizehints = 1;
static const int lockfullscreen = 0; // 1 = fullscreen can't be toggled off

static const Layout layouts[] = {
    {"", tile},
    {"", NULL},
    {"", monocle},
    {"", spiral},
    {"󱤆", dwindle},
};

/* ---------------------------- keybind macros ------------------------------- */
#define TAGKEYS(KEY, TAG)                                                      \
  {MODKEY, KEY, view, {.ui = 1 << TAG}},                                       \
      {MODKEY | ControlMask, KEY, toggleview, {.ui = 1 << TAG}},               \
      {MODKEY | ShiftMask, KEY, tag, {.ui = 1 << TAG}},                        \
      {MODKEY | ControlMask | ShiftMask, KEY, toggletag, {.ui = 1 << TAG}},

#define STACKKEYS(MOD, ACTION)                                                 \
  {MOD, XK_j, ACTION##stack, {.i = INC(+1)}},                                  \
      {MOD,                                                                    \
       XK_k,                                                                   \
       ACTION##stack,                                                          \
       {.i = INC(-1)}}, /*{ MOD, XK_grave, ACTION##stack, {.i = PREVSEL } },   \
                        { MOD, XK_q,     ACTION##stack, {.i = 0 } },           \
                        { MOD, XK_a,     ACTION##stack, {.i = 1 } },           \
                        { MOD, XK_z,     ACTION##stack, {.i = 2 } },           \
                        { MOD, XK_x,     ACTION##stack, {.i = -1 } }, */

#define GTKCMD(cmd)                                                            \
  {                                                                            \
    .v = (const char *[]) { "/usr/bin/gtk-launch", cmd, NULL }                 \
  }

/* ---------------------------- commands ------------------------------------- */
static char dmenumon[2] = "0";
static const char *dmenucmd[] = {"dmenu_run", "-m",  dmenumon,       "-fn",
                                 dmenufont,   "-nb", normbgcolor,    "-nf",
                                 normfgcolor, "-sb", selbordercolor, "-sf",
                                 selfgcolor,  NULL};

static const char *termcmd[] = {"kitty", NULL}; // main terminal

// spawn a terminal when a tag is middle-clicked
static const Arg tagexec[] = {
    {.v = termcmd}, /* 1 */
    {.v = termcmd}, /* 2 */
    {.v = termcmd}, /* 3 */
    {.v = termcmd}, /* 4 */
    {.v = termcmd}, /* 5 */
    {.v = termcmd}, /* 6 */
    {.v = termcmd}, /* 7 */
    {.v = termcmd}, /* 8 */
    {.v = termcmd}, /* 9 */
};

/* ---------------------------- keybindings ----------------------------------- */
static const Key keys[] = {
    {MODKEY,                             XK_p,            spawn,            {.v = dmenucmd}},    // Open Dmenu (like a terminal to start apps)
    {MODKEY,                             XK_Return,       spawn,            {.v = termcmd}},     // Spawn Terminal (this config is kitty)
    {MODKEY | ControlMask,               XK_b,            togglebar,        {0}},                // Toggle Top Bar
    STACKKEYS(MODKEY, focus) STACKKEYS(MODKEY | ShiftMask, push)
    {MODKEY | ShiftMask,                 XK_i,            incnmaster,       {.i = +1}},          // Increrase master windows
    {MODKEY | ControlMask,               XK_i,            incnmaster,       {.i = -1}},          // Decrerase Master windows
    {MODKEY,                             XK_Tab,          view,             {0}},                // Swap between 2 last visited tabs
    {MODKEY,                             XK_h,            movecorner,       {.f = -0.05}},       // maximalist: prev corner / tiling: decrerase master area
    {MODKEY,                             XK_l,            movecorner,       {.f = +0.05}},       // maximalist: next corner / tiling: increrase master area
    {MODKEY | ShiftMask,                 XK_m,            resizemaximalist, {.f = +1}},          // maximalist: grow focused window by maximalistresizestep
    {MODKEY | ControlMask,               XK_m,            resizemaximalist, {.f = -1}},          // maximalist: shrink focused window by maximalistresizestep
    {MODKEY | ControlMask,               XK_Right,        resizeedge,       {.i = +2}},          // ctrl: expand right edge outward
    {MODKEY | ControlMask,               XK_Left,         resizeedge,       {.i = -2}},          // ctrl: shrink right edge back in
    {MODKEY | ControlMask,               XK_Down,         resizeedge,       {.i = +4}},          // ctrl: expand bottom edge outward
    {MODKEY | ControlMask,               XK_Up,           resizeedge,       {.i = -4}},          // ctrl: shrink bottom edge back in
    {MODKEY | ShiftMask,                 XK_Left,         resizeedge,       {.i = +1}},          // shift: expand left edge outward
    {MODKEY | ShiftMask,                 XK_Right,        resizeedge,       {.i = -1}},          // shift: shrink left edge back in
    {MODKEY | ShiftMask,                 XK_Up,           resizeedge,       {.i = +3}},          // shift: expand top edge outward
    {MODKEY | ShiftMask,                 XK_Down,         resizeedge,       {.i = -3}},          // shift: shrink top edge back in
    {MODKEY | ControlMask | ShiftMask,   XK_f,            togglefocusonhover, {0}},              // Toggle focus-follows-hover on/off (persists across reboots)
    {MODKEY,                             XK_r,            keyshade, {0}},                        // TOGGLE SHADE (rolls the notch up/down, same as double-clicking it)
    {MODKEY,                             XK_0,            view,             {.ui = ~0}},         // View All Tags
    {MODKEY | ShiftMask,                 XK_0,            tag,              {.ui = ~0}},         // View All Tags
    {MODKEY | ControlMask | ShiftMask,   XK_q,            quit,             {1}},                // Refresh Dwm
    {MODKEY | ShiftMask,                 XK_BackSpace,    quit,             {0}},                // Quit Dwm
    {MODKEY,                             XK_q,            killclient,       {0}},                // Kill focused window
    {MODKEY | ShiftMask,                 XK_q,            killclient,       {.ui = 1}},          // Kill unfocused windows
    {MODKEY | ControlMask,               XK_x,            xrdb,             {.v = NULL}},        // Refresh Xrdb Colours, DO NOT CHANGE
    {MODKEY,                             XK_f,            togglefullscreen, {0}},                // Toggle Fulscreen
    {MODKEY | ControlMask | ShiftMask,   XK_m,            togglemaximalist, {0}},                // Toggle Maximalist Mode (WindowMaker Mode)
//  {MODKEY,                             XK_k,            setlayout,        {.v = &layouts[1]}}, // Tile Layout
//  {MODKEY | ShiftMask | ControlMask,   XK_k,            setlayout,        {.v = &layouts[2]}}, // Monocle Layout
//  {MODKEY | ShiftMask,                 XK_k,            setlayout,        {.v = &layouts[3]}}, // Spiral Layout
//  {MODKEY | ControlMask,               XK_k,            setlayout,        {.v = &layouts[4]}}, // Dwindle Layout
//  {MODKEY | ControlMask,               XK_space,        setlayout,        {0}},                // Default Layout
    {MODKEY | ShiftMask,                 XK_space,        togglefloating,   {0}},                // Unfloat a window
    {MODKEY | ControlMask,               XK_space,        focusmaster,      {0}},                // Focus on master window
    {MODKEY | ControlMask | ShiftMask,   XK_l,            togglesticky,     {0}},                // Make window follow trough tabs
    // Multi monitor control
    {MODKEY,                             XK_bracketright, focusmon,         {.i = -1}},          // idk
    {MODKEY | ShiftMask,                 XK_bracketright, tagmon,           {.i = -1}},          // idk
    {MODKEY,                             XK_bracketleft,  focusmon,         {.i = +1}},          // idk
    {MODKEY | ShiftMask,                 XK_bracketleft,  tagmon,           {.i = +1}},          // idk
    // Vanity gaps control
    {MODKEY,                             XK_g,            incrgaps,         {.i = -3}},          // Decrerase size of gaps
    {MODKEY | ShiftMask,                 XK_g,            incrgaps,         {.i = +3}},          // Increrase size of gaps
    // Tag Keys
    TAGKEYS(XK_1, 0) TAGKEYS(XK_2, 1) TAGKEYS(XK_3, 2) TAGKEYS(XK_4, 3)
        TAGKEYS(XK_5, 4) TAGKEYS(XK_6, 5) TAGKEYS(XK_7, 6) TAGKEYS(XK_8, 7)
            TAGKEYS(XK_9, 8)
    // Toggle dwmblocks
    {MODKEY | ControlMask,  XK_t,     togglebarstatus, {0}},
    // Specific app keybinds
    {MODKEY,                           XK_m,     spawn, {.v = (const char *[]) {"kitty", "-e", "opencode", NULL}}},
    {MODKEY,                           XK_b,     spawn, {.v = (const char *[]) {"helium-browser", NULL}}}, // Browser
    {MODKEY,                           XK_d,     spawn, {.v = (const char *[]) {"kitty", "-e", "concord", NULL}}}, // DISCORD (concord)
    {MODKEY | ShiftMask,               XK_d,     spawn, {.v = (const char *[]) {"vesktop", NULL}}}, // Vesktop
    {MODKEY,                           XK_e,     spawn, SHCMD("emacsclient -c -F '((name . \"Emacs Client\") (title . \"Emacs Client\") (width . 110) (height . 53))'") }, // Bigger Floating Emacs Client
    {MODKEY | ControlMask,             XK_e,     spawn, SHCMD("emacsclient --eval '(emacs-everywhere)'")}, // Emacs Everywhere
    {MODKEY | ShiftMask,               XK_b,     spawn, {.v = (const char *[]) {"kitty", "-e", "ptui", NULL}}}, // Kitty Btop Terminal
    {MODKEY | ShiftMask,               XK_f,     spawn, {.v = (const char *[]) {"nautilus", NULL}}}, // File Browser
    {MODKEY | ShiftMask,               XK_r,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/screenrecord", "toggle", NULL}}}, // Screen Recording Script
    {MODKEY | ShiftMask,               XK_w,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/loadouts", NULL}}}, // Custom Wallpaper Picker Script
    {MODKEY | ControlMask | ShiftMask, XK_w,     spawn, {.v = (const char *[]) {"onlyoffice-desktopeditors", NULL}}}, // OnlyOffice 
    {MODKEY,                           XK_t,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/system/toggle-kitty-opacity", NULL}}}, // System Transparency Toggle
    {MODKEY | ShiftMask,               XK_t,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/system/trackpad-toggle", NULL}}}, // Trackpad Toggle
    {MODKEY,                           XK_c,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/cam.sh", "--view", NULL}}}, // Open Camera
    {MODKEY,                           XK_Shift_R, spawn, {.v = (const char *[]){HOME "/.config/scripts/system/powermenu", NULL}}}, // Power Menu Options
    {MODKEY,                           XK_BackSpace, spawn, {.v = (const char *[]){"slock", NULL}}}, // Lockscreen

    // Special Function keys
    {0, XF86XK_AudioMute,             spawn, SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle; pkill -RTMIN+10 dwmblocks")}, // Mute Audio
    {0, XF86XK_AudioLowerVolume,      spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ -5%; pkill -RTMIN+10 dwmblocks")}, // Decrerase Volume
    {0, XF86XK_AudioRaiseVolume,      spawn, SHCMD("pactl set-sink-volume @DEFAULT_SINK@ +5%; pkill -RTMIN+10 dwmblocks")}, // Increrase Volume
    {0, XF86XK_AudioMicMute,          spawn, SHCMD("pactl set-source-mute @DEFAULT_SOURCE@ toggle; pkill -RTMIN+10 dwmblocks")}, // Mute Microphone
    {0, XF86XK_Mail,                  spawn, SHCMD("helium-browser https://mail.google.com") }, // Open Browser Gmail
    // Backup Keybinds for special function keys
    {MODKEY, XK_F9,             spawn, SHCMD("helium-browser https://mail.google.com") }, // Open Browser Gmail
    {MODKEY, XK_F2,             spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/screenshot-minimal.sh", NULL}}}, // Screenshot Shortcut

    // Laptop Brightness Utility
    {0, XF86XK_MonBrightnessUp,       spawn, SHCMD("brightnessctl set +1%")}, // Increrase Brightness
    {0, XF86XK_MonBrightnessDown,     spawn, SHCMD("brightnessctl set 1%-")}, // Decrerase Brightness
    // Screenshot Binds
    {MODKEY | ShiftMask,               XK_s,  spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/screenshot-minimal.sh", NULL}}}, // Full Screenshot
    {MODKEY | ControlMask,             XK_s,  spawn, {.v = (const char *[]) {HOME "/.config/scripts/images-photos-wallpapers/screenshot.sh", NULL}}}, // Sellection Screenshot
    // Input - Output binds
    {MODKEY,                XK_o,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/audio-switch", "output", NULL}}},  //AUDIO OUTPUT, swap speaker
    {MODKEY,                XK_i,     spawn, {.v = (const char *[]) {HOME "/.config/scripts/audio-video/audio-switch", "input", NULL}}},  //AUDIO INPUT, swap mic
};

/* ---------------------------- mouse buttons ---------------------------------
 * click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle,
 * ClkClientWin, or ClkRootWin
 * ----------------------------------------------------------------------------*/
static const Button buttons[] = {
/* click                event mask           button          function argument */
/* Location        Modifier   Button     Function   Argument */
#ifndef __OpenBSD__
    {ClkWinTitle,  0,          Button2,        zoom,         {0}},
    {ClkStatusText,0,          Button1,        sigstatusbar, {.i = 1}},
    {ClkStatusText,0,          Button2,        sigstatusbar, {.i = 2}},
    {ClkStatusText,0,          Button3,        sigstatusbar, {.i = 3}},
    {ClkStatusText,0,          Button4,        sigstatusbar, {.i = 4}},
    {ClkStatusText,0,          Button5,        sigstatusbar, {.i = 5}},
    {ClkStatusText,ShiftMask,  Button1,        sigstatusbar, {.i = 6}},
#endif
    {ClkStatusText,ShiftMask,  Button3,        spawn,
     SHCMD("st -e nvim ~/.local/src/dwmblocks/blocks.h")},
    {ClkClientWin, MODKEY,     Button1,        movemouse,    {0}},      /* left */
    {ClkClientWin, MODKEY,     Button2,        defaultgaps,  {0}},      /* middle */
    {ClkClientWin, MODKEY,     Button3,        resizemouse,  {0}},      /* right */
    {ClkTagBar,    0,          Button1,        view,         {0}},
    {ClkTagBar,    0,          Button3,        toggleview,   {0}},
    {ClkTagBar,    MODKEY,     Button1,        tag,          {0}},
    {ClkTagBar,    MODKEY,     Button3,        toggletag,    {0}},
    {ClkRootWin,   0,          Button2,        togglebar,    {0}}, /* hide bar */
};

/* ============================================================================
 * The three blocks below run at startup / mode-toggle time rather than being
 * referenced constantly while editing keybinds/appearance, so they live at
 * the bottom out of the way.
 * ==========================================================================*/

/* ---------------------------- autostart ------------------------------------- */
static const char *const autostart[] = {
	/* wallpaper restore */
	"/bin/sh", "-c", "wal -R && feh --bg-fill \"$(<" HOME "/.cache/wal/wal)\"", NULL,
  /* disable touchpad DWT for gaming */
	"/bin/sh", "-c", "xinput set-prop \"$(xinput list --id-only 'ASCP1200:00 093A:3016 Touchpad')\" 'libinput Disable While Typing Enabled' 0", NULL,
	/* compositor */
	"picom", "--config", HOME "/.config/picom/picom.conf", NULL,
	/* statusbar */
	"/bin/sh", "-c", "exec dwmblocks", NULL,
	/* theme restore */
	"/bin/sh", "-c", "exec " HOME "/.config/scripts/theme-restore", NULL,
	/* numlock */
	"/bin/sh", "-c", "numlockx on", NULL,
	/* dark mode */
	"/bin/sh", "-c", "gsettings set org.gnome.desktop.interface color-scheme 'prefer-dark'", NULL,
	/* pre-clear dwmblocks signal */
	"pkill", "-RTMIN+15", "dwmblocks", NULL,
	/* mpc player-change watcher */
	"/bin/sh", "-c", "exec sh -c 'while mpc idle player; do pkill -RTMIN+11 dwmblocks; done'", NULL,
	/* pactl sink-change watcher */
	"/bin/sh", "-c",
		"exec sh -c \"pactl subscribe | grep --line-buffered \\\"Event 'change' on sink\\\" | while read -r _; do pkill -RTMIN+10 dwmblocks; done\"",
		NULL,
	/* portal */
	"/bin/sh", "-c", "exec /usr/lib/xdg-desktop-portal-gtk", NULL,
	/* mic listener */
	"/bin/sh", "-c", "exec " HOME "/.config/scripts/statusbar/mic-listener", NULL,
  /* battery percentage watcher */
	"/bin/sh", "-c", "exec " HOME "/.config/scripts/system/batery-watchdog", NULL,
  /* emacs daemon */
  "/bin/sh", "-c", "exec emacs --daemon", NULL,
  /* numlock */
	"/bin/sh", "-c", "xset r rate 150 50", NULL,
  /* wmmp dockapp: bridge daemon in bg, WMmp itself is spawned by wm-dock.sh */
	"/bin/sh", "-c", "setsid " HOME "/.config/scripts/wmmp-qmmp-bridge.py 6601 >/dev/null 2>&1 &", NULL,
	NULL /* terminate autostart apps when dwm closes */
};

/* ---------------------------- maximalist mode toggle command ---------------- */
// Command run for as long as Maximalist Mode is on; started on toggle-on.
static const char *const maximalistcmd[] = {
	"/bin/sh", "-c", "exec " HOME "/.config/scripts/wm-dock.sh", NULL,
};

/* ---------------------------- window rules -----------------------------------
 * Add .nomaximalist = 1 to any rule below (designated-initializer style, like
 * the wmgtemp/DockApp/etc rows) to fully exclude that app from Maximalist
 * Mode: no forced floating, no border change, no notch, ever.
 * Add .nonotch = 1 instead if you still want the floating/border treatment
 * but never a notch on that app specifically.
 * ------------------------------------------------------------------------------*/
static const Rule rules[] = {
    { .class = "Qmmp", .isfloating = 1, .monitor = -1, .nomaximalist = 1 },
    { .title = "Qmmp", .isfloating = 1, .monitor = -1, .nomaximalist = 1 },
    { .class = "Qmmp",       .title = "Qmmp",      .isfloating = 1, .monitor = -1, .nomaximalist = 1 },
    { .class = "Qmmp",       .title = "Playlist",  .isfloating = 1, .monitor = -1, .nomaximalist = 1 },
    { .class = "Qmmp",       .title = "Equalizer", .isfloating = 1, .monitor = -1, .nomaximalist = 1 },
    {NULL, NULL, "emacs-everywhere", 0, 1, 0, 1, -1},
    {NULL, NULL, "CamPreview", 0, 1, -1},
    {"NULL", NULL, "Emacs Client", 0, 1, -1},
    {NULL, NULL, "Powermenu", 0, 1, -1},
    { .class = "DockApp", .isfloating = 1, .monitor = -1, .nomaximalist = 1, .staticlabel = "DOCK", .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    /* wmnetload's real WM_CLASS is "Wmnetload" (capitalized), not "wmnetload" — matched by instance below so the tile wrap actually fires */
    { .class = "Wmnetload", .instance = "wmnetload", .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
        /* wminfo */
    { .class = "wminfo",      .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wminfo",      .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmail */
    { .class = "wmail",       .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmail",       .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmbatteries */
    { .class = "wmbatteries", .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmbatteries", .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* WMmp */
    { .class = "WMmp",        .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "WMmp",        .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmmemload */
    { .class = "wmmemload",   .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmmemload",   .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmnetload */
    { .class = "wmnetload",   .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmnetload",   .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmcpuload */
    { .class = "wmcpuload",   .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmcpuload",   .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmclockmon */
    { .class = "wmclockmon",  .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmclockmon",  .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmclock */ 
    { .class = "WMClock", .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmclock", .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmtz */
    { .class = "wmtz",        .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmtz",        .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },

    /* wmweather+ */
    { .class = "wmweather+",  .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1, .isdockapp = 1 },
    { .title = "wmweather+",  .isfloating = 1, .monitor = -1, .nomaximalist = 1, .nokill = 1, .alwayssticky = 1, .alwaysbelow = 1, .nofocus = 1, .nofullscreen = 1 },
};
