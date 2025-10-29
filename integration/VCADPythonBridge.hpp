#ifndef slic3r_VCAD_Python_Bridge_hpp_
#define slic3r_VCAD_Python_Bridge_hpp_

#ifdef VCAD_PYTHON_SUPPORT

#include "VCAD.hpp"
#include <string>
#include <vector>

namespace Slic3r {

/**
 * Python Bridge for OpenVCAD Integration
 * Allows calling the Python-based OpenVCAD MultiMaterial Studio from C++
 */
class VCADPythonBridge {
public:
    VCADPythonBridge();
    ~VCADPythonBridge();
    
    /**
     * Initialize Python interpreter and import OpenVCAD module
     */
    bool initialize();
    
    /**
     * Shutdown Python interpreter
     */
    void shutdown();
    
    /**
     * Check if Python bridge is available
     */
    bool is_available() const { return m_initialized; }
    
    /**
     * Process OpenVCAD project using Python backend
     * This calls the Python openvcad_multimaterial_studio module
     */
    bool process_vcad_project(const std::string& project_path,
                             const VCADMaterialConfig& config,
                             const std::string& output_path);
    
    /**
     * Get list of available materials from Python module
     */
    std::vector<std::string> get_available_materials();
    
    /**
     * Validate material configuration using Python module
     */
    bool validate_material_config(const VCADMaterialConfig& config);
    
    /**
     * Get Python module version
     */
    std::string get_module_version();
    
private:
    /**
     * Execute Python code and get result
     */
    bool execute_python(const std::string& code, std::string& result);
    
    /**
     * Call Python function with arguments
     */
    bool call_python_function(const std::string& module_name,
                             const std::string& function_name,
                             const std::vector<std::string>& args,
                             std::string& result);
    
    /**
     * Convert VCADMaterialConfig to Python dict string
     */
    std::string config_to_python_dict(const VCADMaterialConfig& config);
    
    bool m_initialized;
    void* m_python_module;  // PyObject* in actual implementation
};

} // namespace Slic3r

#endif // VCAD_PYTHON_SUPPORT

#endif /* slic3r_VCAD_Python_Bridge_hpp_ */
