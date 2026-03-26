# ADQ-protocolo: Project Initialization Document

**Project Type:** Embedded Systems / Wireless Protocol Implementation
**Language:** C (C99) with Python utilities
**Build System:** CMake (3.16+)
**Repository:** Git
**Status:** v1 Development

---

## Project Overview

**ADQ-protocolo** is a wireless data acquisition system for remote sensor nodes (strain gauges) communicating with a central base station.

### Key Characteristics

- **Architecture:** Star topology (nodes → base station)
- **Synchronization:** Time-sync via beacon
- **Reliability:** ACK/NACK scheme + local buffering + retransmission logic
- **Protocol Inspiration:** LXRS-like principles
- **Output:** Real-time telemetry + link diagnostics

---

## Repository Structure

```
.
├── firmware/
│   ├── common/          # Shared libraries (codec, CRC, ring buffer, time sync)
│   ├── node/            # Remote node firmware
│   └── base/            # Base station firmware
├── tools/
│   ├── host-cli/        # PC utilities (frame parsing)
│   └── rf-lab/          # RF lab utilities
├── hardware/            # BOM, schematics, design notes
├── tests/
│   ├── unit/            # Unit tests (CRC, etc.)
│   └── sim/             # Functional simulations
├── docs/
│   ├── adr/             # Architecture Decision Records
│   └── research-sources.md
├── CMakeLists.txt       # Root build config
└── README.md            # Primary documentation
```

---

## Core Components

### Firmware (C99)

**Common Library** (`firmware/common/`)
- `adq_protocol.h/c` — Protocol frame definitions and parsing
- `adq_codec.h/c` — Data encoding/decoding
- `adq_crc16.h/c` — Checksum validation
- `adq_ring_buffer.h/c` — Circular buffer for local storage
- `adq_time_sync.h/c` — Time synchronization logic
- Build: `firmware/common/CMakeLists.txt`

**Node Application** (`firmware/node/`)
- Entry: `src/main.c`
- Business logic: `src/node_app.c` / `include/node_app.h`
- Responsibility: Collect sensor data, format frames, manage retransmission
- Build: `firmware/node/CMakeLists.txt`

**Base Station Application** (`firmware/base/`)
- Entry: `src/main.c`
- Business logic: `src/base_app.c` / `include/base_app.h`
- Responsibility: Send beacons, schedule slots (TDMA), manage ACK/NACK
- Build: `firmware/base/CMakeLists.txt`

### Tools

**host-cli** (`tools/host-cli/parse_frames.py`)
- Python 3, no external dependencies
- Parses hexadecimal frame sequences
- Usage: `python tools/host-cli/parse_frames.py 010203...`

**rf-lab** (`tools/rf-lab/`)
- RF testing utilities

### Testing

**Unit Tests** (`tests/unit/`)
- Example: `test_crc16.c`
- Enabled by default (`ADQ_BUILD_TESTS=ON`)
- Run with: `ctest --test-dir build`

**Simulations** (`tests/sim/`)
- Functional protocol simulations (loss/retry scenarios)

---

## Build & Environment Requirements

### Compiler Requirements

- **CMake:** ≥ 3.16
- **C Compiler:** C99 compatible
  - **Option 1 (Windows):** MSVC with "Desktop development with C++"
  - **Option 2:** `gcc`/`clang` from MinGW/MSYS2
- **Optional:** Python 3.8+ (for `tools/host-cli`)

### Verification

Before building, verify toolchain availability:

```bash
cmake --version
cl /?              # (if using MSVC)
# OR
gcc --version      # (if using MinGW)
python --version
```

### Build Steps

```bash
# Generate build artifacts
cmake -S . -B build

# Build (uses default generator, or add -G Ninja for Ninja)
cmake --build build

# Run tests
ctest --test-dir build
```

---

## Development Workflow

### File Modification Guidelines

**When modifying protocol/codec files:**
- Update corresponding tests in `tests/unit/`
- Run: `ctest --test-dir build` to validate
- Document changes in `docs/adr/` if architectural

**When adding node/base functionality:**
- Keep platform abstraction in `firmware/common/`
- Implement platform-specific logic in node/base folders
- Avoid hardcoding RF/ADC drivers (placeholders ok for v1)

**When updating documentation:**
- Update `README.md` for user-facing guidance
- Use `docs/adr/ADR-NNNN-*.md` for design decisions

### Key Flags in CMakeLists.txt

- `ADQ_BUILD_TESTS` - Enable/disable unit tests (default: ON)

### Test Execution

```bash
ctest --test-dir build              # Run all tests
ctest --test-dir build -V           # Verbose output
ctest --test-dir build -R <pattern> # Run matching tests
```

---

## Version 1 Scope

### Implemented

✅ Technical documentation (v1)
✅ Portable C firmware skeleton
✅ Frame codec and CRC
✅ Ring buffer for buffering
✅ Time sync framework
✅ Minimal CLI for frame parsing
✅ Unit test baseline (CRC, link profile, parser/fragment, scheduler, ACK/NACK transactions)

### Deferred (Post-v1)

❌ Full payload encryption and hardened key lifecycle
❌ Secure boot and signed OTA workflow
❌ Mesh/repeater topology

---

## Next Steps / Milestones

1. **Integrate real RF HAL** (nRF52840 2.4 GHz backend)
2. **Close end-to-end DATA_FRAME -> ACK/NACK over real radio**
3. **Execute staged field tests** (10 m, 50 m, 100 m LOS)
4. **Finalize v1 acceptance KPIs** (PER, latency, retries, recovery)

---

## Key Architecture Decisions

See `docs/adr/ADR-0001-radio-and-mac.md` for rationale on:
- Star topology choice
- TDMA-based MAC scheduling
- ACK/NACK + retransmission strategy
- Local buffering for link continuity

---

## Quick Reference: Common Tasks

| Task | Command |
|------|---------|
| Full clean build | `cmake -S . -B build && cmake --build build` |
| Run tests | `ctest --test-dir build` |
| Parse hex frame | `python tools/host-cli/parse_frames.py <hex>` |
| View test output | `ctest --test-dir build -V` |
| Change generator | `cmake -S . -B build -G Ninja` |

---

## Dependencies & External References

- **CMake ecosystem** — for cross-platform builds
- **C standard library** — all firmware is self-contained (no threading libs in v1)
- **Python stdlib** — `parse_frames.py` uses only `binascii`, `struct`, `sys`

---

## Contact / Maintainer Notes

- Project started: 2026-03-25
- Last updated: 2026-03-26
- Managed by: ALAN team (Becario 19)

---

## File Manifest (Key Files)

| File | Purpose |
|------|---------|
| `README.md` | User-facing primary docs |
| `CLAUDE.md` | This file — agent/developer onboarding |
| `agents/AGENTS.md` | Multi-agent workflow + contribution guidelines |
| `CMakeLists.txt` | Root build configuration |
| `firmware/common/adq_protocol.h` | Protocol frame spec |
| `firmware/node/src/main.c` | Node entry point |
| `firmware/base/src/main.c` | Base station entry point |
| `docs/adr/ADR-0001-*.md` | Architecture decisions |
| `tools/host-cli/parse_frames.py` | Frame diagnostic tool |
| `tests/unit/test_crc16.c` | CRC validation test |

---

## Agent-Specific Information

For **autonomous agents** (LLMs, CI/CD bots, automation systems), see **`agents/AGENTS.md`** which covers:

- Agent roles (contributor, QA, release manager, etc.)
- Approved workflows and git patterns
- Forbidden operations (safety guardrails)
- Decision trees for common scenarios
- Communication protocols (commit messages, PR templates)
- Integration points (CI/CD, APIs)

**End of Project Initialization Document**

For agent-specific workflows, consult `agents/AGENTS.md`.
For human developers, consult `README.md` and project ADRs in `docs/adr/`.
