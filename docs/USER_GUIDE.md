# Creality MultiMaterial Studio - OpenVCAD Integration

A comprehensive integration tool for using OpenVCAD Studio volumetric designs with Creality's MultiMaterial 3D printing system.

## Overview

This application bridges the gap between OpenVCAD Studio (volumetric multi-material CAD software) and Creality's advanced multi-material 3D printing ecosystem. It enables designers to:

- Import OpenVCAD project files
- Configure different materials for different volumetric regions
- Export to 3MF format for seamless integration with Creality Print
- Support multi-material printing with PLA, ABS, TPU, and PETG

## Features

✨ **Key Features:**
- 🔧 Full OpenVCAD project file support (.vcad, .py, .json)
- 🎨 Multi-material configuration for volumetric regions
- 📦 3MF file export for Creality Print compatibility
- 🔥 Support for multiple material types (PLA, ABS, TPU, PETG)
- 💻 Command-line interface for automation
- 📝 JSON-based configuration files
- 🧊 Volumetric and voxel-based design support

## Installation

### Prerequisites
- Python 3.7 or higher
- OpenVCAD Studio (for volumetric design)
- Creality Print (for 3D printing)

### Setup
```bash
# Clone the repository
git clone https://github.com/thewriterben/CrealityPrint-KCAD.git
cd CrealityPrint-KCAD

# Make the script executable
chmod +x openvcad_multimaterial_studio.py
```

## Usage

### Basic Usage

1. **List supported formats:**
```bash
./openvcad_multimaterial_studio.py --list-formats
```

2. **List available materials:**
```bash
./openvcad_multimaterial_studio.py --list-materials
```

3. **Process an OpenVCAD project:**
```bash
./openvcad_multimaterial_studio.py --project my_design.vcad
```

### Advanced Usage

**With custom material configuration:**
```bash
./openvcad_multimaterial_studio.py \
  --project my_design.vcad \
  --material region1=PLA \
  --material region2=ABS \
  --material support=TPU \
  --output my_design_multimaterial.3mf
```

**Using a configuration file:**
```bash
./openvcad_multimaterial_studio.py \
  --project my_design.vcad \
  --config examples/default_config.json \
  --output output/my_design.3mf
```

**Processing Python-based OpenVCAD designs:**
```bash
./openvcad_multimaterial_studio.py \
  --project my_design.py \
  --config materials.json
```

## Configuration

### Material Configuration File Format

Create a JSON file with material assignments for volumetric regions:

```json
{
  "region1": "PLA",
  "region2": "ABS",
  "support": "TPU",
  "flexible_joint": "TPU"
}
```

See `examples/` directory for more configuration examples.

### Supported Materials

| Material | Temperature | Bed Temp | Characteristics |
|----------|-------------|----------|-----------------|
| PLA      | 200°C       | 60°C     | Rigid, easy to print |
| ABS      | 240°C       | 100°C    | Rigid, high temperature |
| TPU      | 230°C       | 50°C     | Flexible, elastic |
| PETG     | 235°C       | 80°C     | Semi-flexible, durable |

## Workflow

1. **Design in OpenVCAD Studio**
   - Create your volumetric design using OpenVCAD
   - Define material regions programmatically or visually
   - Export project files (.vcad or .py)

2. **Configure Materials**
   - Create a material configuration JSON file
   - Assign materials to different volumetric regions

3. **Process with OpenVCAD MultiMaterial Studio**
   - Run the application with your project and configuration
   - Generate 3MF output file

4. **Print with Creality Print**
   - Open the 3MF file in Creality Print
   - Load materials into your multi-material system
   - Start printing

## Examples

### Example 1: Graded Lattice Structure
```bash
./openvcad_multimaterial_studio.py \
  --project examples/sample_design.vcad \
  --config examples/default_config.json
```

### Example 2: Soft Robotics Design
```bash
./openvcad_multimaterial_studio.py \
  --project robotics_gripper.py \
  --config examples/flexible_config.json
```

### Example 3: Custom Material Mapping
```bash
./openvcad_multimaterial_studio.py \
  --project complex_design.vcad \
  --material core=ABS \
  --material shell=PETG \
  --material actuator=TPU \
  --material base=PLA
```

## Hardware Compatibility

This software is designed to work with:
- **Creality K2 Plus** with multi-material system
- **Creality CFS (Color Filament System)**
- Other Creality printers with multi-material support

## File Formats

### Input Formats
- `.vcad` - OpenVCAD project files (JSON-based)
- `.py` - Python-based OpenVCAD designs
- `.json` - Generic volumetric data files
- `.stl`, `.obj` - 3D mesh files for reference

### Output Formats
- `.3mf` - 3D Manufacturing Format (for Creality Print)
- `.json` - Metadata and configuration export

## OpenVCAD Integration Details

### Volumetric Design Support
OpenVCAD uses implicit functions and voxel-based representations for multi-material design. This integration:
- Preserves volumetric material assignments
- Maintains smooth material transitions
- Supports functionally graded structures
- Handles complex geometries efficiently

### Material Region Mapping
The application maps OpenVCAD's material regions to Creality's material system:
- Each unique material region in OpenVCAD maps to a specific filament
- Material transitions are preserved in the 3MF output
- Graded structures are maintained through voxel data

## Troubleshooting

### Common Issues

**Issue: "Project file not found"**
- Ensure the path to your OpenVCAD project is correct
- Use absolute paths if relative paths don't work

**Issue: "Unsupported file format"**
- Check that you're using a supported OpenVCAD file format
- Run `--list-formats` to see supported formats

**Issue: "Unknown material"**
- Verify material names match available materials
- Run `--list-materials` to see available materials

**Issue: "Material region not recognized"**
- Ensure region names in config match those in your OpenVCAD design
- Check for typos in region identifiers

## Advanced Features

### Python API Integration
For programmatic control, import the module directly:

```python
from openvcad_multimaterial_studio import OpenVCADMultiMaterialStudio

app = OpenVCADMultiMaterialStudio()
result = app.process_project(
    'design.vcad',
    {'region1': 'PLA', 'region2': 'ABS'},
    'output.3mf'
)
```

## Development

### Running Tests
```bash
python -m unittest tests/test_openvcad_studio.py -v
```

### Contributing
Contributions are welcome! Please feel free to submit pull requests.

## License

MIT License - See LICENSE file for details

## Support

For issues and questions:
- GitHub Issues: https://github.com/thewriterben/CrealityPrint-KCAD/issues
- Documentation: See `docs/` directory

## Acknowledgments

- OpenVCAD Development Team at CU Boulder
- Creality 3D Printing Community
- Contributors to this project

## Related Resources

- [OpenVCAD Official Documentation](https://matterassembly.org/openvcad)
- [OpenVCAD GitHub Repository](https://github.com/MacCurdyLab/OpenVCAD-Public)
- [Creality Print Documentation](https://www.creality.com/pages/software)

---

**Version:** 1.0.0  
**Last Updated:** October 2025
