# LogAnalyzer

A high-performance, lightweight C++ Command Line Interface (CLI) tool for parsing, analyzing, and detecting anomalies in large HTTP access log files. The project is designed with a strong focus on execution speed, memory efficiency, and robustness when processing massive datasets.

---

# Why C++?

## Performance & Execution Speed

C++ was intentionally selected to maximize performance. When processing log files containing hundreds of thousands of entries, C++ compiles directly to native machine code, eliminating the runtime interpretation overhead and garbage collection pauses associated with languages such as Python. This makes it particularly well suited for systems-level data processing.

## Developer Background

The language was also chosen based on my experience with C++. Having developed several command-line applications throughout my Fundamentals of Programming course, I am comfortable with C++'s memory management model and performance-oriented design, making it a natural choice for this project.

---

# Features

- High-performance parsing optimized for very large log files.
- Robust handling of malformed and corrupted log entries without interrupting execution.
- Detection of suspicious activities, including repeated authentication failures (401) and server error spikes (5xx).
- Configurable command-line filtering by time range.
- Configurable Top-N endpoint reporting.
- Built-in execution time benchmarking using high-resolution timers.
- Memory-efficient processing suitable for large datasets.

---

# Project Structure

```
LogAnalyzer/
├── CMakeLists.txt
├── include/
│   ├── LogEntry.h
│   ├── LogParser.h
│   ├── LogAnalyzer.h
│   └── Reporter.h
├── src/
│   ├── main.cpp
│   ├── LogParser.cpp
│   ├── LogAnalyzer.cpp
│   └── Reporter.cpp
├── sample/
│   └── access.log
└── build/
```

---

# Build Instructions

Open a terminal (or PowerShell) in the project root directory and execute the following commands:

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

---

# Usage

After compilation, the executable will be located at:

```text
./build/Release/LogAnalyzer.exe
```

## Standard Report

Generate the complete analysis report.

```bash
./build/Release/LogAnalyzer.exe ./sample/access.log
```

---

## Custom Top-N Endpoints

Display the top 20 most frequently accessed endpoints.

```bash
./build/Release/LogAnalyzer.exe ./sample/access.log -n 20
```

---

## Filter by Hour Range

Analyze only requests between 00:00 and 03:59.

```bash
./build/Release/LogAnalyzer.exe ./sample/access.log --start 0 --end 3
```

---

## Combined Filtering

Analyze requests between 00:00 and 03:59 and display only the top 5 endpoints.

```bash
./build/Release/LogAnalyzer.exe ./sample/access.log --start 0 --end 3 -n 5
```

---

# Output

The generated report includes:

- Total valid requests
- Number of malformed log entries
- HTTP status code distribution
- Most frequently requested endpoints
- Hourly traffic distribution
- Detection of suspicious login attempts (401)
- Detection of server error spikes (5xx)
- Total execution time

---

# Design Highlights

## Fast Parsing

The parser is designed around lightweight static utility functions to minimize unnecessary object creation and maximize throughput. Large log files containing nearly half a million entries can be processed in just over one second in Release mode.

## Fault Tolerance

Malformed lines, incomplete entries, and concatenated log fragments are automatically detected and skipped without affecting surrounding valid records. Invalid lines are counted separately while preserving the integrity of the final analysis.

## Memory Efficiency

The application processes logs using standard C++ containers with minimal overhead, making it suitable for very large datasets while maintaining a small memory footprint.

---

# Challenges and Lessons Learned

## Algorithmic Optimization

The primary challenge was achieving high throughput while maintaining complete analytical accuracy.

The dataset contained intentionally corrupted entries, including malformed records and inline-concatenated log fragments. A naïve parser would either terminate unexpectedly or incorrectly shift tokens, resulting in invalid statistics.

To solve this, the parsing logic was redesigned around optimized static parsing routines. Valid entries are isolated correctly, while malformed lines are redirected to a dedicated invalid-line counter. This approach ensures reliable statistics without sacrificing performance.

## CMake and Project Organization

Another significant challenge was learning CMake and organizing the project into a clean multi-directory structure.

Although initially unfamiliar with modern CMake workflows, building this project provided practical experience with:

- Multi-file C++ projects
- Header file organization
- Source directory separation
- Build configurations (Debug vs. Release)
- Cross-platform project structure
- Compiler optimization settings

This experience resulted in a maintainable and portable project layout.

---

# Requirements

- C++17 or newer
- CMake 3.10+
- A C++ compiler supporting C++17 (MSVC, GCC, or Clang)

---
