#pragma once

#include <GL/glew.h>
#include <string>
#include <istream>
#include "ShaderSource.hpp"
#include "ShaderFile.hpp"

class ShaderObject
{
    GLuint id_;
    GLenum type_;

    ShaderObject(GLenum type,
                 std::string const& source);
public:
    ShaderObject(std::istream const&, GLenum type);
    ShaderObject(ShaderSource const&);
    ShaderObject(ShaderFile const&);
    ShaderObject(ShaderObject&&);
    ShaderObject& operator=(ShaderObject&&);
    ~ShaderObject();

    void compile();

    std::pair<GLboolean, std::string> getCompileErrors() const;
    std::string getSource() const;
    GLenum getType() const { return type_; }

    operator GLuint() const { return id_; }
    friend void swap(ShaderObject&, ShaderObject&) noexcept;

    ShaderObject(ShaderObject const&) = delete;
    ShaderObject& operator=(ShaderObject const&) = delete;
};

void swap(ShaderObject&, ShaderObject&) noexcept;
