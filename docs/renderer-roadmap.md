# Renderer roadmap

## Phase 0 — native graphics proof of concept

- [x] Start a clean Linux-only working tree.
- [x] Create a tiny SDL2/OpenGL demo with no Minecraft runtime.
- [ ] Build and run it on Ubuntu with Mesa.
- [ ] Add FPS/frame-time reporting and resize handling.

## Phase 1 — map the rendering boundary

Audit Sodium's source on the untouched `dev` branch and classify each component as:

1. Minecraft/game integration (not reusable for a desktop renderer).
2. Generic algorithms that could be reimplemented independently.
3. OpenGL state/batching ideas that need a clean API boundary.
4. Third-party code with its own license and notice obligations.

Do not bulk-copy files into the prototype. Sodium is licensed primarily under PolyForm Shield 1.0.0, which includes a noncompete clause; get qualified licensing advice before turning copied code into a desktop-rendering product. Prefer independently written implementations where practical.

## Phase 2 — renderer library

Design a renderer API around generic surfaces and GPU resources, not Minecraft chunks or game objects. Add explicit resource lifetime, damage regions, scaling, frame pacing, and GPU capability checks. Benchmark against a simple baseline before optimizing.

## Phase 3 — compositor research

Study Mutter's renderer and scene graph APIs for the exact GNOME versions targeted. Build a separate experimental integration or benchmark harness first. Never replace system compositor files or disable the working desktop to test this prototype.

## Not done yet

This branch is not a Mutter plugin, not a GNOME Shell extension, not a replacement compositor, and not a claim that Sodium can directly render GNOME windows.
