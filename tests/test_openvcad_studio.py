#!/usr/bin/env python3
"""
Unit tests for OpenVCAD MultiMaterial Studio
"""

import unittest
import os
import sys
import json
import tempfile
from pathlib import Path

# Add parent directory to path for imports
sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from openvcad_multimaterial_studio import OpenVCADMultiMaterialStudio


class TestOpenVCADMultiMaterialStudio(unittest.TestCase):
    """Test cases for OpenVCADMultiMaterialStudio class."""
    
    def setUp(self):
        """Set up test fixtures."""
        self.app = OpenVCADMultiMaterialStudio()
        self.temp_dir = tempfile.mkdtemp()
    
    def tearDown(self):
        """Clean up test fixtures."""
        # Clean up temp directory
        import shutil
        if os.path.exists(self.temp_dir):
            shutil.rmtree(self.temp_dir)
    
    def test_initialization(self):
        """Test application initialization."""
        self.assertEqual(self.app.version, "1.0.0")
        self.assertIsInstance(self.app.supported_formats, list)
        self.assertIsInstance(self.app.materials, dict)
    
    def test_supported_formats(self):
        """Test supported file formats."""
        expected_formats = ['.vcad', '.py', '.json', '.3mf', '.stl', '.obj']
        self.assertEqual(self.app.supported_formats, expected_formats)
    
    def test_materials_available(self):
        """Test that materials are properly defined."""
        expected_materials = ['PLA', 'ABS', 'TPU', 'PETG']
        for material in expected_materials:
            self.assertIn(material, self.app.materials)
            self.assertIn('name', self.app.materials[material])
            self.assertIn('temp', self.app.materials[material])
            self.assertIn('bed_temp', self.app.materials[material])
    
    def test_load_nonexistent_project(self):
        """Test loading a non-existent project file."""
        with self.assertRaises(FileNotFoundError):
            self.app.load_openvcad_project("/nonexistent/file.vcad")
    
    def test_load_unsupported_format(self):
        """Test loading an unsupported file format."""
        # Create a temp file with unsupported extension
        temp_file = os.path.join(self.temp_dir, "test.txt")
        with open(temp_file, 'w') as f:
            f.write("test")
        
        with self.assertRaises(ValueError):
            self.app.load_openvcad_project(temp_file)
    
    def test_load_valid_project(self):
        """Test loading a valid project file."""
        # Create a temp OpenVCAD project file
        temp_file = os.path.join(self.temp_dir, "test.vcad")
        with open(temp_file, 'w') as f:
            f.write('{"design": {"name": "test"}}')
        
        project_info = self.app.load_openvcad_project(temp_file)
        
        self.assertEqual(project_info['name'], 'test')
        self.assertEqual(project_info['format'], '.vcad')
        self.assertGreater(project_info['size'], 0)
    
    def test_configure_materials_valid(self):
        """Test material configuration with valid materials."""
        config = {
            'region1': 'PLA',
            'region2': 'ABS'
        }
        
        validated = self.app.configure_materials(config)
        
        self.assertEqual(validated['region1'], 'PLA')
        self.assertEqual(validated['region2'], 'ABS')
    
    def test_configure_materials_invalid(self):
        """Test material configuration with invalid materials."""
        config = {
            'region1': 'PLA',
            'region2': 'INVALID_MATERIAL'
        }
        
        validated = self.app.configure_materials(config)
        
        # Invalid material should be filtered out
        self.assertIn('region1', validated)
        self.assertNotIn('region2', validated)
    
    def test_export_to_3mf(self):
        """Test exporting to 3MF format."""
        project_info = {
            'name': 'test_design',
            'path': '/test/path/test_design.vcad',
            'format': '.vcad',
            'size': 1024
        }
        
        material_config = {
            'region1': 'PLA',
            'region2': 'ABS'
        }
        
        output_path = os.path.join(self.temp_dir, 'output.3mf')
        result = self.app.export_to_3mf(project_info, material_config, output_path)
        
        # Check that metadata file was created
        self.assertTrue(os.path.exists(result))
        
        # Verify metadata content
        with open(result, 'r') as f:
            metadata = json.load(f)
        
        self.assertEqual(metadata['version'], '1.0.0')
        self.assertEqual(metadata['source'], 'OpenVCAD-MultiMaterial-Studio')
        self.assertTrue(metadata['multi_material'])
    
    def test_process_project_complete_workflow(self):
        """Test complete project processing workflow."""
        # Create a temp OpenVCAD project file
        temp_file = os.path.join(self.temp_dir, "workflow_test.vcad")
        with open(temp_file, 'w') as f:
            f.write('{"design": {"name": "workflow_test"}}')
        
        material_config = {
            'region1': 'PLA',
            'region2': 'ABS',
            'support': 'PETG'
        }
        
        output_path = os.path.join(self.temp_dir, 'workflow_output.3mf')
        
        result = self.app.process_project(temp_file, material_config, output_path)
        
        # Verify output exists
        self.assertTrue(os.path.exists(result))
        
        # Verify output content
        with open(result, 'r') as f:
            data = json.load(f)
        
        self.assertEqual(data['project']['name'], 'workflow_test')
        self.assertIn('region1', data['materials'])
        self.assertIn('region2', data['materials'])


class TestMaterialProperties(unittest.TestCase):
    """Test cases for material properties."""
    
    def setUp(self):
        """Set up test fixtures."""
        self.app = OpenVCADMultiMaterialStudio()
    
    def test_pla_properties(self):
        """Test PLA material properties."""
        pla = self.app.materials['PLA']
        self.assertEqual(pla['temp'], 200)
        self.assertEqual(pla['bed_temp'], 60)
    
    def test_abs_properties(self):
        """Test ABS material properties."""
        abs_material = self.app.materials['ABS']
        self.assertEqual(abs_material['temp'], 240)
        self.assertEqual(abs_material['bed_temp'], 100)
    
    def test_tpu_properties(self):
        """Test TPU material properties."""
        tpu = self.app.materials['TPU']
        self.assertEqual(tpu['temp'], 230)
        self.assertEqual(tpu['bed_temp'], 50)
    
    def test_petg_properties(self):
        """Test PETG material properties."""
        petg = self.app.materials['PETG']
        self.assertEqual(petg['temp'], 235)
        self.assertEqual(petg['bed_temp'], 80)


if __name__ == '__main__':
    unittest.main()
