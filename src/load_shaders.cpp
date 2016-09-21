#include "load_shaders.hpp"

#include <iterator>
#include <tuple>
#include <string>
#include <stdexcept>
#include <vector>
#include "ShaderFile.hpp"
#include "ShaderObject.hpp"
#include "ShaderProgram.hpp"
#include "GLException.hpp"

// XXX: debug
#include <iostream>

using ShaderObjectList = std::vector<ShaderObject>;
using ShaderFileList = std::vector<ShaderFile>;

ShaderObjectList load_shaders_from_files(ShaderFileList const&);
void compile_shaders(ShaderObjectList&);
void attach_shaders(ShaderProgram&, ShaderObjectList&);

ShaderProgram loadShaders(ShaderFileList const& file_list)
{
    if (file_list.empty()) {
        throw std::invalid_argument{
                "loadShaders passed empty shader list"};
    }

    ShaderObjectList shader_objects {load_shaders_from_files(file_list)};
    compile_shaders(shader_objects);

    ShaderProgram shader_program;
    attach_shaders(shader_program, shader_objects);
    shader_program.link();

    return shader_program;
}

ShaderObjectList load_shaders_from_files(ShaderFileList const& file_list)
{
    ShaderObjectList shader_objects;

    for (auto& file : file_list) {
        shader_objects.emplace_back(file);
    }

    return shader_objects;
}

void compile_shaders(ShaderObjectList& shader_objects)
{
    bool success = true;
    for (auto& o : shader_objects) {
        try {
            o.compile();
        }
        catch (GLShaderCompileError const& e) {
            std::cerr << "Compilation error in shader " << std::endl;
            std::cerr << std::string('#',60) << std::endl;
            std::cerr << e.what() << std::endl;
            success = false;
        }
    }
    std::cout << std::endl;
    if (!success) {
        throw GLShaderCompileError{"Shaders failed to compile"};
    }
}

void attach_shaders(ShaderProgram& program,
                    ShaderObjectList& objects)
{
    for (auto& o : objects) {
        program.attachObject(std::move(o));
    }
}
