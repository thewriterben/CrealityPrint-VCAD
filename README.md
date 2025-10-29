# CrealityPrint-KCAD

Creality MultiMaterial Studio integration for OpenVCAD volumetric designs - bringing multi-material 3D printing capabilities to advanced volumetric and voxel-based design workflows.

## 🎯 Overview

This project provides a seamless integration between OpenVCAD Studio (a volumetric, multi-material CAD system) and Creality's MultiMaterial 3D printing ecosystem. It enables designers to create complex multi-material objects with smooth material transitions, graded structures, and functionally optimized parts.

## ✨ Features

- **OpenVCAD Integration**: Direct support for OpenVCAD project files (.vcad, .py, .json)
- **Multi-Material Support**: Configure different materials (PLA, ABS, TPU, PETG) for different design regions
- **Volumetric Design**: Support for voxel-based and implicit function designs
- **3MF Export**: Seamless export to 3MF format for Creality Print compatibility
- **Flexible Workflows**: Command-line interface with JSON configuration support
- **Material Profiles**: Pre-configured material settings optimized for multi-material printing
- **Graded Structures**: Support for smooth material transitions and functionally graded designs

## 🚀 Quick Start

```bash
# List available materials
./openvcad_multimaterial_studio.py --list-materials

# Process an OpenVCAD project with default settings
./openvcad_multimaterial_studio.py --project my_design.vcad

# Use custom material configuration
./openvcad_multimaterial_studio.py \
  --project my_design.vcad \
  --material region1=PLA \
  --material region2=ABS \
  --output my_design.3mf
```

## 📖 Documentation

- [User Guide](docs/USER_GUIDE.md) - Complete usage instructions
- [Material Examples](examples/materials.md) - Material configuration examples
- [API Documentation](docs/API.md) - Developer reference

## 🛠️ Requirements

- Python 3.7+
- OpenVCAD Studio (for volumetric design)
- Creality Print (for 3D printing)
- Creality multi-material compatible printer (K2 Plus recommended)

## 📦 Installation

```bash
git clone https://github.com/thewriterben/CrealityPrint-KCAD.git
cd CrealityPrint-KCAD
chmod +x openvcad_multimaterial_studio.py
```

## 🎨 Use Cases

- **Functionally Graded Materials**: Smooth transitions between rigid and flexible regions
- **Impact-Absorbing Designs**: Strategic material placement for energy absorption
- **Medical Models**: Scan-to-print workflows with tissue-specific materials
- **Lattice Structures**: Graded struts with varying material properties
- **Soft Robotics**: Compliant mechanisms with embedded rigid components
- **Educational Demonstrations**: Multi-material property visualization

## 🤝 Contributing

Contributions are welcome! Please feel free to submit issues and pull requests.

## 📄 License

MIT License - See LICENSE file for details

## 🔗 Links

- [Creality Official](https://www.creality.com/)
- [OpenVCAD Official](https://matterassembly.org/openvcad)
- [OpenVCAD GitHub](https://github.com/MacCurdyLab/OpenVCAD-Public)
- [Project Repository](https://github.com/thewriterben/CrealityPrint-KCAD)