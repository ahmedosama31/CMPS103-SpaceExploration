# CMPS103-SpaceExploration 🚀

A Mars station simulation system that manages space exploration missions and rovers using advanced data structures and algorithms.

## Overview

This project simulates a Mars station that receives mission requests, assigns them to available rovers, and tracks mission execution. The system implements various data structures including queues, stacks, and priority queues to efficiently manage missions of different types (Emergency, Mountainous, and Polar).

## Features

- **Mission Management**: Handle multiple types of missions with different priorities
  - Emergency Missions (highest priority)
  - Mountainous Missions
  - Polar Missions
  
- **Rover Fleet**: Manage multiple rovers with different capabilities
  - Track rover availability and assignments
  - Monitor rover checkups and maintenance
  
- **Request Processing**: Process incoming mission requests and abort requests
  - Queue-based mission assignment
  - Priority-based mission execution
  
- **Simulation**: Day-by-day simulation of Mars station operations
  - Real-time mission tracking
  - Performance statistics and reporting

## Project Structure

```
├── MarsStation.h/cpp       # Main Mars station controller
├── Mission.h/cpp           # Mission class implementation
├── rover.h/cpp             # Rover class implementation
├── Requests.h/cpp          # Request handling
├── newRequest.h/cpp        # New mission requests
├── abortRequest.h/cpp      # Abort mission requests
├── LinkedQueue.h           # Linked list-based queue implementation
├── priQueue.h              # Priority queue implementation
├── ArrayStack.h            # Array-based stack implementation
├── QueueADT.h              # Queue abstract data type
├── StackADT.h              # Stack abstract data type
├── Node.h                  # Node structure for linked data structures
├── priNode.h               # Priority node structure
├── Enums.h/cpp             # Mission type enumerations
├── UI.h/cpp                # User interface handling
├── main.cpp                # Program entry point
└── input_case*.txt         # Test input files
```

## Data Structures Used

- **Linked Queue**: For managing waiting missions
- **Priority Queue**: For handling emergency missions
- **Array Stack**: For efficient mission operations
- **Custom Nodes**: For building dynamic data structures

## Getting Started

### Prerequisites

- C++ compiler (supporting C++11 or later)
- Visual Studio (project includes .sln and .vcxproj files)

### Building the Project

1. Clone the repository:
   ```bash
   git clone https://github.com/ahmedosama31/CMPS103-SpaceExploration.git
   cd CMPS103-SpaceExploration
   ```

2. Open the solution file in Visual Studio:
   ```
   CMPS103-SpaceExploration.sln
   ```

3. Build and run the project (F5 or Ctrl+F5)

### Running with Test Cases

The project includes several test input files (`input_case1.txt` through `input_case6.txt`). The program reads simulation parameters and events from these files.

## Input Format

Input files contain:
- Number of rovers and their specifications
- Mission parameters (duration, checkup times)
- Event list with timestamps and mission details

Example input structure:
```
<Number of rovers>
<Rover specifications>
<Auto-promotion parameters>
<Number of events>
<Event list>
```

## Output

The program generates:
- Day-by-day simulation log
- Mission assignments and completions
- Rover status updates
- Performance statistics (waiting times, execution times)
- Output files with mission summaries

## Course Information

This project was developed for **CMPS103** (Data Structures and Algorithms course).

## License

This project is available for educational purposes.

## Contributors

- [Ahmed Osama](https://github.com/ahmedosama31)

---

*Last updated: December 2025*