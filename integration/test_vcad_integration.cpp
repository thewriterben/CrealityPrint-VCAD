#include "VCAD.hpp"
#include <iostream>
#include <fstream>

// Mock implementations for testing without full libslic3r
namespace Slic3r {
    class Model {};
    class DynamicPrintConfig {};
    struct ConfigSubstitutionContext {};
}

using namespace Slic3r;

void test_vcad_parser() {
    std::cout << "\n=== Testing VCAD Parser ===" << std::endl;
    
    VCADFileParser parser;
    
    // Test file type detection
    std::cout << "\nTesting file type detection:" << std::endl;
    std::cout << "  test.vcad: " << (parser.is_vcad_file("test.vcad") ? "Valid" : "Invalid") << std::endl;
    std::cout << "  test.py: " << (parser.is_vcad_file("test.py") ? "Valid" : "Invalid") << std::endl;
    std::cout << "  test.json: " << (parser.is_vcad_file("test.json") ? "Valid" : "Invalid") << std::endl;
    std::cout << "  test.stl: " << (parser.is_vcad_file("test.stl") ? "Valid" : "Invalid") << std::endl;
    
    // Test material library
    std::cout << "\nTesting material library:" << std::endl;
    VCADMaterial pla = parser.get_default_material("PLA");
    std::cout << "  PLA: " << pla.nozzle_temp << "°C nozzle, " << pla.bed_temp << "°C bed" << std::endl;
    
    VCADMaterial abs = parser.get_default_material("ABS");
    std::cout << "  ABS: " << abs.nozzle_temp << "°C nozzle, " << abs.bed_temp << "°C bed" << std::endl;
    
    VCADMaterial tpu = parser.get_default_material("TPU");
    std::cout << "  TPU: " << tpu.nozzle_temp << "°C nozzle, " << tpu.bed_temp << "°C bed" << std::endl;
    
    VCADMaterial petg = parser.get_default_material("PETG");
    std::cout << "  PETG: " << petg.nozzle_temp << "°C nozzle, " << petg.bed_temp << "°C bed" << std::endl;
}

void test_material_config() {
    std::cout << "\n=== Testing Material Configuration ===" << std::endl;
    
    VCADFileParser parser;
    VCADMaterialConfig config;
    
    // Test JSON parsing
    std::string json_config = R"({
        "region1": "PLA",
        "region2": "ABS",
        "support": "TPU",
        "flexible_joint": "TPU",
        "rigid_frame": "ABS"
    })";
    
    if (parser.parse_material_config(json_config, config)) {
        std::cout << "\nParsed material configuration:" << std::endl;
        for (const auto& mapping : config.region_to_material) {
            std::cout << "  " << mapping.first << " -> " << mapping.second << std::endl;
        }
    }
}

void test_vcad_project_loading() {
    std::cout << "\n=== Testing VCAD Project Loading ===" << std::endl;
    
    // Create a test VCAD file
    std::string test_vcad_content = R"({
        "design": {
            "name": "test_design",
            "version": "1.0.0"
        },
        "regions": {
            "region1": {
                "material": "PLA",
                "bounds": [0, 0, 0, 10, 10, 10]
            },
            "region2": {
                "material": "ABS",
                "bounds": [10, 0, 0, 20, 10, 10]
            }
        }
    })";
    
    // Write test file
    std::ofstream test_file("/tmp/test_design.vcad");
    test_file << test_vcad_content;
    test_file.close();
    
    // Load the project
    VCADFileParser parser;
    VCADProject project;
    
    if (parser.load_vcad_project("/tmp/test_design.vcad", project)) {
        std::cout << "\nLoaded project:" << std::endl;
        std::cout << "  Name: " << project.project_name << std::endl;
        std::cout << "  Format: " << project.format << std::endl;
        std::cout << "  Size: " << project.file_size << " bytes" << std::endl;
        std::cout << "  Volumetric: " << (project.is_volumetric ? "Yes" : "No") << std::endl;
        std::cout << "  Regions: " << project.regions.size() << std::endl;
        std::cout << "  Materials: " << project.materials.size() << std::endl;
        
        for (const auto& region : project.regions) {
            std::cout << "    Region: " << region.region_name 
                     << " -> " << region.material_id << std::endl;
        }
    }
}

void test_3mf_metadata_generation() {
    std::cout << "\n=== Testing 3MF Metadata Generation ===" << std::endl;
    
    VCADProject project;
    project.project_name = "test_multimaterial";
    project.openvcad_version = "1.0.0";
    project.is_volumetric = true;
    
    project.regions.push_back(VCADRegion("core", "ABS"));
    project.regions.push_back(VCADRegion("shell", "PETG"));
    project.regions.push_back(VCADRegion("support", "TPU"));
    
    VCADMaterial abs;
    abs.type = "ABS";
    abs.nozzle_temp = 240;
    abs.bed_temp = 100;
    project.materials["ABS"] = abs;
    
    VCADMaterial petg;
    petg.type = "PETG";
    petg.nozzle_temp = 235;
    petg.bed_temp = 80;
    project.materials["PETG"] = petg;
    
    VCADMaterial tpu;
    tpu.type = "TPU";
    tpu.nozzle_temp = 230;
    tpu.bed_temp = 50;
    project.materials["TPU"] = tpu;
    
    VCADMaterialConfig config;
    config.region_to_material["core"] = "ABS";
    config.region_to_material["shell"] = "PETG";
    config.region_to_material["support"] = "TPU";
    
    VCADTo3MFConverter converter;
    std::string metadata;
    
    if (converter.generate_3mf_metadata(project, config, metadata)) {
        std::cout << "\nGenerated 3MF metadata:" << std::endl;
        std::cout << metadata << std::endl;
    }
}

void test_default_config_generation() {
    std::cout << "\n=== Testing Default Config Generation ===" << std::endl;
    
    VCADProject project;
    project.regions.push_back(VCADRegion("region1", "PLA"));
    project.regions.push_back(VCADRegion("region2", "ABS"));
    project.regions.push_back(VCADRegion("support", "TPU"));
    
    VCADMaterialConfig config = get_default_material_config(project);
    
    std::cout << "\nGenerated default configuration:" << std::endl;
    std::cout << "Region to Material mapping:" << std::endl;
    for (const auto& mapping : config.region_to_material) {
        std::cout << "  " << mapping.first << " -> " << mapping.second << std::endl;
    }
    
    std::cout << "\nMaterial to Extruder mapping:" << std::endl;
    for (const auto& mapping : config.material_to_extruder) {
        std::cout << "  " << mapping.first << " -> Extruder " << mapping.second << std::endl;
    }
}

void test_supported_extensions() {
    std::cout << "\n=== Testing Supported Extensions ===" << std::endl;
    
    std::vector<std::string> extensions = get_vcad_extensions();
    
    std::cout << "\nSupported OpenVCAD file extensions:" << std::endl;
    for (const auto& ext : extensions) {
        std::cout << "  " << ext << std::endl;
    }
}

int main() {
    std::cout << "========================================" << std::endl;
    std::cout << "OpenVCAD Integration Test Suite" << std::endl;
    std::cout << "========================================" << std::endl;
    
    try {
        test_vcad_parser();
        test_material_config();
        test_vcad_project_loading();
        test_3mf_metadata_generation();
        test_default_config_generation();
        test_supported_extensions();
        
        std::cout << "\n========================================" << std::endl;
        std::cout << "All tests completed successfully!" << std::endl;
        std::cout << "========================================" << std::endl;
        
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "\nTest failed with exception: " << e.what() << std::endl;
        return 1;
    }
}
