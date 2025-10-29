# OpenVCAD MultiMaterial Integration Analysis

## Current State

### Repository Structure

1. **CrealityPrint-VCAD** (Integration Tool)
   - Python-based CLI tool
   - Bridges OpenVCAD designs with Creality multi-material printing
   - Loads `.vcad`, `.py`, `.json` files
   - Exports to 3MF format with material metadata
   - Currently standalone - not integrated into CrealityPrint slicer

2. **CrealityPrint** (3D Slicer)
   - C++ application based on Orca Slicer (fork of Bambu Studio/PrusaSlicer)
   - Full-featured slicer with extensive 3MF support
   - Built with CMake
   - Has sophisticated 3MF import/export in `src/libslic3r/Format/bbs_3mf.cpp`
   - Supports multi-material printing already

3. **OpenVCAD-Public** (CAD System)
   - Volumetric, multi-material CAD system
   - Uses implicit functions for geometry and materials
   - Supports functionally graded structures
   - Python library (`pyvcad`) for programmatic design
   - Outputs PNG stacks, FEA meshes, and meshes for FFF printing

## Key Components

### OpenVCADMultiMaterialStudio Class
Located in: `CrealityPrint-VCAD/openvcad_multimaterial_studio.py`

**Key Methods:**
- `load_openvcad_project(project_path)` - Loads .vcad, .py, .json files
- `configure_materials(material_config)` - Maps regions to materials (PLA, ABS, TPU, PETG)
- `export_to_3mf(project_info, material_config, output_path)` - Exports to 3MF with metadata
- `process_project(project_path, material_config, output_path)` - Complete workflow

**Supported Materials:**
- PLA: 200°C nozzle, 60°C bed
- ABS: 240°C nozzle, 100°C bed
- TPU: 230°C nozzle, 50°C bed
- PETG: 235°C nozzle, 80°C bed

### CrealityPrint 3MF Handling
Located in: `CrealityPrint/src/libslic3r/Format/`

**Key Files:**
- `bbs_3mf.cpp/hpp` - Bambu/Creality 3MF format handler
- `3mf.cpp/hpp` - Standard 3MF format handler
- Supports plate data, material info, gcode embedding

**Key Functions:**
- `load_bbs_3mf()` - Load 3MF files with plate and material data
- `store_bbs_3mf()` - Save 3MF files with full metadata

## Integration Strategy

### Option 1: Plugin Architecture (Recommended)
Add OpenVCAD import as a new file format handler in CrealityPrint.

**Pros:**
- Clean separation of concerns
- Follows existing architecture pattern
- Easy to maintain and extend
- Can be toggled on/off

**Implementation:**
1. Create new format handler: `src/libslic3r/Format/VCAD.cpp/hpp`
2. Integrate Python bridge or rewrite core logic in C++
3. Add UI elements for material region configuration
4. Hook into existing 3MF export pipeline

### Option 2: External Tool Integration
Keep the Python tool separate but add launcher/integration in CrealityPrint UI.

**Pros:**
- No modification to core slicer code
- Easier to develop and test
- Can be updated independently

**Cons:**
- Requires Python runtime
- Less seamless user experience
- Potential cross-platform issues

### Option 3: Direct Python Embedding
Embed Python interpreter in CrealityPrint and call the existing Python module.

**Pros:**
- Reuses existing Python code
- Can leverage pyvcad library directly
- Faster development

**Cons:**
- Adds Python dependency to C++ application
- Potential distribution/packaging issues
- Performance overhead

## Recommended Approach

**Hybrid Approach: C++ Format Handler + Python Backend**

1. **Phase 1: Add VCAD Format Support**
   - Create `VCAD.cpp/hpp` in `src/libslic3r/Format/`
   - Add `.vcad`, `.py` to supported import formats
   - Parse VCAD file metadata and extract region information

2. **Phase 2: Material Region UI**
   - Add UI dialog for material region configuration
   - Allow users to map OpenVCAD regions to filament slots
   - Store configuration in project settings

3. **Phase 3: 3MF Export Enhancement**
   - Extend existing `store_bbs_3mf()` to include VCAD material data
   - Preserve volumetric information in 3MF metadata
   - Ensure compatibility with Creality K2 Plus multi-material system

4. **Phase 4: Python Integration (Optional)**
   - For advanced features, embed Python interpreter
   - Call `pyvcad` for volumetric processing if needed
   - Provide fallback for systems without Python

## Technical Considerations

### File Format Handling
- OpenVCAD uses JSON-based `.vcad` files
- Python-based designs are `.py` files with pyvcad imports
- Need to extract material region definitions from both formats

### Material Mapping
- OpenVCAD regions are named (e.g., "region1", "core", "shell")
- CrealityPrint uses filament slots (1-4 on K2 Plus)
- Need bidirectional mapping system

### 3MF Metadata Structure
Current 3MF already supports:
- Multiple materials per object
- Plate-based organization
- Material properties (temp, bed temp)

Need to add:
- Volumetric region definitions
- Material gradient information
- OpenVCAD-specific metadata

### Build System Integration
- CrealityPrint uses CMake
- Need to add VCAD format handler to CMakeLists.txt
- Consider optional Python dependency

## Next Steps

1. Create VCAD format handler skeleton
2. Implement basic .vcad file parsing
3. Add material region configuration UI
4. Test with sample OpenVCAD files
5. Integrate with 3MF export pipeline
6. Add documentation and examples
