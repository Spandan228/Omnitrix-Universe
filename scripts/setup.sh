#!/bin/bash
# Setup script for Omnitrix Universe dependencies

echo "Setting up Omnitrix Universe..."

# Detect OS
OS="Unknown"
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="Linux"
elif [[ "$OSTYPE" == "cygwin" ]] || [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    OS="Windows"
fi

echo "Detected OS: $OS"

if [ "$OS" = "Linux" ]; then
    echo "Installing SFML via apt-get..."
    sudo apt-get update
    sudo apt-get install -y libsfml-dev
elif [ "$OS" = "Windows" ]; then
    echo "Please install SFML manually on Windows."
    echo "1. Download SFML for MinGW from https://www.sfml-dev.org/download.php"
    echo "2. Extract and set the paths in your compiler or use MSYS2:"
    echo "   pacman -S mingw-w64-x86_64-sfml"
fi

echo "Setup complete. You can now build the game using 'make release' or 'make all'."
