# PROJECT_STATUS_AGENT - Automated Daily Update

**How to use:** At end of work day, invoke the agent:

```bash
/update-project-status
# Or manually after work:
# 1. invoke Agent to gather updates
# 2. Updates PROJECT_STATUS.md automatically
# 3. Commits to git
```

---

## How It Works

The agent does this every time you invoke it:

1. **Reads** PROJECT_STATUS.md (current state)
2. **Asks you:**
   - What did you complete today?
   - Any blockers came up?
   - What's next priority?
   - Any team decisions made?
3. **Updates** PROJECT_STATUS.md automatically:
   - Timestamp
   - Completed tasks section
   - Blockers status
   - Sprint progress
4. **Commits** to git automatically

---

## To Invoke (When Ready)

Once your team is ready to use this, you have two options:

### Option A: Manual Invoke (Recommended for MVP)
At end of day:
```bash
/update-project-status
```

The agent will ask questions and update the file.

### Option B: Automatic Daily (Advanced)
```bash
/schedule-daily-status-update 17:00
```

Runs automatically at 5 PM every workday.

---

## Example Interaction

```
User: /update-project-status

Agent: What did you complete today? (List tasks or "none")
User: Implemented RF driver basic send/receive, validated 10m link

Agent: Any blockers emerged today?
User: None, on track

Agent: What's the next priority?
User: S2.2 point-to-point test at 50m and 100m

Agent: Any team decisions made?
User: Decided LoRa SX1276 is final RF module

Agent: ✅ Updating PROJECT_STATUS.md...
Agent: ✅ Committing changes...
Agent: ✅ Done! Status updated and pushed to git.

# File automatically updated with:
# - New completion entry
# - Timestamp refreshed
# - S2.1 marked as further along
# - RF decision logged
```

---

## Current Implementation Status

Currently: **MANUAL (you can edit PROJECT_STATUS.md directly)**

To automate with a skill/agent:
1. Ask me to create the `update-project-status` skill
2. Or configure a daily cron job with automations

For now: **Edit PROJECT_STATUS.md file directly** and commit, or ask me to invoke the Agent tool to help format the update.

---

## Implementation Steps (If You Want to Enable It)

To fully automate this, ask for:

```
"Create a skill /update-project-status that:
1. Reads PROJECT_STATUS.md
2. Asks project questions
3. Updates the file
4. Auto commits"
```

Or:

```
"Create an agent that updates PROJECT_STATUS.md daily at 17:00"
```

I can implement either one!

