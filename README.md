# Glint Engine

> /ɡlɪnt/ \[*noun*] &mdash; a small, bright flash of light

Small and simple JavaScript game engine written in C++ using QuickJS

## Features

- Write games in JavaScript
- Built with raylib for rendering
- Fast reload with F5
- Audio support (music & sound effects)
- 2D graphics, camera, input handling

## Quick Start

```bash
# Install dependencies
just configure

# Build the engine
xmake build glint

# Run an example game
xmake run glint examples/balls
```

## Examples

Check the `examples/` directory:
- `balls/` - Bouncing balls with collision sounds
- `camera/` - Camera movement and zoom
- `text/` - Text rendering with custom fonts
- `texture/` - Image loading and rendering

## Building

### Requirements

- C++20 compiler
- [Xmake](https://xmake.io/)
- Node.js (for type checking)

### Commands

```bash
just configure    # Setup project
just build        # Build engine
just check        # Run all checks
just run <game>   # Run a game
```
