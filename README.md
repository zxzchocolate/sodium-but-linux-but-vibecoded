# Sodium, but Linux — renderer prototype

A clean Linux-native rendering experiment inspired by the performance goals of [Sodium](https://github.com/CaffeineMC/sodium). The long-term idea is to investigate whether useful rendering techniques can be adapted for a Linux desktop compositor.

**Status: early prototype. This is not a desktop compositor and it does not replace GNOME/Mutter.** The current demo opens an SDL2/OpenGL window and draws a triangle, proving the native graphics toolchain works before attempting compositor integration.

## What is here

- `renderer-prototype/` — small, independent C++/SDL2/OpenGL proof of concept.
- `docs/renderer-roadmap.md` — staged plan for researching a real desktop-rendering integration.
- `UPSTREAM_NOTICES.md` — provenance and licensing notes for the original Sodium project.

The `linux-renderer-prototype` branch intentionally removes the Minecraft mod build, Fabric/NeoForge adapters, game assets, and mod-specific tests from its working tree. The original `dev` branch is left untouched as a reference snapshot.

## Build on Ubuntu

Install the native dependencies:

```sh
sudo apt update
sudo apt install build-essential cmake libsdl2-dev libgl-dev
```

Build and run:

```sh
cmake -S renderer-prototype -B build
cmake --build build -j"$(nproc)"
./build/sodium-linux-demo
```

On Wayland, SDL2 will normally use the available Wayland backend; if that is unavailable, it may use X11/XWayland.

## The actual goal

A desktop compositor must handle much more than drawing a scene: client surfaces, damage tracking, buffer lifetimes, input, frame scheduling, scaling, color management, multi-monitor layouts, and synchronization. GNOME's compositor is Mutter, so the realistic path is to prototype and benchmark rendering ideas independently, then study a well-defined Mutter integration point. Swapping Mutter's renderer wholesale is not a drop-in change.

## Upstream and licensing

Sodium is developed by CaffeineMC: https://github.com/CaffeineMC/sodium. This prototype does **not** copy Sodium renderer source code yet. Sodium's repository uses the PolyForm Shield 1.0.0 license for most project content, with separate third-party notices; read `UPSTREAM_NOTICES.md` before attempting to reuse any upstream code. This repository is an unofficial experiment and is not affiliated with CaffeineMC or GNOME.
