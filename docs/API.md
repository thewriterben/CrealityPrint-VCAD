# API Documentation

## OpenVCADMultiMaterialStudio Class

The main class for integrating OpenVCAD projects with Creality MultiMaterial printing.

### Constructor

```python
app = OpenVCADMultiMaterialStudio()
```

Creates a new instance of the application with default settings.

### Properties

- `version` (str): Application version number
- `supported_formats` (List[str]): List of supported file formats
- `materials` (Dict): Dictionary of available materials and their properties

### Methods

#### print_banner()
```python
app.print_banner()
```
Prints the application banner with version information.

**Returns:** None

---

#### list_supported_formats()
```python
app.list_supported_formats()
```
Lists all supported file formats to stdout.

**Returns:** None

---

#### list_materials()
```python
app.list_materials()
```
Lists all available materials with their properties.

**Returns:** None

---

#### load_openvcad_project(project_path)
```python
project_info = app.load_openvcad_project("/path/to/project.vcad")
```
Loads an OpenVCAD project file and returns its metadata.

**Parameters:**
- `project_path` (str): Path to the OpenVCAD project file

**Returns:** Dict containing:
- `name` (str): Project name
- `path` (str): Full path to project
- `format` (str): File format/extension
- `size` (int): File size in bytes

**Raises:**
- `FileNotFoundError`: If the project file doesn't exist
- `ValueError`: If the file format is not supported

---

#### configure_materials(material_config)
```python
validated_config = app.configure_materials({
    'region1': 'PLA',
    'region2': 'ABS'
})
```
Validates and configures materials for the project.

**Parameters:**
- `material_config` (Dict[str, str]): Mapping of region names to material names

**Returns:** Dict of validated material configuration

---

#### export_to_3mf(project_info, material_config, output_path)
```python
output_file = app.export_to_3mf(
    project_info,
    material_config,
    "/path/to/output.3mf"
)
```
Exports the configured project to 3MF format.

**Parameters:**
- `project_info` (Dict): Project information from load_openvcad_project()
- `material_config` (Dict): Validated material configuration
- `output_path` (str): Destination path for the 3MF file

**Returns:** str - Path to the generated file

---

#### process_project(project_path, material_config, output_path=None)
```python
result = app.process_project(
    "/path/to/project.vcad",
    {'region1': 'PLA', 'region2': 'ABS'},
    "/path/to/output.3mf"
)
```
Complete workflow: load, configure, and export a project.

**Parameters:**
- `project_path` (str): Path to the OpenVCAD project
- `material_config` (Dict[str, str]): Material configuration
- `output_path` (str, optional): Output file path (auto-generated if None)

**Returns:** str - Path to the generated output file

**Raises:**
- `FileNotFoundError`: If project file doesn't exist
- `ValueError`: If invalid configuration

## Material Configuration Format

### Structure
```json
{
  "region_name": "MATERIAL_NAME"
}
```

### Example
```json
{
  "region1": "PLA",
  "region2": "ABS",
  "support": "TPU",
  "flexible_joint": "TPU",
  "rigid_frame": "ABS"
}
```

### Available Materials

| Material | Properties |
|----------|------------|
| PLA | `{'name': 'PLA', 'temp': 200, 'bed_temp': 60, 'color': 'default'}` |
| ABS | `{'name': 'ABS', 'temp': 240, 'bed_temp': 100, 'color': 'default'}` |
| TPU | `{'name': 'TPU', 'temp': 230, 'bed_temp': 50, 'color': 'default'}` |
| PETG | `{'name': 'PETG', 'temp': 235, 'bed_temp': 80, 'color': 'default'}` |

## Command-Line Interface

### Usage
```bash
openvcad_multimaterial_studio.py [OPTIONS]
```

### Options

| Option | Description |
|--------|-------------|
| `--version` | Show version and exit |
| `--list-formats` | List supported file formats |
| `--list-materials` | List available materials |
| `--project PATH`, `-p PATH` | Path to OpenVCAD project file |
| `--config PATH`, `-c PATH` | Path to material configuration JSON |
| `--material ASSIGN`, `-m ASSIGN` | Material assignment (region=material) |
| `--output PATH`, `-o PATH` | Output path for 3MF file |

### Examples

**List materials:**
```bash
./openvcad_multimaterial_studio.py --list-materials
```

**List supported formats:**
```bash
./openvcad_multimaterial_studio.py --list-formats
```

**Process with inline configuration:**
```bash
./openvcad_multimaterial_studio.py \
  -p design.vcad \
  -m region1=PLA \
  -m region2=ABS \
  -o output.3mf
```

**Process with config file:**
```bash
./openvcad_multimaterial_studio.py \
  --project design.vcad \
  --config materials.json
```

**Process Python-based design:**
```bash
./openvcad_multimaterial_studio.py \
  --project design.py \
  --config materials.json \
  --output final_design.3mf
```

## Error Handling

The application handles the following error conditions:

- **FileNotFoundError**: Project file doesn't exist
- **ValueError**: Unsupported file format or invalid configuration
- **JSON errors**: Malformed configuration files

All errors are reported with descriptive messages to stderr.

## Return Codes

| Code | Meaning |
|------|---------|
| 0 | Success |
| 1 | Error (with message to stderr) |

## Example Usage in Python

### Basic Example
```python
from openvcad_multimaterial_studio import OpenVCADMultiMaterialStudio

# Create application instance
app = OpenVCADMultiMaterialStudio()

# Define material configuration
materials = {
    'region1': 'PLA',
    'region2': 'ABS',
    'support': 'TPU'
}

# Process project
try:
    output = app.process_project(
        'my_design.vcad',
        materials,
        'output.3mf'
    )
    print(f"Success: {output}")
except Exception as e:
    print(f"Error: {e}")
```

### Advanced Example with Multiple Regions
```python
from openvcad_multimaterial_studio import OpenVCADMultiMaterialStudio
import json

app = OpenVCADMultiMaterialStudio()

# Load material configuration from file
with open('complex_materials.json', 'r') as f:
    material_config = json.load(f)

# Add additional runtime materials
material_config['runtime_region'] = 'PETG'

# Process with custom output path
output = app.process_project(
    'complex_design.vcad',
    material_config,
    '/output/final_design.3mf'
)

print(f"Exported to: {output}")
```

### Batch Processing Example
```python
from openvcad_multimaterial_studio import OpenVCADMultiMaterialStudio
from pathlib import Path

app = OpenVCADMultiMaterialStudio()

# Process multiple designs
designs = Path('designs').glob('*.vcad')
material_config = {
    'region1': 'PLA',
    'region2': 'ABS'
}

for design in designs:
    try:
        output = app.process_project(
            str(design),
            material_config,
            f'output/{design.stem}.3mf'
        )
        print(f"Processed: {design.name} -> {output}")
    except Exception as e:
        print(f"Failed: {design.name} - {e}")
```

## Integration with OpenVCAD

### Volumetric Data Structure
OpenVCAD projects use volumetric representations with implicit functions. The integration:

1. **Reads project metadata** from .vcad (JSON) or .py files
2. **Extracts material regions** defined in the design
3. **Maps regions to materials** using the configuration
4. **Generates 3MF output** preserving volumetric information

### Material Region Naming
Ensure your OpenVCAD designs use consistent region naming:
- Use descriptive names: `'core'`, `'shell'`, `'support'`
- Avoid special characters in region names
- Match region names exactly in configuration files

### Python-Based Designs
For Python-based OpenVCAD designs (.py files):
- The application reads the file metadata
- Region definitions should be documented in the file
- Material assignments work the same way as .vcad files

## Performance Considerations

- **File Size**: Large volumetric designs may take longer to process
- **Region Count**: More material regions increase processing complexity
- **Output Format**: 3MF generation time depends on design complexity

## Best Practices

1. **Use descriptive region names** in your OpenVCAD designs
2. **Test material configurations** with small designs first
3. **Validate output** before sending to printer
4. **Keep configuration files** organized and version-controlled
5. **Document custom material mappings** for future reference

## Troubleshooting

### Issue: Material regions not recognized
**Solution:** Ensure region names in configuration exactly match those in OpenVCAD design

### Issue: Export fails
**Solution:** Check disk space and write permissions for output path

### Issue: Invalid material assignment
**Solution:** Use `--list-materials` to verify available materials

## Version History

- **1.0.0** (October 2025): Initial release
  - OpenVCAD project support (.vcad, .py, .json)
  - Multi-material configuration
  - 3MF export
  - Command-line interface
