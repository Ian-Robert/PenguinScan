# 🐧 Penguin Scan 🐧

![C++](https://img.shields.io/badge/standard-C%2B%2B20-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows%20%7C%20Linux-lightgrey.svg)
![Build](https://img.shields.io/badge/build-CMake-green.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

**A high-performance, multi-threaded network scanner built from scratch in C++.**

PenguinScan is a command-line reconnaissance tool designed to map networks, detect open ports, and fingerprint services. PenguinScan runs on a **custom-built, zero-dependency networking engine** that compiles natively on both Windows (Winsock2) and Linux (Berkeley Sockets).

## Key Features

* **Cross-Platform Architecture:** Uses a custom abstraction layer (`Platform.h`) to translate OS-specific socket calls, allowing a single codebase to run seamlessly on Windows, Mac, AND Linux.
* **Multi-Threaded Engine:** Implements a **Producer-Consumer** threading model to scan hundreds of ports concurrently using `std::thread` and `std::atomic` synchronization.
* **CIDR Support:** Automatically expands subnets (e.g., `192.168.1.1/24`).
* **Structured Reporting:** Exports scan results to **JSON** or **Text** files for easy integration with other tools.
* **Banner Grabbing:** Performs service discovery by analyzing initial server responses.

---

## Engineering Highlights

* **Concurrency:** Managed thread pools manually without frameworks, utilizing `std::mutex` for thread-safe console logging and vector storage.
* **Memory Management:** Strict RAII adherence ensures sockets and file handles are automatically closed, preventing resource leaks during long scans.
* **Modern Build System:** Fully integrated **CMake** configuration for portable building across operating systems.

---

## Build Instructions

PenguinScan uses **CMake** for building. Ensure you have CMake (3.20+) and a C++ compiler installed.

### Linux / Mac

```bash
# 1. Create a build directory
mkdir build
cd build

# 2. Configure and Compile
cmake ..
make

# 3. Run
./PenguinScan -t 8.8.8.8 -p 53
```

### Windows 

```powershell
# 1. Create a build directory
mkdir build
cd build

# 2. Configure and Compile
cmake ..
cmake --build . --config Release

# 3. Run
.\Release\PenguinScan.exe -t 8.8.8.8 -p 53
```

---

## Usage Examples

*Basic Port Scan* Scan a single IP for common ports.

```
./PenguinScan -t 45.33.32.156 -p 22-80
```

*Network Sweep* Scan an entire local subnet for active web servers.

```
./PenguinScan -t 192.168.1.1/24 -p 443
```

*Mixed Targets & Reporting* Scan all ports on separate targets, then save the results to a JSON file on desktop.

```
./PenguinScan -t 8.8.8.8,192.168.1.0/24 -ap -s results.json C:\Users\Name\Desktop
```

---

## Usage

```
  TARGETING:
    -t,  --target <IP/CIDR>    Target IP, range, or comma-separated list
                               (e.g., 192.168.1.1, 10.0.0.0/24)

  PORT CONFIGURATION:
    -p,  --ports <range>       Port range (e.g., 80, 20-100)
    -ap, --all-ports           Scan all ports (1-65535)

  SCAN SETTINGS:
    -s,  --speed <0-5>         Scan speed (0=Slowest, 5=Fastest)
    -ti, --timeout <ms>        Connection timeout in milliseconds
    -b,  --banner              Attempt to grab service banners

  OUTPUT:
    -sa, --save <fmt> <dir>    Save results to file e.g., -sa results.txt C:\Users\Name\Desktop
                               

  MISC:
    -v,  --version             Show version information
    -h,  --help                Show this help message
```


