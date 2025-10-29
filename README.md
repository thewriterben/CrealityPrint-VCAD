# CrealityPrint-VCAD Integration

**OpenVCAD Multi-Material Integration for CrealityPrint 3D Slicer**

This repository contains the integration code and tools to enable CrealityPrint to import and process OpenVCAD multi-material volumetric designs.

## Project Overview

CrealityPrint-VCAD bridges the gap between OpenVCAD Studio (volumetric multi-material CAD software) and Creality's advanced multi-material 3D printing ecosystem. It provides both a Python-based standalone tool and a C++ integration module for direct incorporation into the CrealityPrint slicer.

## Repository Structure

```
CrealityPrint-VCAD/
├── openvcad_multimaterial_studio.py  # Python-based standalone tool
├── integration/                       # C++ integration for CrealityPrint
│   ├── VCAD.hpp                      # VCAD format handler header
│   ├── VCAD.cpp                      # VCAD format handler implementation
│   ├── VCADPythonBridge.hpp          # Optional Python bridge header
│   ├── VCADPythonBridge.cpp          # Optional Python bridge implementation
│   ├── CMakeLists_VCAD.txt           # CMake integration script
│   ├── test_vcad_integration.cpp     # Test program
│   ├── Makefile                      # Build script for tests
│   ├── README.md                     # Integration documentation
│   └── integration_analysis.md       # Technical analysis
├── docs/                              # Documentation
│   ├── API.md                        # API documentation
│   └── USER_GUIDE.md                 # User guide
├── examples/                          # Example files
│   ├── sample_design.vcad            # Sample VCAD file
│   ├── sample_design.py              # Sample Python design
│   ├── default_config.json           # Default material config
│   └── flexible_config.json          # Flexible material config
└── tests/                             # Test suite
    └── test_openvcad_studio.py       # Python tests
```

## Features

### Python Tool (`openvcad_multimaterial_studio.py`)
- ✅ Load OpenVCAD project files (`.vcad`, `.py`, `.json`)
- ✅ Configure materials for different volumetric regions
- ✅ Export to 3MF format for CrealityPrint
- ✅ Support for PLA, ABS, TPU, and PETG materials
- ✅ Command-line interface for automation

### C++ Integration Module (`integration/`)
- ✅ Native C++ format handler for CrealityPrint
- ✅ Parse OpenVCAD files directly in the slicer
- ✅ Material region extraction and mapping
- ✅ 3MF metadata generation
- ✅ Optional Python bridge for advanced features
- ✅ Tested and validated prototype

## Quick Start

### Using the Python Tool

```bash
# List available materials
./openvcad_multimaterial_studio.py --list-materials

# Process a design with material configuration
./openvcad_multimaterial_studio.py \
  --project examples/sample_design.vcad \
  --config examples/default_config.json \
  --output output.3mf
```

### Integrating into CrealityPrint

See the detailed instructions in `integration/README.md` for how to integrate the C++ module into the CrealityPrint source code.

## Supported File Formats

### Input Formats
- `.vcad` - OpenVCAD project files (JSON-based)
- `.py` - Python-based OpenVCAD designs
- `.json` - Generic volumetric data files

### Output Formats
- `.3mf` - 3D Manufacturing Format (for CrealityPrint)

## Supported Materials

| Material | Nozzle Temp | Bed Temp | Characteristics |
|----------|-------------|----------|-----------------|
| PLA      | 200°C       | 60°C     | Rigid, easy to print |
| ABS      | 240°C       | 100°C    | Rigid, high temperature |
| TPU      | 230°C       | 50°C     | Flexible, elastic |
| PETG     | 235°C       | 80°C     | Semi-flexible, durable |

## Hardware Compatibility

- **Creality K2 Plus** with multi-material system
- **Creality CFS (Color Filament System)**
- Other Creality printers with multi-material support

## Documentation

- **[Integration README](integration/README.md)** - Detailed C++ integration guide
- **[API Documentation](docs/API.md)** - Python API reference
- **[User Guide](docs/USER_GUIDE.md)** - Complete user guide
- **[Integration Analysis](integration/integration_analysis.md)** - Technical analysis

## Development

### Testing the C++ Integration

```bash
cd integration
make clean
make
./test_vcad_integration
```

### Testing the Python Tool

```bash
python -m unittest tests/test_openvcad_studio.py -v
```

## Installation

### Python Tool

```bash
# Clone the repository
git clone https://github.com/thewriterben/CrealityPrint-VCAD.git
cd CrealityPrint-VCAD

# Make executable
chmod +x openvcad_multimaterial_studio.py
```

### C++ Integration

Follow the instructions in `integration/README.md` to integrate into CrealityPrint source code.

## Contributing

Contributions are welcome! Please feel free to submit pull requests or open issues.

## License

MIT License - See LICENSE file for details

## Acknowledgments

- OpenVCAD Development Team at CU Boulder
- Creality 3D Printing Community
- Contributors to this project

## Related Resources

- [OpenVCAD Official Documentation](https://matterassembly.org/openvcad)
- [OpenVCAD GitHub Repository](https://github.com/MacCurdyLab/OpenVCAD-Public)
- [Creality Print Documentation](https://www.creality.com/pages/software)
- [CrealityPrint GitHub](https://github.com/CrealityOfficial/CrealityPrint)

---

**Version:** 1.0.0  
**Last Updated:** October 29, 2025  
**Author:** thewriterben with Manus AI
