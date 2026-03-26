# AGENTS.md: Multi-Agent Contribution Guidelines

**Purpose:** Guide for autonomous agents (Claude instances, CI/CD bots, future AI systems) contributing to ADQ-protocolo.

---

## Context & Project Quick-Start

For full project overview, see **`CLAUDE.md`** at repository root.

**Key Facts:**
- Wireless data acquisition system (strain gauges → base station)
- Star topology, deterministic MAC, ACK/NACK + buffering
- Pure C99 + Python utilities, CMake build
- v1 in active implementation; priority is nRF52840 backend and 100 m validation

---

## Agent Roles & Workflows

### Role 1: Code Contributor / Feature Developer

**Scope:**
- Firmware implementation (node/base apps, common lib)
- Unit tests and simulations
- Minor documentation updates

**Workflow:**

```mermaid
1. Read `CLAUDE.md` + relevant ADR in `docs/adr/`
2. Understand target module (e.g., firmware/node/)
3. Create feature branch: git checkout -b feat/DESCRIPTION
4. Implement + test: cmake -S . -B build && ctest --test-dir build
5. Commit with clear message: "feat: Add X to Y"
6. Create PR with summary + test results
7. Await human review before merge
```

**Key Conventions:**

| What | Rule |
|------|------|
| Branch names | `feat/`, `fix/`, `docs/`, `test/` prefix |
| Commit structure | Atomic commits; link ADR if design-relevant |
| Platform code | Use `firmware/common/` for abstractions; avoid hardcoding RF/ADC |
| Test coverage | All protocol changes must run `ctest --test-dir build` successfully |

---

### Role 2: Documentation / Architecture Steward

**Scope:**
- ADR creation/updates
- Protocol design discussion
- README/docs refinement
- Design decision documentation

**Workflow:**

```
1. Identify decision point (e.g., "How to handle MAC collisions?")
2. Create ADR: docs/adr/ADR-NNNN-short-title.md
3. State: Context | Decision | Justification | Consequences
4. Seek human approval via PR
5. Merge and reference in README.md + code comments
```

**ADR Template:**

```markdown
# ADR-NNNN: [Title]

- **Status:** Proposed | Accepted | Deprecated
- **Date:** YYYY-MM-DD

## Context
[Why is this needed?]

## Decision
[What we're doing]

## Justification
[Why this approach?]

## Consequences
[Trade-offs, follow-ups]
```

---

### Role 3: Testing / QA Bot

**Scope:**
- Automated test execution on PR/commit
- Coverage validation
- Regression detection
- Performance baseline comparison (future)

**Workflow:**

```bash
# On every commit to main or PR:
1. cmake -S . -B build          # Clean build
2. ctest --test-dir build       # Run all tests
3. Comment in PR:
   - ✅ All tests pass
   - ❌ Test failures (with snippet)
   - ⚠️ Coverage below threshold (future)
```

**Exit Criteria (MUST PASS before merge):**
- ✅ All unit tests in `tests/unit/` pass
- ✅ No compilation warnings (if `-Werror` enabled)
- ✅ `ctest --test-dir build` returns code 0

---

### Role 4: Release / Integration Manager

**Scope:**
- Version bumping
- Changelog generation
- Tag creation
- Release artifact preparation

**Convention:**

```
Version scheme: v1.X.Y (e.g., v1.0.0, v1.1.0)
- MAJOR (1+): protocol generation changes
- MINOR (X): features compatible with current generation
- PATCH (Y): fixes without behavior break

Each release:
1. Create changelog entry
2. Update version in CMakeLists.txt (if any)
3. Tag: git tag -a vX.Y.Z -m "Release vX.Y.Z"
4. Push tag: git push origin vX.Y.Z
```

---

## Common Commands for Agents

### Build & Test

```bash
# Full clean build + test
cmake -S . -B build && cmake --build build && ctest --test-dir build -V

# Rebuild (incremental)
cmake --build build

# Test with verbose output
ctest --test-dir build -V

# Test specific suite (e.g., crc)
ctest --test-dir build -R crc
```

### Code Quality Checks

```bash
# (Future) Static analysis placeholder
# cppcheck firmware/

# (Future) Code coverage
# gcov + lcov
```

### Git Operations (Safe Patterns)

```bash
# Feature branch workflow
git checkout -b feat/my-feature
git add firmware/node/src/new_feature.c
git commit -m "feat: Implement X in node"

# Keep branch updated
git fetch origin
git rebase origin/main

# Create PR (requires human review)
# Use: gh pr create --title "..." --body "..."
```

---

## Forbidden Actions

**NEVER** (without explicit human authorization):

- ❌ Force-push to `main` or `develop` branches
- ❌ Delete commits or branches without discussion
- ❌ Modify `.github/workflows/` or CI config unilaterally
- ❌ Commit changes to `firmware/node/include/platform_config.h` with hardcoded chipset (use placeholders)
- ❌ Merge PRs without human approval
- ❌ Tag releases without version strategy alignment

---

## File Ownership & Modification Rules

### Protected Files (Requires Human Approval)

| File/Path | Why | Approval Level |
|-----------|-----|-----------------|
| `CMakeLists.txt` | Build system | Senior dev |
| `README.md` | Main user docs | Project lead |
| `docs/adr/` | Architecture | Design reviewer |
| `.github/workflows/` | CI/CD | DevOps |

### Open for Agent Modification

| Path | Rule |
|------|------|
| `firmware/node/src/` | Feature branches OK |
| `firmware/base/src/` | Feature branches OK |
| `firmware/common/` | Requires tests + ADR if core change |
| `tests/unit/` | Add tests freely; update existing requires discussion |

---

## Communication Protocol

### Commit Message Format

```
<type>: <subject>                     # max 70 chars

<body>                                # max 72 chars per line
- Detail 1
- Detail 2

Closes: #<issue-number> (if applicable)
Related-To: ADR-NNNN (if applicable)
```

**Types:** `feat`, `fix`, `docs`, `test`, `refactor`

### Pull Request Description Template

```markdown
## Summary
[1-2 sentences: what & why]

## Changes
- [ ] Feature / Bugfix / Docs
- [ ] Tests added/updated
- [ ] ADR referenced (if design-relevant)

## Validation
- Build: [✅/❌]
- Tests: [% pass rate]
- Link to test output: [if applicable]

## Checklist
- [ ] No force-push
- [ ] Branch updated from main
- [ ] Commit messages clear
- [ ] Ready for human review
```

---

## Decision Tree for Agents

```
Agent receives task: Implement X

├─ Is it a built-in test/validation?
│  ├─ YES → Run test, report results, don't merge PRs
│  └─ NO → Proceed
│
├─ Is it a code change touching firmware/common/?
│  ├─ YES → Must include unit test + ADR if architectural
│  └─ NO → Standard PR workflow
│
├─ Does it require breaking changes or new protocol version?
│  ├─ YES → Must create ADR (status: Proposed) + human sign-off
│  └─ NO → Feature branch OK
│
├─ Ready to merge?
│  ├─ TESTS PASS + PR DRAFTED → Wait for human approval
│  ├─ TESTS FAIL → Fix, push, re-test, don't merge
│  └─ CONFLICT → Rebase on main, resolve, don't force-push
```

---

## Integration Points (External Systems)

### CI/CD Hooks (if configured)

- **Pre-push:** Validate build + tests locally
- **On PR:** Auto-run `ctest`, comment results
- **On merge to main:** Tag with commit hash (optional)

### API / SDK Integration (Future)

- Claude API for code review suggestions
- GitHub API for PR/issue automation
- Slack/Discord webhooks for notifications

---

## Troubleshooting Guide for Agents

| Problem | Solution |
|---------|----------|
| Build fails: missing include | Check `firmware/common/include/` exports; update `CMakeLists.txt` if needed |
| Test fails: CRC mismatch | Validate test data alignment with `adq_protocol.h` spec |
| Git conflict on rebase | `git rebase --abort`, ask human for help, create new branch |
| PR approval takes long | Add context in PR body: link ADR, explain use case |

---

## Resources for Agents

- **Protocol Spec:** `docs/protocol/protocol-v1.md` (current ADQ v1 spec)
- **Architecture:** `docs/adr/ADR-0001-radio-and-mac.md`
- **Hardware:** `hardware/README.md`
- **Tool Reference:** `tools/host-cli/README.md`, `tools/rf-lab/README.md`
- **User Docs:** `README.md`

---

## Future Enhancements for Agent Coordination

- [ ] Shared memory/context for agent hand-offs
- [ ] Task queue system (Jira/GitHub Projects integration)
- [ ] Automatic changelog generation from commits
- [ ] Performance regression detection
- [ ] Distributed testing (coverage mapping across test suite)

---

**Last Updated:** 2026-03-26
**Version:** 1.0 (Provisional)

For human-specific guidelines, see `README.md`.
For project overview, see `CLAUDE.md`.
