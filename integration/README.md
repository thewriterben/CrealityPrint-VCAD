# OpenVCAD Multi-Material Integration for CrealityPrint

**Author:** Manus AI
**Date:** October 29, 2025
**Version:** 1.0.0

## 1. Overview

This project provides a C++ based integration of OpenVCAD multi-material functionality directly into the CrealityPrint 3D slicer. It introduces a new file format handler that allows CrealityPrint to import OpenVCAD project files (`.vcad`, `.py`, `.json`) and correctly interpret their volumetric material data. The integration is designed to be seamless, extending CrealityPrint's existing multi-material capabilities to support the advanced, functionally graded designs produced by OpenVCAD.

The core of this integration is a new `VCAD` format handler built to parse OpenVCAD files, manage material region definitions, and prepare the data for CrealityPrint's slicing engine and 3MF export pipeline. A prototype has been successfully developed and tested, demonstrating the viability of this approach.

## 2. Integration Architecture

The integration is designed as a native C++ module within CrealityPrint's `libslic3r` library, ensuring performance and stability. The architecture consists of three main components:

| Component              | File(s)                                      | Description                                                                                                                              |
|------------------------|----------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------|
| **VCAD Format Handler**  | `VCAD.hpp`, `VCAD.cpp`                         | Contains the primary logic for parsing OpenVCAD files (`.vcad`, `.py`), extracting material regions, and converting them into a format that CrealityPrint can use. |
| **Python Bridge**        | `VCADPythonBridge.hpp`, `VCADPythonBridge.cpp` | An **optional** component that embeds a Python interpreter to interface directly with the `openvcad_multimaterial_studio.py` script for more complex file processing. |
| **Build System Script**  | `CMakeLists_VCAD.txt`                        | A CMake script snippet to integrate the new source files into the CrealityPrint build system, including the logic for enabling the optional Python support. |

This design allows for a lightweight, C++-only implementation for basic functionality, with the option to enable a more powerful Python-based backend if required.

## 3. How to Integrate

Follow these steps to integrate the OpenVCAD handler into the CrealityPrint source code.

### Step 1: Copy Integration Files

Place the provided integration files into the `CrealityPrint/src/libslic3r/Format/` directory.

```bash
# Assuming you are in the directory containing the vcad_integration folder
# and the cloned CrealityPrint repository is at /home/ubuntu/CrealityPrint

cp /home/ubuntu/vcad_integration/VCAD.hpp /home/ubuntu/CrealityPrint/src/libslic3r/Format/
cp /home/ubuntu/vcad_integration/VCAD.cpp /home/ubuntu/CrealityPrint/src/libslic3r/Format/

# Copy optional Python bridge files
cp /home/ubuntu/vcad_integration/VCADPythonBridge.hpp /home/ubuntu/CrealityPrint/src/libslic3r/Format/
cp /home/ubuntu/vcad_integration/VCADPythonBridge.cpp /home/ubuntu/CrealityPrint/src/libslic3r/Format/
```

### Step 2: Update CMakeLists.txt

Append the contents of `CMakeLists_VCAD.txt` to the end of `CrealityPrint/src/libslic3r/CMakeLists.txt`.

```bash
cat /home/ubuntu/vcad_integration/CMakeLists_VCAD.txt >> /home/ubuntu/CrealityPrint/src/libslic3r/CMakeLists.txt
```

This will add the new source files to the `libslic3r` target and introduce a new CMake option, `ENABLE_VCAD_PYTHON`.

### Step 3: Modify Main Application Logic

To complete the integration, you will need to modify CrealityPrint's main application logic to recognize and use the new format handler. This typically involves:

1.  **Registering the new file types:** In the file import dialog logic, add `.vcad`, `.py`, and `.json` as recognized extensions that should be handled by the `load_vcad` function.
2.  **Calling the loader:** In the file loading mechanism, add a condition to check for OpenVCAD file extensions and call `Slic3r::load_vcad()` instead of the standard model importers.
3.  **UI for Material Configuration:** Develop a user interface dialog that appears after a VCAD file is loaded. This dialog should display the material regions extracted from the file and allow the user to map each region to a specific filament extruder in CrealityPrint.

## 4. How to Build

Once the files are integrated, you can build CrealityPrint from source following its standard build process. To enable the optional Python support, you must have Python 3 development libraries installed and set the `ENABLE_VCAD_PYTHON` flag during the CMake configuration step.

### Build without Python Support (Recommended)

Follow the standard build instructions for CrealityPrint. The new VCAD handler will be compiled in by default.

### Build with Python Support

1.  **Install Python Development Libraries:**

    ```bash
    # For Debian/Ubuntu
    sudo apt-get update
    sudo apt-get install python3-dev
    ```

2.  **Configure with CMake:**

    When running CMake to configure the CrealityPrint build, add the following flag:

    ```bash
    cmake .. -DENABLE_VCAD_PYTHON=ON
    ```

3.  **Build CrealityPrint:**

    Proceed with the normal compilation process (e.g., `make`).

## 5. Deliverables

This project delivers a set of C++ source files, build scripts, and documentation that form a complete prototype for the integration.

| File Name                     | Description                                                                                             |
|-------------------------------|---------------------------------------------------------------------------------------------------------|
| `VCAD.hpp`                    | Header file for the C++ VCAD format handler, defining all data structures and class interfaces.         |
| `VCAD.cpp`                    | Implementation file for the C++ VCAD format handler, containing the core parsing logic.                 |
| `VCADPythonBridge.hpp`        | Header for the optional Python bridge.                                                                  |
| `VCADPythonBridge.cpp`        | Implementation of the optional Python bridge using the Python C-API.                                    |
| `CMakeLists_VCAD.txt`         | CMake script to add the new files and the `ENABLE_VCAD_PYTHON` option to the CrealityPrint build system. |
| `test_vcad_integration.cpp`   | A standalone test program to compile and validate the functionality of the VCAD handler.                |
| `Makefile`                    | A makefile for easily compiling and running the standalone test program.                                |
| `README.md`                   | This documentation file.                                                                                |

These files provide a solid foundation for completing the full integration into the CrealityPrint slicer.
