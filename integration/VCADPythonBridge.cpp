#include "VCADPythonBridge.hpp"

#ifdef VCAD_PYTHON_SUPPORT

#include <Python.h>
#include <iostream>
#include <sstream>

namespace Slic3r {

VCADPythonBridge::VCADPythonBridge() 
    : m_initialized(false), m_python_module(nullptr) {
}

VCADPythonBridge::~VCADPythonBridge() {
    shutdown();
}

bool VCADPythonBridge::initialize() {
    if (m_initialized) {
        return true;
    }
    
    std::cout << "Initializing Python bridge for OpenVCAD..." << std::endl;
    
    // Initialize Python interpreter
    Py_Initialize();
    
    if (!Py_IsInitialized()) {
        std::cerr << "Failed to initialize Python interpreter" << std::endl;
        return false;
    }
    
    // Add current directory to Python path
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('.')");
    
    // Try to import the OpenVCAD module
    PyObject* module_name = PyUnicode_FromString("openvcad_multimaterial_studio");
    PyObject* module = PyImport_Import(module_name);
    Py_DECREF(module_name);
    
    if (module == nullptr) {
        std::cerr << "Failed to import openvcad_multimaterial_studio module" << std::endl;
        PyErr_Print();
        return false;
    }
    
    m_python_module = module;
    m_initialized = true;
    
    std::cout << "Python bridge initialized successfully" << std::endl;
    return true;
}

void VCADPythonBridge::shutdown() {
    if (!m_initialized) {
        return;
    }
    
    if (m_python_module) {
        Py_DECREF(static_cast<PyObject*>(m_python_module));
        m_python_module = nullptr;
    }
    
    if (Py_IsInitialized()) {
        Py_Finalize();
    }
    
    m_initialized = false;
}

bool VCADPythonBridge::process_vcad_project(const std::string& project_path,
                                           const VCADMaterialConfig& config,
                                           const std::string& output_path) {
    if (!m_initialized) {
        std::cerr << "Python bridge not initialized" << std::endl;
        return false;
    }
    
    std::cout << "Processing VCAD project via Python: " << project_path << std::endl;
    
    PyObject* module = static_cast<PyObject*>(m_python_module);
    
    // Get the OpenVCADMultiMaterialStudio class
    PyObject* studio_class = PyObject_GetAttrString(module, "OpenVCADMultiMaterialStudio");
    if (!studio_class) {
        std::cerr << "Failed to get OpenVCADMultiMaterialStudio class" << std::endl;
        PyErr_Print();
        return false;
    }
    
    // Create instance
    PyObject* studio_instance = PyObject_CallObject(studio_class, nullptr);
    Py_DECREF(studio_class);
    
    if (!studio_instance) {
        std::cerr << "Failed to create OpenVCADMultiMaterialStudio instance" << std::endl;
        PyErr_Print();
        return false;
    }
    
    // Build material config dictionary
    PyObject* config_dict = PyDict_New();
    for (const auto& mapping : config.region_to_material) {
        PyObject* key = PyUnicode_FromString(mapping.first.c_str());
        PyObject* value = PyUnicode_FromString(mapping.second.c_str());
        PyDict_SetItem(config_dict, key, value);
        Py_DECREF(key);
        Py_DECREF(value);
    }
    
    // Call process_project method
    PyObject* process_method = PyObject_GetAttrString(studio_instance, "process_project");
    if (!process_method) {
        std::cerr << "Failed to get process_project method" << std::endl;
        PyErr_Print();
        Py_DECREF(config_dict);
        Py_DECREF(studio_instance);
        return false;
    }
    
    // Build arguments tuple
    PyObject* args = PyTuple_Pack(3,
        PyUnicode_FromString(project_path.c_str()),
        config_dict,
        PyUnicode_FromString(output_path.c_str())
    );
    
    // Call the method
    PyObject* result = PyObject_CallObject(process_method, args);
    
    Py_DECREF(args);
    Py_DECREF(config_dict);
    Py_DECREF(process_method);
    Py_DECREF(studio_instance);
    
    if (!result) {
        std::cerr << "Failed to process VCAD project" << std::endl;
        PyErr_Print();
        return false;
    }
    
    // Get result string
    const char* result_str = PyUnicode_AsUTF8(result);
    if (result_str) {
        std::cout << "Processing result: " << result_str << std::endl;
    }
    
    Py_DECREF(result);
    
    return true;
}

std::vector<std::string> VCADPythonBridge::get_available_materials() {
    std::vector<std::string> materials;
    
    if (!m_initialized) {
        return materials;
    }
    
    PyObject* module = static_cast<PyObject*>(m_python_module);
    
    // Create instance
    PyObject* studio_class = PyObject_GetAttrString(module, "OpenVCADMultiMaterialStudio");
    if (!studio_class) {
        return materials;
    }
    
    PyObject* studio_instance = PyObject_CallObject(studio_class, nullptr);
    Py_DECREF(studio_class);
    
    if (!studio_instance) {
        return materials;
    }
    
    // Get materials attribute
    PyObject* materials_dict = PyObject_GetAttrString(studio_instance, "materials");
    if (materials_dict && PyDict_Check(materials_dict)) {
        PyObject* keys = PyDict_Keys(materials_dict);
        Py_ssize_t size = PyList_Size(keys);
        
        for (Py_ssize_t i = 0; i < size; ++i) {
            PyObject* key = PyList_GetItem(keys, i);
            const char* material_name = PyUnicode_AsUTF8(key);
            if (material_name) {
                materials.push_back(material_name);
            }
        }
        
        Py_DECREF(keys);
    }
    
    if (materials_dict) {
        Py_DECREF(materials_dict);
    }
    Py_DECREF(studio_instance);
    
    return materials;
}

bool VCADPythonBridge::validate_material_config(const VCADMaterialConfig& config) {
    if (!m_initialized) {
        return false;
    }
    
    // Get available materials
    std::vector<std::string> available = get_available_materials();
    
    // Check if all configured materials are available
    for (const auto& mapping : config.region_to_material) {
        bool found = false;
        for (const auto& mat : available) {
            if (mat == mapping.second) {
                found = true;
                break;
            }
        }
        
        if (!found) {
            std::cerr << "Invalid material: " << mapping.second << std::endl;
            return false;
        }
    }
    
    return true;
}

std::string VCADPythonBridge::get_module_version() {
    if (!m_initialized) {
        return "unknown";
    }
    
    PyObject* module = static_cast<PyObject*>(m_python_module);
    
    PyObject* studio_class = PyObject_GetAttrString(module, "OpenVCADMultiMaterialStudio");
    if (!studio_class) {
        return "unknown";
    }
    
    PyObject* studio_instance = PyObject_CallObject(studio_class, nullptr);
    Py_DECREF(studio_class);
    
    if (!studio_instance) {
        return "unknown";
    }
    
    PyObject* version = PyObject_GetAttrString(studio_instance, "version");
    std::string version_str = "unknown";
    
    if (version) {
        const char* ver = PyUnicode_AsUTF8(version);
        if (ver) {
            version_str = ver;
        }
        Py_DECREF(version);
    }
    
    Py_DECREF(studio_instance);
    
    return version_str;
}

std::string VCADPythonBridge::config_to_python_dict(const VCADMaterialConfig& config) {
    std::ostringstream oss;
    oss << "{";
    
    bool first = true;
    for (const auto& mapping : config.region_to_material) {
        if (!first) {
            oss << ", ";
        }
        oss << "'" << mapping.first << "': '" << mapping.second << "'";
        first = false;
    }
    
    oss << "}";
    return oss.str();
}

bool VCADPythonBridge::execute_python(const std::string& code, std::string& result) {
    if (!m_initialized) {
        return false;
    }
    
    PyObject* main_module = PyImport_AddModule("__main__");
    PyObject* global_dict = PyModule_GetDict(main_module);
    
    PyObject* py_result = PyRun_String(code.c_str(), Py_eval_input, global_dict, global_dict);
    
    if (!py_result) {
        PyErr_Print();
        return false;
    }
    
    const char* result_str = PyUnicode_AsUTF8(py_result);
    if (result_str) {
        result = result_str;
    }
    
    Py_DECREF(py_result);
    return true;
}

} // namespace Slic3r

#endif // VCAD_PYTHON_SUPPORT
