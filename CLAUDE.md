# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

**PhrasingModuleMM** is a single-module VCV Rack plugin (`phrasingmm`) that also compiles as a **MetaModule** hardware plugin. The module is called **THEREELPEET Phrasing** — a 4-lane probabilistic CV envelope generator for controlling presence/dynamics (0–5 V outputs).

There is exactly one module: `src/Phrasing.cpp` (logic + widget) and `src/plugin.cpp` (registration).

## Build

Requires the [metamodule-plugin-sdk](https://github.com/4ms/metamodule-plugin-sdk) and the `arm-none-eabi-gcc` 12.x toolchain.

```bash
cmake -S . -B build -G Ninja \
  -DMETAMODULE_SDK_DIR=/Users/peterbuchhop/vcv-dev/metamodule-plugin-sdk
cmake --build build
```

Output: `metamodule-plugins/PhrasingModuleMM.mmplugin`

The SDK path defaults to `../metamodule-plugin-sdk` (sibling directory) but can be overridden with the `METAMODULE_SDK_DIR` env var or CMake variable.

The MetaModule build compiles with `-DMETAMODULE` defined and targets `arm-none-eabi` via the SDK toolchain file at `${METAMODULE_SDK_DIR}/cmake/arm-none-eabi-gcc.cmake`.

## Architecture

The MetaModule SDK provides a `rack-interface` compatibility shim so the module can `#include <rack.hpp>` and use `rack::` DSP primitives (`dsp::SchmittTrigger`, `random::uniform`, etc.) unchanged from VCV Rack source. The `METAMODULE` preprocessor define is available for platform-specific branches if needed.

The `Phrasing` struct is a standard Rack `Module` subclass. All DSP runs in `Phrasing::process()` at sample rate — no sub-processing or background threads. The module keeps no heap allocations; all state is plain float/bool arrays on the struct.

Key signal flow in `process()`:
1. Read global params (density, jitter knobs, guarantee-one switch)
2. Handle per-lane enable button toggles (SchmittTrigger on button params)
3. Handle per-lane trig inputs (force lane on for a jittered duration)
4. Tick `gapTimer`; when it fires, roll each lane's weight probability and start `laneOnTimer`
5. Apply "guarantee one" override if all active lanes are idle
6. Smooth `laneTarget → laneValue` with one-pole coefficients derived from Duration (attack ≈ 20%, release ≈ 40%)
7. Write `0–5 V` output: `floor + (1 - floor) * smoothedValue`

`knobToSeconds()` and `densityToGapSeconds()` both use the same logarithmic mapping (`minS * pow(maxS/minS, knob)`), covering 5–90 s.

## Formatting

`.clang-format` lives in `src/`. The project uses C++20 (`gnu++20` for MetaModule, standard Rack toolchain for VCV).
