# PROJECT_STATUS.md - ADQ-PROTOCOLO

**Last Updated:** 2026-03-26 22:00
**Updated By:** Initial Setup
**Time Until Next Update:** Until end of work day

---

## 📊 CURRENT STATUS (One-Page Dashboard)

| Metric | Value | Status |
|---|---|---|
| **Current Phase** | Sprint 1 → Sprint 2 Preparation | ✅ On Track |
| **Overall Progress** | 25% | Layer 1 Done, Layers 2-6 Pending |
| **Critical Blockers** | 🔴 1 Critical | RF Hardware Decision Needed |
| **Last Major Milestone** | Sprint 1 Complete | 2026-03-26 |
| **Next Milestone** | S2.1 RF Driver Start | Waiting RF Procurement |

---

## ✅ WHAT WAS COMPLETED

### Last 24 Hours
- Created unified project status tracking system
- Consolidated documentation
- Set up daily update mechanism

### Sprint 1 (Complete ✅)
- ✅ Protocol specification v0.1 (11 message types)
- ✅ Frame codec with CRC-16-CCITT (encoder/decoder)
- ✅ Ring buffer for local persistence
- ✅ Time sync algorithm (low-pass filter)
- ✅ Arduino library with 2 examples (NodeTxDemo, BaseRxDemo)
- ✅ System architecture design (ADR-0001)
- ✅ Build system (CMake 3.16+)
- ✅ Unit tests for CRC validation

**Evidence:**
- `firmware/common/src/adq_codec.c` - Encoder/Decoder working
- `firmware/common/src/adq_crc16.c` - CRC validation tested
- `firmware/common/src/adq_ring_buffer.c` - Buffer management
- `firmware/common/src/adq_time_sync.c` - Sync algorithm
- `arduino/ADQProtocol/` - Arduino library ready
- `docs/protocol/protocol-v0.1.md` - Protocol documented
- `docs/adr/ADR-0001-radio-and-mac.md` - Architecture documented

---

## 🚫 CRITICAL BLOCKERS (What's Stopping Us)

### 🔴 BLOCKER #1: RF Hardware Decision - BLOCKS EVERYTHING

**Status:** ❌ UNRESOLVED
**Severity:** CRITICAL (Blocks entire Sprint 2)
**Since:** Project start
**Depends On:** Team decision
**Action Required:** Select RF module type

**Options:**
```
Option A: LoRa (Sub-1GHz FSK) - SX1276
  ✅ RECOMMENDED - TDMA friendly, range 10km, deterministic latency
  Cost: $15-20/unit
  Timeline: 3-5 days procurement

Option B: WiFi ESP32 Native
  ⚠️ Possible but complex - 802.11 CSMA/CA conflicts with TDMA
  Cost: Integrated (no extra)
  Timeline: 2+ weeks SDK customization

Option C: BLE
  ⚠️ Future consideration - Similar SDK complexity to WiFi

Option D: NB-IoT
  ❌ Not v1 (carrier dependency, slow)
```

**To Unblock:**
- [ ] Team decides: LoRa or WiFi or other?
- [ ] Procurement order placed
- [ ] Hardware arrives and tested
- [ ] Driver implementation begins (S2.1)

**Unblock By:** THIS WEEK (blocking S2.1 for 2+ weeks)
**Owner:** Project Lead

---

### 🟠 BLOCKER #2: Stream Parser FSM - Planned S2.3

**Status:** ⏳ PLANNED (scheduled S2.3)
**Severity:** HIGH (Affects robustness)
**Since:** Sprint 1 design review
**Issue:** Current decoder fails on single bit corruption (frame sync loss)

**Solution:** Implement state machine
```
SEARCH_START → find 0xA5
COLLECT_HEADER → catch 18 bytes
COLLECT_PAYLOAD → read N bytes (from header)
VALIDATE_CRC → check last 2 bytes
```

**Timeline:** 2-3 days (after S2.2)
**Owner:** To be assigned

---

### 🟠 BLOCKER #3: TDMA Scheduler - Planned S2.4

**Status:** ⏳ PLANNED (scheduled S2.4)
**Severity:** HIGH (Core MAC logic)
**Since:** Sprint 1 design
**Issue:** Base cannot assign slots or enforce boundaries

**Solution:** Implement scheduler + slot management
```
Beacon interval: 1000ms (configurable)
Slot duration: 150ms per node
Max nodes: 5-10 (configurable)
```

**Timeline:** 2-3 days (after S2.3)
**Owner:** To be assigned

---

## 📈 PROGRESS BY COMPONENT

```
Layer 1: Protocol & Codec        ████████████████████ 100% ✅ DONE
Layer 2: RF & MAC                ░░░░░░░░░░░░░░░░░░░░   0%  ❌ BLOCKER
Layer 3: Data Integrity          ██════════░░░░░░░░░░  50%  ⚠️  PARTIAL
Layer 4: Timing & Sync           ███░░░░░░░░░░░░░░░░░  40%  ⚠️  PARTIAL
Layer 5: Operations              ░░░░░░░░░░░░░░░░░░░░  10%  ❌ SKELETON
Layer 6: Observability           ░░░░░░░░░░░░░░░░░░░░  10%  ❌ SKELETON
                                 ————————————————————————
                     OVERALL:    ████░░░░░░░░░░░░░░  25%
```

### Component Details

| Component | Status | Evidence | Issues |
|---|---|---|---|
| Frame Format | ✅ 100% | Header 18B + payload 240B max + CRC | None |
| Message Types | ✅ 100% | 11 types: HELLO, BEACON, DATA_FRAME, ACK/NACK, etc | None |
| Encoder/Decoder | ✅ 100% | adq_codec.c working | None |
| CRC-16 CCITT | ✅ 100% | Tests pass | None |
| Ring Buffer | ✅ 100% | Persistence layer ready | None |
| Time Sync | ⚠️ 40% | Algorithm exists, not field validated | Needs validation |
| Arduino Library | ✅ 100% | Ready to use | Using Serial (needs RF driver swap) |
| RF Driver | ❌ 0% | Doesn't exist | ← **MAIN BLOCKER** |
| TDMA Scheduler | ❌ 0% | Doesn't exist | Planned S2.4 |
| Stream Parser FSM | ❌ 0% | Doesn't exist | Planned S2.3 |
| Security (Auth/Anti-Replay) | ❌ 0% | Doesn't exist | Planned S6 |
| ADC Integration | ❌ 0% | Doesn't exist | Planned S3 |

---

## 🗓️ SPRINT ROADMAP

```
Sprint 1: ✅ COMPLETE (Protocol Foundation)
         ├─ Protocol spec v0.1
         ├─ Codec + CRC
         ├─ Arduino library
         └─ Architecture docs

Sprint 2: 🟡 STARTING (RF + Scheduler) — WAITING RF DECISION
         ├─ S2.1: RF Driver (3-4d) — BLOCKED
         ├─ S2.2: Point-to-Point Test (1-2d)
         ├─ S2.3: Stream Parser FSM (2-3d)
         ├─ S2.4: TDMA Scheduler (2-3d)
         └─ S2.5: Integration Checkpoint (1d)

Sprint 3: 🟡 PLANNED (ADC + Stability, Starting ~Apr 17)
         ├─ ADC Driver & Sampling
         ├─ Payload Composition
         ├─ 24-Hour Stability Test
         └─ Hardening & Bug Fixes

Sprint 4: 🟡 PLANNED (Field Tests, Starting ~May 8)
         ├─ Test Campaign Setup (0.2km, 1km, 2km)
         ├─ RF Performance Measurement
         ├─ Loss & Retry Analysis
         └─ Report & Protocol Tuning

Sprint 5: 🟡 PLANNED (Robustness Hardening, ~May 29)
         ├─ Sequence Window & Duplicate Suppression
         ├─ Enhanced Telemetry
         ├─ Fuzz Testing & Edge Cases
         └─ Watchdog & Recovery

Sprint 6: 🟡 PLANNED (Security, ~Jun 19)
         ├─ Pre-Shared Key Auth
         ├─ Anti-Replay Protection
         ├─ Firmware Signing & Secure Boot
         └─ Operational Runbooks

Sprint 7: 🟡 PLANNED (Production, ~Jul 10)
         ├─ DFT/DFM Design
         ├─ FMEA & Risk Mitigation
         ├─ Production Test Plan
         └─ Release v1.0

TARGET RELEASE: August 2, 2026 🎉
```

---

## 🎯 WHAT'S NEEDED TO CONTINUE

### THIS WEEK (Critical Path)
1. **RF Decision** - Team chooses module (LoRa SX1276 recommended)
   - Action: Project Lead decides + orders
   - Unblocks: S2.1 (RF Driver impl)
   - Impact: Entire Sprint 2 timeline

2. **Hardware Procurement** - RF modules arrive
   - Timeline: 3-5 days from order
   - Validation: Basic connectivity test
   - Next: S2.1 implementation

### BEFORE S2.1
- [ ] RF hardware selected ✅ REQUIRED
- [ ] Basic RF connectivity verified (Tx/Rx works)
- [ ] Arduino IDE set up with radio examples

### BEFORE S2.2
- [ ] RF driver implemented (basic send/receive)
- [ ] Point-to-point test framework ready

### BEFORE S2.3
- [ ] S2.2 point-to-point test passes
- [ ] Baseline measurements documented (RSSI, distance, loss)

### BEFORE S2.4
- [ ] S2.3 stream parser FSM complete
- [ ] Parser tested with corrupted frames

### BEFORE S3 (ADC Integration)
- [ ] Final strain gauge hardware specifications locked
- [ ] ADC datasheet reviewed
- [ ] Sample code from manufacturer

---

## ❓ INFORMATION NEEDED FROM TEAM

| Question | Impact | Needed By | Owner |
|---|---|---|---|
| **Which RF module to use?** | 🔴 Blocks S2.1 | **ASAP (this week)** | Lead |
| **Where to order RF modules?** | Blocks procurement | This week | Lead |
| **Actual strain gauge model?** | Blocks S3.1 (ADC driver) | Before Sprint 3 | Hardware |
| **Target number of simultaneous nodes?** (2, 5, 10) | Affects TDMA design | Before S2.4 | Lead |
| **Latency budget?** (ms end-to-end) | Affects slot timing | Before S2.4 | Lead |
| **Acquisition rate per node?** (1Hz, 10Hz, 100Hz) | Affects payload size | Before S3 | Lead |
| **Battery life target?** (hours, days) | Affects sleep/wake design | Before S3 | Lead |
| **Field test locations determined?** (actual GPS coordinates) | Needed for S4 planning | Before Sprint 4 | Field Team |

---

## 🛠️ KEY FILES & LOCATIONS

```
docs/protocol/protocol-v0.1.md ··· Protocol spec (11 msg types, frame format)
docs/architecture/system-architecture-v1.md ··· System design + 6-layer robustness
docs/adr/ADR-0001-radio-and-mac.md ··· Design rationale

firmware/common/src/
  ├─ adq_codec.c ····· Frame encoder/decoder ✅
  ├─ adq_crc16.c ····· CRC-16-CCITT ✅
  ├─ adq_ring_buffer.c ····· Circular buffer ✅
  └─ adq_time_sync.c ····· Time alignment ✅

firmware/node/src/main.c ·············· Node entry point (skeleton)
firmware/base/src/main.c ·············· Base station entry point (skeleton)

arduino/ADQProtocol/ ··· Arduino library (ready to use)
  ├─ examples/NodeTxDemo/ ··· Example: node transmission
  └─ examples/BaseRxDemo/ ··· Example: base reception

tests/unit/test_crc16.c ··· CRC validation tests ✅

tools/host-cli/parse_frames.py ··· Frame parser tool
```

---

## 📞 CONTACTS & ESCALATION

| Role | Responsibility | Response Time |
|---|---|---|
| **Project Lead** | Decisions, RF selection, unblock team | <4 hours |
| **Sprint Lead** | Daily tracking, escalate blockers | <2 hours |
| **Tech Lead** | Architecture, design review, debugging | <1 day |
| **Hardware Owner** | Strain gauge specs, ADC integration (S3) | <1 day |

---

## 🚀 IMMEDIATE NEXT STEPS

```
TODAY (or ASAP):
  ☐ Team meets: decide RF module (LoRa recommended)
  ☐ Project Lead orders RF hardware
  ☐ Assign task owners for S2.1-S2.5

WEEK 1 (3-5 days):
  ☐ RF hardware arrives
  ☐ Basic connectivity test (can Tx/Rx)
  ☐ S2.1 RF driver implementation begins

WEEK 2-3:
  ☐ S2.2: Point-to-point link validated @ 10m, 50m, 100m
  ☐ S2.3: Stream parser FSM implemented
  ☐ S2.4: TDMA scheduler prototype

WEEK 4:
  ☐ Sprint 2 integration checkpoint
  ☐ Planning for Sprint 3 (ADC integration)
```

---

## 📝 UPDATE INSTRUCTIONS FOR NEXT PERSON

**To update this file at end of work day, run:**
```bash
/update-project-status
```

**This will:**
1. Ask you: "What did you complete today?"
2. Ask you: "Any blockers emerged?"
3. Ask you: "What's the next priority?"
4. Ask you: "Any decisions made?"
5. Auto-update this file's timestamp and recent changes section

**Then commit and push automatically.**

Or manually edit the "LAST 24 HOURS" section under "WHAT WAS COMPLETED" and update timestamp.

---

## 📊 HISTORICAL CHANGES

| Date | Who | Change | Impact |
|---|---|---|---|
| 2026-03-26 | Initial Setup | Created unified PROJECT_STATUS.md | Daily tracking enabled |
| 2026-03-20 | Sprint 1 Start | Protocol design begins | S1 foundation |
| 2026-03-26 | Sprint 1 End | Protocol spec complete ✅ | Blocks released for S2 |

---

**🎯 Remember:** This is THE SINGLE SOURCE OF TRUTH for project status.
**📅 Next Update:** End of today (use `/update-project-status` skill)
**⏰ Last Updated:** 2026-03-26 22:00

