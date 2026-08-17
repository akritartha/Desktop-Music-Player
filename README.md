# Melodex

Melodex is a C++ desktop music player built entirely with raylib (no Qt). It provides a sleek, custom-built object-oriented GUI for managing and playing your local audio library.

## Team & Roles
- **GUI**: Nischhal (082BCT044)
- **Audio Engine**: Nikit Bhatta (082BCT040)
- **Database/Library**: Kritartha Adhikari (082BCT034)

## Features
- **Playback**: Play, pause, skip, shuffle, repeat, and volume/seek controls.
- **Library Management**: Scan local folders and build a database of your music.
- **Playlists & Favorites**: Create custom playlists and toggle favorite songs.
- **Context Menu**: Right-click context menu on songs for quick actions.
- **Responsive UI**: Fully custom, resizable application window built from scratch.
- **Multi-Format Support**: Natively plays `.mp3`, `.ogg`, `.flac`, and `.wav` files.
- **Search**: Real-time filtering and searching of your library.

## Prerequisites
This project requires a Linux environment (due to X11/OpenGL dependencies) and the following installed packages:
- **g++**: A C++ compiler.
- **raylib**: Ensure raylib and its dependencies are installed on your system.

*Note: There are no automated build scripts (e.g., CMake) currently included; compilation relies on direct compiler invocation.*

## Build Instructions
Run the following command from the root directory of the project to compile:

```bash
g++ main.cpp melodex-gui/*.cpp Database/*.cpp -Imelodex-gui -IDatabase -o melodex -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```

## Run Instructions
After successfully compiling, execute the compiled binary:

```bash
./melodex
```

## Project Structure
- `melodex-gui/`: Contains the frontend UI layer, utilizing a custom OOP component hierarchy (menus, buttons, sliders, panels) built on raylib.
- `Database/`: Contains the backend database and audio library logic, managing track indexing, file parsing, and playlists.

## Known Limitations / Not Yet Implemented
- The **Add Lyrics** button is non-functional.
- Clicking the **album card** currently does nothing.
- The user **avatar icon** has not been built/integrated.
- No **Flathub / .deb** packaging configured for deployment yet.
