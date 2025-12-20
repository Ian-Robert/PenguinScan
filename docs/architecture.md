# Architecture Overview

## Design Goals

- Cross-platform compatibility
- Clear seperation of concerns
- Minimal dependencies

## Layered Architecture

┌────────────────────────────────────────┐
│                CLI Layer               │
│  - Argument parsing                    │
│  - Help / usage output                 │
│  - Input validation                   │
└───────────────┬────────────────────────┘
                │
┌───────────────▼────────────────────────┐
│            Scan Controller              │
│  - Expands targets (IP/CIDR)            │
│  - Expands port ranges                  │
│  - Orchestrates scan lifecycle          │
│  - Handles cancellation & shutdown     │
└───────────────┬────────────────────────┘
                │
┌───────────────▼────────────────────────┐
│            Scanning Engine              │
│  - Thread pool                          │
│  - Work queue                           │
│  - Timeout enforcement                 │
│  - Rate limiting                       │
│  - Aggregates results                  │
└───────────────┬────────────────────────┘
                │
┌───────────────▼────────────────────────┐
│       Networking Abstraction Layer      │
│  - TCP connect logic                   │
│  - Address resolution                  │
│  - Connection timeouts                 │
│  - Error normalization                 │
└───────────────┬────────────────────────┘
                │
┌───────────────▼────────────────────────┐
│        OS-Specific Implementations      │
│  - Windows sockets                     │
│  - Linux sockets                       │
│  - Platform init / cleanup             │
└────────────────────────────────────────┘

## Platform Abstraction

Windows uses Winsock and Linux uses POSIX sockets.


