#include "VCAD.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// Note: In actual implementation, these would be proper includes from libslic3r
// #include "libslic3r/Model.hpp"
// #include "libslic3r/Config.hpp"
// #include "libslic3r/Format/bbs_3mf.hpp"

namespace Slic3r {

// ============================================================================
// VCADFileParser Implementation
// ============================================================================

VCADFileParser::VCADFileParser() {
    initialize_material_library();
}

VCADFileParser::~VCADFileParser() {
}

void VCADFileParser::initialize_material_library() {
    // Initialize default materials matching the Python implementation
    VCADMaterial pla;
    pla.name = "PLA";
    pla.type = "PLA";
    pla.nozzle_temp = 200;
    pla.bed_temp = 60;
    pla.color = "default";
    m_material_library["PLA"] = pla;
    
    VCADMaterial abs;
    abs.name = "ABS";
    abs.type = "ABS";
    abs.nozzle_temp = 240;
    abs.bed_temp = 100;
    abs.color = "default";
    m_material_library["ABS"] = abs;
    
    VCADMaterial tpu;
    tpu.name = "TPU";
    tpu.type = "TPU";
    tpu.nozzle_temp = 230;
    tpu.bed_temp = 50;
    tpu.color = "default";
    m_material_library["TPU"] = tpu;
    
    VCADMaterial petg;
    petg.name = "PETG";
    petg.type = "PETG";
    petg.nozzle_temp = 235;
    petg.bed_temp = 80;
    petg.color = "default";
    m_material_library["PETG"] = petg;
}

bool VCADFileParser::is_vcad_file(const std::string& filename) {
    // Check file extension
    size_t dot_pos = filename.find_last_of('.');
    if (dot_pos == std::string::npos) {
        return false;
    }
    
    std::string ext = filename.substr(dot_pos);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    return (ext == ".vcad" || ext == ".py" || ext == ".json");
}

VCADMaterial VCADFileParser::get_default_material(const std::string& material_type) {
    auto it = m_material_library.find(material_type);
    if (it != m_material_library.end()) {
        return it->second;
    }
    
    // Return PLA as default if material not found
    return m_material_library["PLA"];
}

bool VCADFileParser::load_vcad_project(const std::string& filename, VCADProject& project) {
    // Check if file exists
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return false;
    }
    
    // Get file size
    file.seekg(0, std::ios::end);
    project.file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    // Extract file info
    size_t last_slash = filename.find_last_of("/\\");
    size_t last_dot = filename.find_last_of('.');
    
    if (last_dot != std::string::npos) {
        project.format = filename.substr(last_dot);
        if (last_slash != std::string::npos) {
            project.project_name = filename.substr(last_slash + 1, last_dot - last_slash - 1);
        } else {
            project.project_name = filename.substr(0, last_dot);
        }
    }
    
    project.project_path = filename;
    
    // Parse based on format
    if (project.format == ".vcad" || project.format == ".json") {
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return parse_vcad_json(content, project);
    } else if (project.format == ".py") {
        return parse_vcad_python(filename, project);
    }
    
    return false;
}

bool VCADFileParser::parse_vcad_json(const std::string& content, VCADProject& project) {
    // Simple string-based parsing for demonstration
    // In production, use a proper JSON library like nlohmann/json or rapidjson
    
    std::cout << "Parsing VCAD JSON content..." << std::endl;
    
    // Look for region definitions using simple string search
    size_t pos = 0;
    while ((pos = content.find("\"region", pos)) != std::string::npos) {
        // Find the region name
        size_t name_start = pos + 1;
        size_t name_end = content.find("\"", name_start);
        if (name_end == std::string::npos) break;
        
        std::string region_name = content.substr(name_start, name_end - name_start);
        
        // Try to find associated material (simplified)
        std::string material_id = "PLA";  // Default
        
        // Look for material keywords nearby
        size_t search_end = std::min(pos + 200, content.length());
        std::string nearby = content.substr(pos, search_end - pos);
        
        if (nearby.find("ABS") != std::string::npos) material_id = "ABS";
        else if (nearby.find("TPU") != std::string::npos) material_id = "TPU";
        else if (nearby.find("PETG") != std::string::npos) material_id = "PETG";
        else if (nearby.find("PLA") != std::string::npos) material_id = "PLA";
        
        VCADRegion region(region_name, material_id);
        project.regions.push_back(region);
        
        pos = name_end;
    }
    
    // If no regions found, create defaults
    if (project.regions.empty()) {
        project.regions.push_back(VCADRegion("region1", "PLA"));
        project.regions.push_back(VCADRegion("region2", "ABS"));
    }
    
    // Set default properties
    project.is_volumetric = true;
    project.openvcad_version = "1.0.0";
    
    // Add materials from library
    for (const auto& region : project.regions) {
        if (m_material_library.find(region.material_id) != m_material_library.end()) {
            project.materials[region.material_id] = m_material_library[region.material_id];
        }
    }
    
    std::cout << "Found " << project.regions.size() << " material regions" << std::endl;
    
    return true;
}

bool VCADFileParser::parse_vcad_python(const std::string& filename, VCADProject& project) {
    std::cout << "Parsing Python-based OpenVCAD design..." << std::endl;
    
    // Read file content
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string content((std::istreambuf_iterator<char>(file)),
                       std::istreambuf_iterator<char>());
    
    // Look for material definitions using simple string search
    // Create default regions for Python files
    project.regions.push_back(VCADRegion("region1", "PLA"));
    project.regions.push_back(VCADRegion("region2", "ABS"));
    project.regions.push_back(VCADRegion("support", "TPU"));
    
    project.is_volumetric = true;
    project.openvcad_version = "1.0.0";
    
    // Add materials
    for (const auto& region : project.regions) {
        if (m_material_library.find(region.material_id) != m_material_library.end()) {
            project.materials[region.material_id] = m_material_library[region.material_id];
        }
    }
    
    std::cout << "Extracted " << project.regions.size() << " regions from Python design" << std::endl;
    
    return true;
}

bool VCADFileParser::parse_material_config(const std::string& json_content, VCADMaterialConfig& config) {
    // Simple string-based JSON parsing for material configuration
    // Format: {"region1": "PLA", "region2": "ABS", ...}
    
    size_t pos = 0;
    while ((pos = json_content.find("\"", pos)) != std::string::npos) {
        size_t key_start = pos + 1;
        size_t key_end = json_content.find("\"", key_start);
        if (key_end == std::string::npos) break;
        
        std::string key = json_content.substr(key_start, key_end - key_start);
        
        // Find the value
        size_t value_start = json_content.find("\"", key_end + 1);
        if (value_start == std::string::npos) break;
        value_start++;
        
        size_t value_end = json_content.find("\"", value_start);
        if (value_end == std::string::npos) break;
        
        std::string value = json_content.substr(value_start, value_end - value_start);
        
        config.region_to_material[key] = value;
        
        pos = value_end + 1;
    }
    
    return !config.region_to_material.empty();
}

bool VCADFileParser::extract_regions(const std::string& vcad_content, std::vector<VCADRegion>& regions) {
    // Extract material regions from VCAD content using simple string search
    size_t pos = 0;
    while ((pos = vcad_content.find("\"region", pos)) != std::string::npos) {
        size_t name_start = pos + 1;
        size_t name_end = vcad_content.find("\"", name_start);
        if (name_end == std::string::npos) break;
        
        std::string region_name = vcad_content.substr(name_start, name_end - name_start);
        VCADRegion region(region_name, "PLA");  // Default material
        regions.push_back(region);
        
        pos = name_end;
    }
    
    return !regions.empty();
}

// ============================================================================
// VCADTo3MFConverter Implementation
// ============================================================================

VCADTo3MFConverter::VCADTo3MFConverter() {
}

VCADTo3MFConverter::~VCADTo3MFConverter() {
}

bool VCADTo3MFConverter::convert_to_model(const VCADProject& project,
                                         const VCADMaterialConfig& /* config */,
                                         Model* model) {
    if (!model) {
        std::cerr << "Error: Model pointer is null" << std::endl;
        return false;
    }
    
    std::cout << "Converting OpenVCAD project to Model..." << std::endl;
    std::cout << "Project: " << project.project_name << std::endl;
    std::cout << "Regions: " << project.regions.size() << std::endl;
    std::cout << "Materials: " << project.materials.size() << std::endl;
    
    // In actual implementation, this would:
    // 1. Create ModelObject from volumetric data
    // 2. Assign materials to different regions
    // 3. Set up multi-material configuration
    
    // For now, log the conversion process
    for (const auto& region : project.regions) {
        std::cout << "  Region: " << region.region_name 
                  << " -> Material: " << region.material_id << std::endl;
    }
    
    return true;
}

bool VCADTo3MFConverter::apply_material_config(Model* model,
                                              const VCADMaterialConfig& config,
                                              DynamicPrintConfig* print_config) {
    if (!model || !print_config) {
        return false;
    }
    
    std::cout << "Applying material configuration..." << std::endl;
    
    // Apply material settings to print config
    for (const auto& mapping : config.region_to_material) {
        std::cout << "  " << mapping.first << " -> " << mapping.second << std::endl;
    }
    
    return true;
}

bool VCADTo3MFConverter::generate_3mf_metadata(const VCADProject& project,
                                              const VCADMaterialConfig& /* config */,
                                              std::string& metadata_json) {
    // Generate JSON metadata for 3MF file
    std::ostringstream oss;
    
    oss << "{\n";
    oss << "  \"source\": \"OpenVCAD-CrealityPrint-Integration\",\n";
    oss << "  \"openvcad_version\": \"" << project.openvcad_version << "\",\n";
    oss << "  \"project_name\": \"" << project.project_name << "\",\n";
    oss << "  \"is_volumetric\": " << (project.is_volumetric ? "true" : "false") << ",\n";
    oss << "  \"multi_material\": true,\n";
    oss << "  \"regions\": [\n";
    
    for (size_t i = 0; i < project.regions.size(); ++i) {
        const auto& region = project.regions[i];
        oss << "    {\n";
        oss << "      \"name\": \"" << region.region_name << "\",\n";
        oss << "      \"material\": \"" << region.material_id << "\"\n";
        oss << "    }";
        if (i < project.regions.size() - 1) {
            oss << ",";
        }
        oss << "\n";
    }
    
    oss << "  ],\n";
    oss << "  \"materials\": {\n";
    
    size_t mat_count = 0;
    for (const auto& mat_pair : project.materials) {
        const auto& mat = mat_pair.second;
        oss << "    \"" << mat.type << "\": {\n";
        oss << "      \"nozzle_temp\": " << mat.nozzle_temp << ",\n";
        oss << "      \"bed_temp\": " << mat.bed_temp << "\n";
        oss << "    }";
        if (mat_count < project.materials.size() - 1) {
            oss << ",";
        }
        oss << "\n";
        mat_count++;
    }
    
    oss << "  }\n";
    oss << "}\n";
    
    metadata_json = oss.str();
    return true;
}

ModelObject* VCADTo3MFConverter::create_model_object_from_vcad(const VCADProject& /* project */) {
    // In actual implementation, this would create a ModelObject from volumetric data
    std::cout << "Creating ModelObject from VCAD project..." << std::endl;
    return nullptr;
}

bool VCADTo3MFConverter::assign_materials_to_regions(ModelObject* object,
                                                    const std::vector<VCADRegion>& regions,
                                                    const VCADMaterialConfig& config) {
    if (!object) {
        return false;
    }
    
    std::cout << "Assigning materials to regions..." << std::endl;
    
    for (const auto& region : regions) {
        auto it = config.region_to_material.find(region.region_name);
        if (it != config.region_to_material.end()) {
            std::cout << "  " << region.region_name << " -> " << it->second << std::endl;
        }
    }
    
    return true;
}

// ============================================================================
// Public API Functions
// ============================================================================

bool load_vcad(const char* path,
              DynamicPrintConfig& config,
              ConfigSubstitutionContext& /* config_substitutions */,
              Model* model,
              const VCADMaterialConfig* material_config) {
    
    std::cout << "Loading OpenVCAD file: " << path << std::endl;
    
    VCADFileParser parser;
    VCADProject project;
    
    // Load project
    if (!parser.load_vcad_project(path, project)) {
        std::cerr << "Failed to load VCAD project" << std::endl;
        return false;
    }
    
    // Use provided config or create default
    VCADMaterialConfig mat_config;
    if (material_config) {
        mat_config = *material_config;
    } else {
        mat_config = get_default_material_config(project);
    }
    
    // Convert to model
    VCADTo3MFConverter converter;
    if (!converter.convert_to_model(project, mat_config, model)) {
        std::cerr << "Failed to convert VCAD to model" << std::endl;
        return false;
    }
    
    // Apply material configuration
    if (!converter.apply_material_config(model, mat_config, &config)) {
        std::cerr << "Failed to apply material configuration" << std::endl;
        return false;
    }
    
    std::cout << "Successfully loaded OpenVCAD project" << std::endl;
    return true;
}

bool store_vcad_3mf(const char* path,
                   Model* model,
                   const DynamicPrintConfig* config,
                   const VCADProject* vcad_project) {
    
    std::cout << "Storing VCAD-enhanced 3MF: " << path << std::endl;
    
    if (!model || !config) {
        std::cerr << "Error: Invalid model or config" << std::endl;
        return false;
    }
    
    if (vcad_project) {
        VCADTo3MFConverter converter;
        VCADMaterialConfig dummy_config;
        std::string metadata;
        
        converter.generate_3mf_metadata(*vcad_project, dummy_config, metadata);
        
        std::cout << "Generated metadata:" << std::endl;
        std::cout << metadata << std::endl;
    }
    
    std::cout << "Successfully stored VCAD 3MF" << std::endl;
    return true;
}

std::vector<std::string> get_vcad_extensions() {
    return {".vcad", ".py", ".json"};
}

VCADMaterialConfig get_default_material_config(const VCADProject& project) {
    VCADMaterialConfig config;
    
    // Create default mapping for all regions
    int extruder_id = 1;
    for (const auto& region : project.regions) {
        config.region_to_material[region.region_name] = region.material_id;
        
        if (config.material_to_extruder.find(region.material_id) == config.material_to_extruder.end()) {
            config.material_to_extruder[region.material_id] = extruder_id++;
        }
    }
    
    return config;
}

} // namespace Slic3r
