"""
OpenVCAD Sample Design - Multi-Material Graded Structure
This is a sample OpenVCAD design file demonstrating volumetric multi-material design.
"""

# Import OpenVCAD (would be: from openvcad import *)
# This is a demonstration file showing the structure

# Design metadata
design_name = "graded_lattice"
design_version = "1.0"
voxel_resolution = 0.5  # mm

# Material regions definition
regions = {
    "core": {
        "material": "ABS",
        "density": 0.8,
        "coordinates": [(0, 0, 0), (50, 50, 50)]
    },
    "outer_shell": {
        "material": "PETG",
        "density": 1.0,
        "coordinates": [(50, 0, 0), (100, 50, 50)]
    },
    "flexible_joint": {
        "material": "TPU",
        "density": 0.6,
        "coordinates": [(100, 0, 0), (120, 50, 50)]
    }
}

# Geometric parameters
dimensions = {
    "length": 120,
    "width": 50,
    "height": 50
}

# Voxel data would be defined here in a real OpenVCAD file
# This is a placeholder demonstration

print(f"OpenVCAD Design: {design_name}")
print(f"Version: {design_version}")
print(f"Regions: {len(regions)}")
