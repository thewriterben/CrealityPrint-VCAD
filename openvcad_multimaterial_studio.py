#!/usr/bin/env python3
"""
Creality MultiMaterial Studio - OpenVCAD Integration
This application bridges OpenVCAD Studio designs with Creality's multi-material 3D printing system.
"""

import os
import sys
import json
import argparse
from pathlib import Path
from typing import Dict, List, Optional, Tuple


class OpenVCADMultiMaterialStudio:
    """Main application class for OpenVCAD and Creality MultiMaterial Studio integration."""
    
    def __init__(self):
        self.version = "1.0.0"
        self.supported_formats = ['.vcad', '.py', '.json', '.3mf', '.stl', '.obj']
        self.materials = {
            'PLA': {'name': 'PLA', 'temp': 200, 'bed_temp': 60, 'color': 'default'},
            'ABS': {'name': 'ABS', 'temp': 240, 'bed_temp': 100, 'color': 'default'},
            'TPU': {'name': 'TPU', 'temp': 230, 'bed_temp': 50, 'color': 'default'},
            'PETG': {'name': 'PETG', 'temp': 235, 'bed_temp': 80, 'color': 'default'}
        }
        
    def print_banner(self):
        """Print application banner."""
        print("=" * 60)
        print("  Creality MultiMaterial Studio - OpenVCAD Integration")
        print(f"  Version {self.version}")
        print("=" * 60)
        print()
    
    def list_supported_formats(self):
        """List all supported file formats."""
        print("Supported File Formats:")
        for fmt in self.supported_formats:
            print(f"  - {fmt}")
        print()
    
    def list_materials(self):
        """List all available materials."""
        print("Available Materials:")
        for material_id, material in self.materials.items():
            print(f"  - {material_id}: {material['name']}")
            print(f"    Temperature: {material['temp']}°C")
            print(f"    Bed Temperature: {material['bed_temp']}°C")
        print()
    
    def load_openvcad_project(self, project_path: str) -> Dict:
        """
        Load an OpenVCAD project file.
        
        Args:
            project_path: Path to the OpenVCAD project file
            
        Returns:
            Dictionary containing project information
        """
        project_file = Path(project_path)
        
        if not project_file.exists():
            raise FileNotFoundError(f"Project file not found: {project_path}")
        
        if project_file.suffix not in self.supported_formats:
            raise ValueError(f"Unsupported file format: {project_file.suffix}")
        
        print(f"Loading OpenVCAD project: {project_file.name}")
        
        project_info = {
            'name': project_file.stem,
            'path': str(project_file),
            'format': project_file.suffix,
            'size': project_file.stat().st_size if project_file.exists() else 0
        }
        
        print(f"  Project Name: {project_info['name']}")
        print(f"  Format: {project_info['format']}")
        print(f"  Size: {project_info['size']} bytes")
        print()
        
        return project_info
    
    def configure_materials(self, material_config: Dict[str, str]) -> Dict:
        """
        Configure materials for multi-material printing.
        
        Args:
            material_config: Dictionary mapping component types to materials
            
        Returns:
            Validated material configuration
        """
        print("Configuring Materials:")
        validated_config = {}
        
        for component, material in material_config.items():
            if material not in self.materials:
                print(f"  Warning: Unknown material '{material}' for {component}")
                continue
            
            validated_config[component] = material
            print(f"  - {component}: {material}")
        
        print()
        return validated_config
    
    def export_to_3mf(self, project_info: Dict, material_config: Dict, output_path: str) -> str:
        """
        Export the configured project to 3MF format for Creality Print.
        
        Args:
            project_info: Project information dictionary
            material_config: Material configuration dictionary
            output_path: Path for the output 3MF file
            
        Returns:
            Path to the generated 3MF file
        """
        output_file = Path(output_path)
        
        print(f"Exporting to 3MF format: {output_file.name}")
        
        # Create 3MF metadata
        metadata = {
            'version': self.version,
            'source': 'OpenVCAD-MultiMaterial-Studio',
            'project': project_info,
            'materials': material_config,
            'printer': 'Creality K2 Plus',
            'multi_material': True
        }
        
        # In a real implementation, this would generate a proper 3MF file
        # For now, we'll create a JSON representation
        with open(output_file.with_suffix('.json'), 'w') as f:
            json.dump(metadata, f, indent=2)
        
        print(f"  Export completed: {output_file.with_suffix('.json')}")
        print(f"  Multi-material configuration included")
        print()
        
        return str(output_file.with_suffix('.json'))
    
    def process_project(self, project_path: str, material_config: Dict[str, str], 
                       output_path: Optional[str] = None) -> str:
        """
        Process an OpenVCAD project for multi-material printing.
        
        Args:
            project_path: Path to the OpenVCAD project
            material_config: Material configuration
            output_path: Optional output path for 3MF file
            
        Returns:
            Path to the generated output file
        """
        # Load project
        project_info = self.load_openvcad_project(project_path)
        
        # Configure materials
        validated_config = self.configure_materials(material_config)
        
        # Determine output path
        if output_path is None:
            output_path = f"{project_info['name']}_multimaterial.3mf"
        
        # Export to 3MF
        result = self.export_to_3mf(project_info, validated_config, output_path)
        
        return result


def main():
    """Main entry point for the application."""
    parser = argparse.ArgumentParser(
        description='Creality MultiMaterial Studio - OpenVCAD Integration',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Examples:
  %(prog)s --list-formats
  %(prog)s --list-materials
  %(prog)s --project my_design.vcad --config materials.json
  %(prog)s --project my_design.py --material region1=PLA --material region2=ABS
        """
    )
    
    parser.add_argument('--version', action='version', version='%(prog)s 1.0.0')
    parser.add_argument('--list-formats', action='store_true', 
                       help='List supported file formats')
    parser.add_argument('--list-materials', action='store_true',
                       help='List available materials')
    parser.add_argument('--project', '-p', type=str,
                       help='Path to OpenVCAD project file')
    parser.add_argument('--config', '-c', type=str,
                       help='Path to material configuration JSON file')
    parser.add_argument('--material', '-m', action='append', nargs=1,
                       help='Material assignment (format: component=material)')
    parser.add_argument('--output', '-o', type=str,
                       help='Output path for 3MF file')
    
    args = parser.parse_args()
    
    # Create application instance
    app = OpenVCADMultiMaterialStudio()
    app.print_banner()
    
    # Handle list commands
    if args.list_formats:
        app.list_supported_formats()
        return 0
    
    if args.list_materials:
        app.list_materials()
        return 0
    
    # Process project
    if args.project:
        material_config = {}
        
        # Load from config file if provided
        if args.config:
            with open(args.config, 'r') as f:
                material_config = json.load(f)
        
        # Add command-line material assignments
        if args.material:
            for material_arg in args.material:
                component, material = material_arg[0].split('=')
                material_config[component] = material
        
        # Default material configuration if none provided
        if not material_config:
            material_config = {
                'region1': 'PLA',
                'region2': 'ABS',
                'support': 'TPU'
            }
        
        try:
            result = app.process_project(args.project, material_config, args.output)
            print(f"Success! Output file: {result}")
            return 0
        except Exception as e:
            print(f"Error: {e}", file=sys.stderr)
            return 1
    
    # If no action specified, show help
    parser.print_help()
    return 0


if __name__ == '__main__':
    sys.exit(main())
