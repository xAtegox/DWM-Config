#!/usr/bin/env bash

DEVICE="/dev/video0"
PID_FILE="/tmp/cam_rec.pid"
LOG_FILE="/tmp/cam_rec.log"

# --- Crop configuration ---
# Based on 1920x1080 input. Tune these to frame your face.
# Formula: x = (1920 - CROP_W) / 2  to keep it centered horizontally.
# Increase CROP_Y to cut more ceiling, decrease to show more above your head.
CROP_W=1920 # width  — square crop, removes sides
CROP_H=1080 # height — same as width = square
CROP_X=610  # x offset — (1920 - 700) / 2 = 610 (centered, cuts door)
CROP_Y=100  # y offset — shifts down to catch face + chin, cuts excess ceiling

# --- Border configuration ---
BORDER=10           # thickness of the warm beige border in pixels
BORDER_COLOR=E8D5B7 # warm beige — change to taste (no # prefix for mpv)

# --- Arg parsing ---
# Supports: --view, --toggle-rec
MODE=""
FLIP=true # always mirrored

for arg in "$@"; do
  case "$arg" in
  --view) MODE="view" ;;
  --toggle-rec) MODE="toggle-rec" ;;
  esac
done

# --- Helpers ---

check_device() {
  if [[ ! -e "$DEVICE" ]]; then
    notify-send "Camera" "No camera found at $DEVICE" -i camera
    exit 1
  fi
}

# Explicitly enforce auto white balance, continuous autofocus, and
# backlight compensation via v4l2-ctl so they are guaranteed on.
init_camera() {
  v4l2-ctl -d "$DEVICE" \
    --set-ctrl=white_balance_automatic=1 \
    --set-ctrl=focus_automatic_continuous=1 \
    --set-ctrl=backlight_compensation=1 \
    2>/dev/null
}

# Build the filter chain for mpv (vf) and ffmpeg (-vf).
# Order: crop → pad (border) → hflip
build_vf() {
  local padded_w=$((CROP_W + BORDER * 2))
  local padded_h=$((CROP_H + BORDER * 2))
  local vf="crop=${CROP_W}:${CROP_H}:${CROP_X}:${CROP_Y}"
  vf="${vf},pad=${padded_w}:${padded_h}:${BORDER}:${BORDER}:color=#${BORDER_COLOR}"
  $FLIP && vf="${vf},hflip"
  echo "$vf"
}

# --- Modes ---

case "$MODE" in
view)
  check_device
  init_camera

  VF=$(build_vf)

  # Native H264 from camera: 1920x1080 @ 30fps, no CPU re-decode penalty.
  # --no-cache + --untimed = true zero-latency preview.
  mpv /dev/video0 \
    --demuxer-lavf-o=input_format=mjpeg,video_size=1920x1080,framerate=30 \
    --profile=low-latency \
    --untimed \
    --no-cache \
    --ontop \
    --no-border \
    --focus-on=never \
    --input-default-bindings=no \
    --input-vo-keyboard=no \
    --geometry=405x230-30-40 \
    --title="CamPreview" \
    --vf="$VF"
  ;;

toggle-rec)
  check_device

  if [[ -f "$PID_FILE" ]]; then
    # STOP — SIGINT lets ffmpeg flush and finalize the container properly
    PID=$(cat "$PID_FILE")
    FILENAME=$(cat "${PID_FILE}.file" 2>/dev/null)

    if kill -INT "$PID" 2>/dev/null; then
      wait "$PID" 2>/dev/null
    fi

    rm -f "$PID_FILE" "${PID_FILE}.file"

    # Delete the recording file
    [[ -n "$FILENAME" && -f "$FILENAME" ]] && rm -f "$FILENAME"

    notify-send "Camera" "Recording stopped and cleaned up." -i camera-video
  else
    init_camera

    FILENAME="/tmp/cam_rec_$(date +%Y%m%d_%H%M%S).mp4"
    VF=$(build_vf)

    # Crop (and optional flip) always require re-encoding since we are
    # applying a filter — stream copy is not possible with -vf.
    ffmpeg \
      -f v4l2 \
      -input_format mjpeg \
      -video_size 1920x1080 \
      -framerate 30 \
      -fflags nobuffer \
      -flags low_delay \
      -i "$DEVICE" \
      -vf "$VF" \
      -c:v libx264 -preset ultrafast -tune zerolatency \
      "$FILENAME" \
      >"$LOG_FILE" 2>&1 &

    FFMPEG_PID=$!

    # Brief sanity check — if ffmpeg dies immediately something is wrong
    sleep 1
    if ! kill -0 "$FFMPEG_PID" 2>/dev/null; then
      notify-send "Camera" "Failed to start recording. Check $LOG_FILE" -i camera
      exit 1
    fi

    echo "$FFMPEG_PID" >"$PID_FILE"
    echo "$FILENAME" >"${PID_FILE}.file"

    notify-send "Camera" "Recording started at ${CROP_W}x${CROP_H}@30fps" -i camera-video
  fi
  ;;

*)
  echo "Usage: cam.sh [--view | --toggle-rec]"
  exit 1
  ;;
esac
