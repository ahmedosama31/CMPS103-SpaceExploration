# Phase 2 Development Plan & Workload Management

## Overview
Phase 2 involves transforming the rudimentary Phase 1.2 simulation into a fully functional, deterministic discrete-event simulator. The goal is to process missions through their lifecycle (RDY -> OUT -> EXEC -> BACK -> DONE) while managing rover availability, maintenance, and statistics.

## Feature Breakdown & Tasks

### 1. Core Simulation Loop (The "Heart")
- [ ] **Implement Global Timer:** Ensure `current_day` increments correctly only after all daily operations are finished.
- [ ] **Deterministic Execution Order:**
    1. Check waiting missions (RDY) and assign rovers.
    2. Check OUT missions -> Move to EXEC if arrived.
    3. Check EXEC missions -> Move to BACK if finished.
    4. Check BACK missions -> Move to DONE if arrived.
    5. Check Rovers in Checkup -> Move to Available if done.
- [ ] **Mode Support:**
    - [ ] **Interactive Mode:** Pause after each day, print lists.
    - [ ] **Silent Mode:** Run to completion, produce output file only.

### 2. Mission Assignment (RDY -> OUT)
- [ ] **Priority Logic:**
    1. Emergency Missions (if applicable/Bonus).
    2. Polar Missions (Polar Rovers first, then Normal/Digging if allowed).
    3. Digging Missions (Digging Rovers only).
    4. Normal Missions (Normal Rovers first, then Polar, then Digging).
- [ ] **Assignment Action:**
    - Link Rover to Mission.
    - Calculate arrival time (optional, or check daily).
    - Move Rover to "Unavailable" (remove from Available list).
    - Move Mission to OUT list.

### 3. Mission Progression
- [ ] **OUT -> EXEC:**
    - Calculate travel time: `Distance / RoverSpeed` (converted to days).
    - If `(CurrentDay - LaunchDay) * 25 >= TravelTimeHours`, move to EXEC.
- [ ] **EXEC -> BACK:**
    - Track execution days.
    - If `DaysInExec >= MissionDuration`, move to BACK.
- [ ] **BACK -> DONE:**
    - Travel time is same as OUT.
    - When arrived, move to DONE.
    - **Critical:** Release the rover (see Section 4).

### 4. Rover Management
- [ ] **Release Rover:**
    - Increment rover's `MissionsCompleted`.
    - Check if `MissionsCompleted == M` (Checkup limit).
- [ ] **Checkup Logic:**
    - If needs checkup: Move to Checkup List. Set `CheckupExitDay = CurrentDay + CheckupDuration`.
    - If no checkup: Move back to appropriate Available List.
- [ ] **Checkup -> Available:**
    - Check daily if `CurrentDay >= CheckupExitDay`.
    - Move back to Available.

### 5. Abort Handling
- [ ] **Normal Mission Abort:**
    - Search RDY list -> Move to Aborted.
    - Search OUT list -> Recall Rover (Move to BACK immediately, or mark as aborted to handle on arrival).
    - Search EXEC/BACK -> Cannot abort.
- [ ] **Auto-Abort Polar:**
    - Check RDY Polar missions daily.
    - If `WaitTime > 2 * Duration`, move to Aborted.

### 6. Output & Statistics
- [ ] **Output File:**
    - Format: `Fday ID Rday Wdays MDUR Tdays`
    - Sort by `Fday` (Descending).
- [ ] **Statistics:**
    - Avg Wait Time, Avg Execution Time, Avg Turnaround Time.
    - Auto-abort % for Polar.
    - Rover/Mission counts.

---

## Suggested Workflow for "Member 1"

Assuming you are responsible for the **Core Simulation & Mission Assignment**:

1.  **Clean Slate:** Modify `MarsStation::Simulate()` to remove the random Phase 1 logic.
2.  **Skeleton Loop:** Create the `while(!finished)` loop that calls helper functions for each stage (e.g., `AssignMissions()`, `UpdateOutMissions()`, etc.).
3.  **Assignment Logic:** Implement `AssignMissions()`. This is the most complex logic involving the priority rules.
4.  **Time Steps:** Implement the logic to move missions between lists based on time (Distance/Speed).

## Tips for Success
- **Unit Testing:** Test your `AssignMissions` function with a specific input file (e.g., 1 rover, 1 mission) to ensure it assigns correctly on the right day.
- **Debug Prints:** In the early stages, print *everything* to console even in Silent Mode to trace where missions disappear.
- **Git:** Commit often. e.g., "Implemented RDY->OUT logic", "Implemented Checkup logic".

## Reference
- [Project PDF](Project_Docs/Data-Structures-and-Algorithms-Project.txt)
- [Weights CSV](Project_Docs/Phase2-Project-features-weights-F25-weights.csv)
