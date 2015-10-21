#pragma once

#include <GL/glew.h>
#include <string>
#include "ShaderSource.hpp"
#include "ShaderFile.hpp"

class ShaderObject
{
    GLuint id_;
    GLenum type_;
    std::string origin_;

    ShaderObject(GLenum type,
                 std::string const& source,
                 std::string const& origin);
public:
    ShaderObject(ShaderSource const&);
    ShaderObject(ShaderFile const&);
    ShaderObject(ShaderObject&&);
    ShaderObject& operator=(ShaderObject&&);
    ~ShaderObject();

    void compile();

    std::pair<GLboolean, std::string> getCompileErrors() const;
    std::string getSource() const;
    GLenum getType() const { return type_; }
    std::string const& getOrigin() const { return origin_; }

    operator GLuint() const { return id_; }
    friend void swap(ShaderObject&, ShaderObject&) noexcept;

    ShaderObject(ShaderObject const&) = delete;
    ShaderObject& operator=(ShaderObject const&) = delete;
};

void swap(ShaderObject&, ShaderObject&) noexcept;
