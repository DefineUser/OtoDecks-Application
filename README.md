# OtoDecks Application

OtoDecks is an audio player application that provides an interactive GUI for playing, manipulating, and visualizing audio tracks. The application leverages JUCE library components to offer features such as waveform display, turntable interaction, and playlist management.

## Features

- **DeckGUI**: Interface for controlling playback, volume, and speed of audio tracks.
- **DJAudioPlayer**: Manages audio playback, including loading tracks, setting gain, and adjusting speed.
- **TurntableComponent**: Simulates a turntable interface for interactive audio playback control.
- **WaveformDisplay**: Visual representation of audio waveforms, displaying the current playback position.
- **PlaylistComponent**: Manages a playlist of audio tracks, allowing for loading and searching of tracks.

## Installation

### Prerequisites

- JUCE library
- C++ compiler (g++, clang++, MSVC)
- CMake/Projucer tool provided by JUCE (optional, for build configuration)

### Steps

1. **Clone the Repository**:

    ```sh
    git clone https://github.com/DefineUser/otodecks.git
    cd otodecks
    ```

2. **Download JUCE**:

    Download the JUCE library from [JUCE website](https://juce.com/download) and extract it to a suitable location.

3. **Set JUCE Path**:

    Set the `JUCE_HOME` environment variable to point to the directory where you extracted JUCE.

4. **Build the Project**:

    If you're using a CMake build system, navigate to the project directory and run:

    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```

    Alternatively, you can use the Projucer tool provided by JUCE to generate project files for your preferred IDE (Visual Studio, Xcode, etc.).

## Usage

1. **Run the Application**:

    After building the project, run the executable:

    ```sh
    ./OtoDecksApplication
    ```

2. **Using the DeckGUI**:

    - **Play**: Click the "PLAY" button to start playback.
    - **Stop**: Click the "STOP" button to stop playback.
    - **Load**: Click the "LOAD" button to select an audio file to load into the player.
    - **Volume**: Adjust the volume slider to set the playback volume.
    - **Speed**: Adjust the speed slider to change the playback speed.

3. **Using the TurntableComponent**:

    - Drag the turntable to simulate DJ scratching and control playback position.

4. **Using the PlaylistComponent**:

    - **Load Tracks**: Click the "Load Tracks from File" button to load audio files from a directory.
    - **Search Tracks**: Use the search box to find tracks in the playlist.
    - **Load to Decks**: Click the "Load 1" or "Load 2" buttons next to a track to load it into Deck 1 or Deck 2, respectively.

## Project Structure

- `src/`: Contains the source files for the project.
  - `DeckGUI.cpp` / `DeckGUI.h`: Defines the DeckGUI component.
  - `DJAudioPlayer.cpp` / `DJAudioPlayer.h`: Defines the DJAudioPlayer class.
  - `TurntableComponent.cpp` / `TurntableComponent.h`: Defines the TurntableComponent class.
  - `WaveformDisplay.cpp` / `WaveformDisplay.h`: Defines the WaveformDisplay component.
  - `MainComponent.cpp` / `MainComponent.h`: Defines the main application component.
  - `PlaylistComponent.cpp` / `PlaylistComponent.h`: Defines the PlaylistComponent class.

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
