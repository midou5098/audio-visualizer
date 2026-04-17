🎧 Audio Spectrum Visualizer (SDL2 + FFTW)

A real-time audio spectrum visualizer built in C++ using SDL2, FFTW, and low-level audio processing.
This project supports both live microphone input and MP3 file playback, with frequency-domain analysis rendered dynamically.

---

🚀 Features

- 🎤 Live Audio Capture
  
  - Captures microphone input using SDL audio callbacks
  - Real-time FFT processing

- 🎵 MP3 File Visualization
  
  - Load and play ".mp3" files
  - Extract metadata and album art
  - Sync visualization with playback

- 📊 Frequency Spectrum Rendering
  
  - FFT (Fast Fourier Transform) using FFTW
  - 50 frequency bands (log-scaled)
  - Smooth interpolation between frames

- 🖼️ Simple UI System
  
  - Mode selection (Live / File)
  - Interactive file picker
  - Dynamic rendering using SDL2

---

🧠 Technical Overview

Audio Pipeline

- Live Mode
  
  - SDL audio device captures float samples
  - Callback fills a buffer of 1024 samples

- File Mode
  
  - MP3 decoded using "minimp3"
  - Playback handled via SDL_mixer
  - Manual synchronization using elapsed time

---

FFT Processing

- Windowing: Hann Window
- FFT Size: "1024"
- Output: "513 frequency bins"

Frequency bands are computed using a logarithmic scale:

- Low frequencies emphasized (human perception)
- Each bar aggregates multiple FFT bins
- Magnitude smoothed over time for visual stability

---

Rendering

- SDL2 renderer
- Vertical bars represent amplitude
- Height scaled and clamped per frame
- Album art displayed when available

---

🛠️ Dependencies

- SDL2
- SDL2_ttf
- SDL2_image
- SDL2_mixer
- FFTW3
- minimp3
- TagLib
- portable-file-dialogs

---

📦 Build & Run

g++ main.cpp -o visualizer \
-lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer \
-lfftw3 -ltag

Then run:

./visualizer

---

🎮 Controls

Key / Action| Function
Mouse Click| Select mode
ESC| Return to menu
File Picker| Load MP3

---

⚠️ Known Limitations

- No dynamic resizing
- Hardcoded FFT size (1024)
- Basic UI (no layout system)
- Limited error handling (file/audio failures)

---

🧩 Future Improvements

- Beat detection / peak tracking
- Better frequency band distribution
- GPU-accelerated rendering
- Modular architecture (separate audio / render / UI layers)
- Support for more audio formats

---

📌 Notes

This project focuses on low-level audio processing and visualization, not on UI polish.
Most logic (FFT, buffering, synchronization) is implemented manually to better understand the underlying systems.

---

🧠 Why this project?

To explore:

- Real-time signal processing
- FFT-based analysis
- Audio pipelines in C++
- SDL-based rendering systems

---

📜 License

Free to use and modify.
