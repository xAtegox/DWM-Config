#!/usr/bin/env bash
# Based on original script by https://github.com/TheGassyNinja
set -euo pipefail
export DISPLAY="${DISPLAY:-:0}"

# Output directory: default to $HOME/Screenshots
OUTDIR="${1:-${HOME}/Screenshots}"
mkdir -p "${OUTDIR}"

current="$(date +%Y%m%d_%H%M%S).png"
filepath="${OUTDIR}/${current}"

# Determine screenshot utility
tool_path="$(command -v import 2>/dev/null || command -v scrot 2>/dev/null)" || {
  notify-send "Screenshot Tool Missing" "Neither 'import' nor 'scrot' is installed!"
  echo "Error: No screenshot tool found."
  exit 1
}
TOOL="$(basename "${tool_path}")"

# Select flags based on tool and optional "-s" argument
flags=()
[[ "${TOOL}" == "scrot" && "${2:-}" == "-s" ]] && flags=(--select)
[[ "${TOOL}" == "import" && "${2:-}" != "-s" ]] && flags=(-window root)

# Take the screenshot
"$TOOL" -silent "${flags[@]}" "${filepath}" || {
  notify-send "Screenshot Failed" "Could not save screenshot to ${filepath}."
  exit 1
}

# Copy to clipboard using xclip
if command -v xclip >/dev/null 2>&1; then
  xclip -selection clipboard -t image/png -i "${filepath}"
  notify-send "Screenshot Taken" "Saved as ${current} and copied to clipboard."
else
  notify-send "Screenshot Taken" "Saved as ${current} but xclip is not installed."
fi
