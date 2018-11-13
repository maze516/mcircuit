<p align="center"> 
<img src="https://i.nuuls.com/hWIoU.png">
</p>

## About
Educational software for digital logic simulation heavily inspired by Logisim.

## Architecture
Consists of a simulator backend and the actual UI for simulating components in 2D plane.

Simulator backend is located in the `mcircuit` directory as a subproject.

The Qt frontend is in the root directory of the project.

Simulator backend must be completely independent of a UI framework used for frontend.

[See also](#wip-diagram-of-the-architecture)

## Building
### Simulator backend
#### Requirements
- C++17
- CMake
#### Build process
1. `cd` into the repository
2. `cd mcircuit`
2. `mkdir build && cd build`
3. `cmake ..`
4. `cmake --build .`

### Whole project
#### Requirements
- C++17
- CMake
- Qt 5
#### Build process
1. `cd` into the repository
2. `mkdir build && cd build`
3. `cmake ..`
4. `cmake --build .`

## Guidelines
### Creating simulator backend components
1. Extend the mcircuit::Component class.
2. Implement the virtual methods, make sure your constructor takes in an ID parameter (and calls base constructor).

## WIP Diagram of the architecture
![](https://i.nuuls.com/Tjc6k.png)
