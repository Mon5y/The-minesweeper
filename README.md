# 💣 The Minesweeper

A custom Minesweeper implementation built with C++17 and SFML, featuring clean architecture, unique game modes, and procedural generation elements.

---

## 🚀 Implemented Features (Current Build)

*   **🧩 Clean Architecture:** Codebase is fully refactored and separated into independent modules (`Cell`, `GameBoard`, and `Board` rendering).
*   **👁️ Blind Mode:** A hardcore custom game mode that relies on hidden mechanics and memory retention.
*   **⚙️ UI & Scaling:** Fully functional main menu and interactive settings with dynamic grid scaling and mine balancing for Easy, Normal, and Hard presets.

---

## 🛠️ In Development & Testing (Upcoming Features)

*   **🌍 Procedural Biomes:** An algorithm to generate distinct visual zones (such as forest, desert, and winter landscapes) across the grid, ensuring every playthrough is visually unique.
*   **🎨 Sprite & Texture Atlas:** Integration of a custom 16x16 pixel-art sprite sheet to replace standard flat color cell rendering.
*   **⚙️ First-Click Safety:** Overhauling the mine placement algorithm to guarantee that the player's very first move never lands on a mine.

---

## 💻 How to Build

The project is compiled via **CMake** using the **MinGW** toolchain:

```bash
mkdir build
cd build
cmake -G "MinGW Makefiles" ..
cmake --build . --config Debug
