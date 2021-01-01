# About

This is a simple tool to change screen resolution for Windows application.
It changes screen resolution before starting application and restores original resolution after application exits.
The tool can be useful for some weird applications, requiring particular screen resolution to work properly, but not setting it themselves.

# Building

```
git clone https://github.com/UltraCoderRU/screen-resolution-changer.git
cd screen-resolution-changer
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

# Usage

```
screen-resolution-changer.exe <width> <height> <executable> [args...]
```

Normally you create a shell link (.lnk) with that command to start application with specific screen resolution.
