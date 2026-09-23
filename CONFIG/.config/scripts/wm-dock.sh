#!/bin/dash

export DISPLAY=:0
export XAUTHORITY="$HOME/.Xauthority"

POSFILE="$HOME/.config/wm-dock/positions.conf"
MONS=$(mktemp)

mkdir -p "$(dirname "$POSFILE")"

# ==================================================
# APPS
#
# NAME         INSTANCE       CLASS       COMMAND
# ==================================================

APPS='
wmbatteries  wmbatteries    DockApp      wmbatteries
wmnetload    wmnetload      Wmnetload    wmnetload -w -i auto -u 1
wmmemload    wmmemload      DockApp      wmmemload
wmcpuload    wmcpuload      DockApp      wmcpuload
wmclockmon   wmclockmon     DockApp      wmclockmon
WMmp         WMmp           DockApp      MPD_HOST=127.0.0.1 MPD_PORT=6601 WMmp
'

# ==================================================
# Monitor layout
#
# One dockapp instance is spawned per connected
# monitor. positions.conf holds absolute coordinates
# as saved on the reference monitor (the primary);
# each other monitor gets a copy at the same relative
# spot, scaled to that monitor's own resolution,
# always 64x64.
# ==================================================

xrandr 2>/dev/null | awk '
  $2 == "connected" {
    name = $1
    isprimary = 0
    for (i = 1; i <= NF; i++) {
      if ($i == "primary") isprimary = 1
      if ($i ~ /^[0-9]+x[0-9]+\+[0-9]+\+[0-9]+$/) {
        split($i, a, /[x+]/)
        printf "%s|%s|%d|%d|%d|%d\n", (isprimary ? "P" : "-"), name, a[1], a[2], a[3], a[4]
        break
      }
    }
  }' > "$MONS"

REF=$(grep -m1 '^P|' "$MONS")
[ -z "$REF" ] && REF=$(grep -m1 '^-|' "$MONS")

if [ -z "$REF" ]; then
  echo "ERROR: no connected monitors found via xrandr"
  rm -f "$MONS"
  exit 1
fi

RW=$(echo "$REF" | cut -d'|' -f3)  # reference monitor width
RH=$(echo "$REF" | cut -d'|' -f4)  # reference monitor height
ROX=$(echo "$REF" | cut -d'|' -f5) # reference monitor origin x
ROY=$(echo "$REF" | cut -d'|' -f6) # reference monitor origin y

# ==================================================
# Find window
# ==================================================

find_window() {
  INSTANCE="$1"
  CLASS="$2"
  SEENFILE="$3"

  i=0
  while [ "$i" -lt 30 ]; do
    ID=$(wmctrl -lx | awk -v wanted="$INSTANCE.$CLASS" \
      '$3 == wanted {print $1; exit}')
    if [ -n "$ID" ] && ! grep -q "^$ID$" "$SEENFILE" 2>/dev/null; then
      printf '%s\n' "$ID"
      return 0
    fi
    sleep 0.1
    i=$((i + 1))
  done
  return 1
}

# ==================================================
# Get saved position (absolute, as stored on the
# reference monitor)
# ==================================================

get_position() {
  NAME="$1"

  grep "^$NAME|" "$POSFILE" 2>/dev/null |
    head -n1 |
    cut -d'|' -f2-3 |
    tr '|' ' '
}

# ==================================================
# Scale a saved coordinate onto a target monitor
# ==================================================

scale_coord() {
  SAVED="$1"
  REFORIGIN="$2"
  REFSIZE="$3"
  TGTORIGIN="$4"
  TGTSIZE="$5"

  awk -v s="$SAVED" -v ro="$REFORIGIN" -v rs="$REFSIZE" \
      -v to="$TGTORIGIN" -v ts="$TGTSIZE" \
    'BEGIN { printf "%d", to + (s - ro) / rs * ts + 0.5 }'
}

# ==================================================
# Start application
# ==================================================

start_app() {
  COMMAND="$1"

  sh -c "$COMMAND" >/dev/null 2>&1 &
}

# ==================================================
# Position window
#
# IMPORTANT:
# This ONLY reads positions.
# It NEVER saves or changes positions.conf.
# ==================================================

position_window() {
  ID="$1"
  X="$2"
  Y="$3"

  wmctrl -i -r "$ID" -e "0,$X,$Y,64,64"
}

# ==================================================
# Kill existing dockapps
# ==================================================

echo "$APPS" |
  while read -r NAME INSTANCE CLASS COMMAND; do
    [ -z "$NAME" ] && continue

    pkill -x "$INSTANCE" 2>/dev/null
  done

sleep 0.3

# ==================================================
# Start dockapps: one copy on the primary monitor (screen 0)
# ==================================================

# Coordinates are saved relative to the primary monitor, so scaling
# them against the primary is the identity — dockapps land exactly
# where they were saved.
MON=$(echo "$REF" | cut -d'|' -f2)
W=$(echo "$REF" | cut -d'|' -f3)
H=$(echo "$REF" | cut -d'|' -f4)
OX=$(echo "$REF" | cut -d'|' -f5)
OY=$(echo "$REF" | cut -d'|' -f6)

echo "$APPS" |
  while read -r NAME INSTANCE CLASS COMMAND; do
    [ -z "$NAME" ] && continue

    SAVED=$(get_position "$NAME")

    if [ -z "$SAVED" ]; then
      echo "WARNING: No saved position for $NAME"
      continue
    fi

    SAVED_X=$(echo "$SAVED" | awk '{print $1}')
    SAVED_Y=$(echo "$SAVED" | awk '{print $2}')
    SEEN=$(mktemp)

    start_app "$COMMAND"

    ID=$(find_window "$INSTANCE" "$CLASS" "$SEEN")

    if [ -n "$ID" ]; then
      printf '%s\n' "$ID" >> "$SEEN"

      X=$(scale_coord "$SAVED_X" "$ROX" "$RW" "$OX" "$W")
      Y=$(scale_coord "$SAVED_Y" "$ROY" "$RH" "$OY" "$H")

      position_window "$ID" "$X" "$Y"
    else
      echo "WARNING: Could not find $NAME on $MON"
    fi

    rm -f "$SEEN"
  done

rm -f "$MONS"