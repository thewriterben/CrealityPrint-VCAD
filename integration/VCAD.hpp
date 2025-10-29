#ifndef slic3r_Format_VCAD_hpp_
#define slic3r_Format_VCAD_hpp_

#include <string>
#include <map>
#include <vector>

namespace Slic3r {

class Model;
class ModelObject;
struct ConfigSubstitutionContext;
class DynamicPrintConfig;

// Material properties for OpenVCAD integration
struct VCADMaterial {
    std::string name;
    std::string type;  // PLA, ABS, TPU, PETG, etc.
    int nozzle_temp;
    int bed_temp;
    std::string color;
    
    VCADMaterial() : nozzle_temp(200), bed_temp(60), color("default") {}
};

// Material region definition from OpenVCAD
struct VCADRegion {
    std::string region_name;
    std::string material_id;
    std::vector<float> bounds;  // Bounding box or volumetric definition
    
    VCADRegion() {}
    VCADRegion(const std::string& name, const std::string& mat_id) 
        : region_name(name), material_id(mat_id) {}
};

// OpenVCAD project information
struct VCADProject {
    std::string project_name;
    std::string project_path;
    std::string format;  // .vcad, .py, .json
    size_t file_size;
    
    std::vector<VCADRegion> regions;
    std::map<std::string, VCADMaterial> materials;
    
    // Metadata
    std::string openvcad_version;
    bool is_volumetric;
    bool has_gradients;
    
    VCADProject() : file_size(0), is_volumetric(true), has_gradients(false) {}
};

// Material configuration for multi-material printing
struct VCADMaterialConfig {
    std::map<std::string, std::string> region_to_material;  // region_name -> material_type
    std::map<std::string, int> material_to_extruder;        // material_type -> extruder_id
    
    VCADMaterialConfig() {}
};

/**
 * OpenVCAD File Parser
 * Handles loading and parsing of OpenVCAD project files (.vcad, .py, .json)
 */
class VCADFileParser {
public:
    VCADFileParser();
    ~VCADFileParser();
    
    /**
     * Check if the file is a valid OpenVCAD project
     */
    bool is_vcad_file(const std::string& filename);
    
    /**
     * Load OpenVCAD project metadata and structure
     */
    bool load_vcad_project(const std::string& filename, VCADProject& project);
    
    /**
     * Parse material configuration from JSON
     */
    bool parse_material_config(const std::string& json_content, VCADMaterialConfig& config);
    
    /**
     * Extract material regions from VCAD file
     */
    bool extract_regions(const std::string& vcad_content, std::vector<VCADRegion>& regions);
    
    /**
     * Get default material properties
     */
    VCADMaterial get_default_material(const std::string& material_type);
    
private:
    /**
     * Parse JSON content from .vcad file
     */
    bool parse_vcad_json(const std::string& content, VCADProject& project);
    
    /**
     * Parse Python-based OpenVCAD design (.py file)
     */
    bool parse_vcad_python(const std::string& filename, VCADProject& project);
    
    /**
     * Initialize default material library
     */
    void initialize_material_library();
    
    std::map<std::string, VCADMaterial> m_material_library;
};

/**
 * OpenVCAD to 3MF Converter
 * Converts OpenVCAD projects to 3MF format with multi-material support
 */
class VCADTo3MFConverter {
public:
    VCADTo3MFConverter();
    ~VCADTo3MFConverter();
    
    /**
     * Convert OpenVCAD project to Model object
     */
    bool convert_to_model(const VCADProject& project, 
                         const VCADMaterialConfig& config,
                         Model* model);
    
    /**
     * Apply material configuration to model
     */
    bool apply_material_config(Model* model, 
                              const VCADMaterialConfig& config,
                              DynamicPrintConfig* print_config);
    
    /**
     * Generate multi-material metadata for 3MF export
     */
    bool generate_3mf_metadata(const VCADProject& project,
                              const VCADMaterialConfig& config,
                              std::string& metadata_json);
    
private:
    /**
     * Create model object from volumetric data
     */
    ModelObject* create_model_object_from_vcad(const VCADProject& project);
    
    /**
     * Assign materials to model regions
     */
    bool assign_materials_to_regions(ModelObject* object,
                                    const std::vector<VCADRegion>& regions,
                                    const VCADMaterialConfig& config);
};

/**
 * Main API functions for OpenVCAD integration
 */

/**
 * Load OpenVCAD project file into Model
 * 
 * @param path Path to .vcad, .py, or .json file
 * @param config Print configuration to populate with material settings
 * @param config_substitutions Configuration substitution context
 * @param model Model object to populate with geometry
 * @param material_config Optional material configuration (if null, uses defaults)
 * @return true if successful, false otherwise
 */
extern bool load_vcad(const char* path, 
                     DynamicPrintConfig& config,
                     ConfigSubstitutionContext& config_substitutions,
                     Model* model,
                     const VCADMaterialConfig* material_config = nullptr);

/**
 * Export Model with OpenVCAD material data to 3MF
 * 
 * @param path Output 3MF file path
 * @param model Model object with geometry and materials
 * @param config Print configuration with material settings
 * @param vcad_project Original OpenVCAD project data (optional, for metadata)
 * @return true if successful, false otherwise
 */
extern bool store_vcad_3mf(const char* path,
                          Model* model,
                          const DynamicPrintConfig* config,
                          const VCADProject* vcad_project = nullptr);

/**
 * Get list of supported OpenVCAD file extensions
 */
extern std::vector<std::string> get_vcad_extensions();

/**
 * Get default material configuration for OpenVCAD project
 */
extern VCADMaterialConfig get_default_material_config(const VCADProject& project);

} // namespace Slic3r

#endif /* slic3r_Format_VCAD_hpp_ */
