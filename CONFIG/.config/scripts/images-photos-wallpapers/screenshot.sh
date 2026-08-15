#!/usr/bin/env bash
set -euo pipefail
export DISPLAY="${DISPLAY:-:0}"

# Dependencies check
for cmd in xclip maim; do command -v "$cmd" &>/dev/null || {
  echo "Missing: $cmd" >&2
  exit 1
}; done

OUTDIR="${2:-$HOME/Screenshots}"
mkdir -p "$OUTDIR"

# Get window title for naming
get_window_title() {
  command -v xdotool &>/dev/null || {
    echo "screenshot"
    return
  }
  local title=$(xdotool getactivewindow getwindowname 2>/dev/null || echo "screenshot")
  # Sanitize title: remove special chars, limit length, convert spaces to underscores
  echo "$title" | sed 's/[^a-zA-Z0-9._-]/_/g' | sed 's/__*/_/g' | cut -c1-30
}

WINDOW_TITLE=$(get_window_title)
TIMESTAMP=$(date +%Y%m%d_%H%M%S)
FILE="$OUTDIR/${WINDOW_TITLE}_${TIMESTAMP}.png"
TMP=$(mktemp --suffix=.png)
trap 'rm -f "$TMP"' EXIT

show_help() {
  cat <<EOF
Usage: $0 [color|full|window] [output_dir]
Environment options:
  WATERMARK=1
  WATERMARK_TEXT='text'
  WATERMARK_POS=position
  WATERMARK_SIZE=px
  WATERMARK_BG='#rrggbbaa'
EOF
}

post() {
  [[ "${WATERMARK:-}" == "1" ]] && {
    command -v magick &>/dev/null || {
      echo "Missing: magick" >&2
      exit 1
    }
    # Watermark subsystem (env overrides supported)
    magick "$FILE" \
      -gravity "${WATERMARK_POS:-southeast}" \
      -pointsize "${WATERMARK_SIZE:-28}" \
      -fill white -undercolor "${WATERMARK_BG:-#00000080}" \
      -annotate +20+20 "${WATERMARK_TEXT:-$(date '+%Y-%m-%d %H:%M')}" \
      "$FILE"
  }
  # Copy to clipboard - use nohup to keep xclip alive
  nohup xclip -selection clipboard -t image/png -i "$FILE" >/dev/null 2>&1 &
  disown

  notify-send "Selective screenshot taken" "Saved as $(basename "$FILE") and copied to clipboard."

  # Give clipboard-monitor time to catch it
  sleep 0.2
}

colorpicker() {
  command -v magick &>/dev/null || {
    command -v notify-send &>/dev/null && notify-send "Missing: magick" 2>/dev/null || echo "Missing: magick"
    exit 1
  }
  maim -s "$TMP" || exit 1
  hex=$(magick "$TMP" -scale 1x1\! -format "#%[hex:p{0,0}]" info: 2>/dev/null || echo "#??????")
  echo "$hex" | xclip -selection clipboard -i && command -v notify-send &>/dev/null && notify-send -t 30000 -i "$TMP" "Color: $hex" 2>/dev/null || true
}

capture_full() {
  maim "$FILE" && post
}

capture_window() {
  command -v xdotool &>/dev/null || {
    echo "Missing: xdotool" >&2
    exit 1
  }
  maim -i "$(xdotool getwindowfocus -f)" "$FILE" && post
}

capture_selection() {
  maim -s "$FILE" && post
}

main() {
  case "${1:-}" in
  -h | --help) show_help ;;
  color*) colorpicker ;;
  full) capture_full ;;
  window) capture_window ;;
  *) capture_selection ;;
  esac
}

main "$@"
