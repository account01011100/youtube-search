# YouTube Search

**youtube-search** is a command-line application that allows users to search for YouTube videos, display the results in a list, and play the selected video using **mpv**. This tool uses the YouTube's search API to fetch video data and presents it in a terminal-based user interface.

## Usage
  youtube-search [QUERY]
  youtube-search -h, --help

  INTERACTIVE CONTROLS:
    w, k                  Move up in results
    s, j                  Move down in results
    Enter, p              Play selected video
    r                     New search
    q                     Quit

  EXAMPLES:
    youtube-search "music video"
    youtube-search

## Dependencies

- `gcc` c compiler
- `libcurl` library for making HTTP requests
- `cJSON` library for JSON parsing
- `mpv` installed on your system for video playback
- `UTF-8` compatible terminal emulator

## Compiling

  **Clone this repository and install dependencies**:
   ```bash
   git clone https://github.com/account01011100/youtube-search.git
   cd youtube-search
   make
