#include "ShaderObject.hpp"

#include <stdexcept>
#include <tuple>
#include "read_file_to_string.hpp"
#include "GLException.hpp"

ShaderObject::ShaderObject(GLenum type,
                           std::string const& source,
                           std::string const& origin)
    : type_   {type}
    , origin_ {origin}
{
    id_ = glCreateShader(type);
    if(id_ <= 0) {
        // TODO: use GL error handling properly
        throw std::runtime_error{"Could not allocate shader"};
    }

    char const* source_addr = &source[0];
    glShaderSource(id_, 1, &source_addr, NULL);
}

ShaderObject::ShaderObject(ShaderSource const& source)
    : ShaderObject{source.shader_type,
                   source.source,
                   "memory"}
{ }

ShaderObject::ShaderObject(ShaderFile const& file)
    : ShaderObject(file.shader_type,
                   read_file_to_string(file.path),
                   file.path)
{ }

ShaderObject::ShaderObject(ShaderObject&& that)
    : id_     {that.id_}
    , type_   {that.type_}
    , origin_ {std::move(that.origin_)}
{
    that.id_ = 0;
    that.type_ = GL_NONE;
}

ShaderObject& ShaderObject::operator=(ShaderObject&& that)
{
    ShaderObject temp(std::move(that));
    swap(*this,temp);
    return *this;
}

ShaderObject::~ShaderObject()
{
    glDeleteShader(id_);
}

void ShaderObject::compile()
{
    glCompileShader(id_);

    // Throw exception on error
    GLboolean status; std::string log;
    std::tie(status, log) = getCompileErrors();
    if (status == GL_FALSE) {
        throw GLShaderCompileError{log};
    }
}

std::pair<GLboolean, std::string> ShaderObject::getCompileErrors() const
{
    std::string error_string;

    GLint compile_status;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &compile_status);

    if (compile_status == GL_FALSE) {
        GLint log_length;
        glGetShaderiv(id_, GL_INFO_LOG_LENGTH, &log_length);
        error_string.resize(log_length);
        glGetShaderInfoLog(id_, log_length, NULL, &(error_string[0]));
    }

    return {compile_status, error_string};
}

std::string ShaderObject::getSource() const
{
    std::string source;
    GLint source_length;

    glGetShaderiv(id_, GL_SHADER_SOURCE_LENGTH, &source_length);
    source.resize(source_length);
    glGetShaderSource(id_, source_length, NULL, &(source[0]));

    return source;
}

void swap(ShaderObject& a, ShaderObject& b) noexcept
{
    using std::swap;

    swap(a.id_, b.id_);
    swap(a.type_, b.type_);
    swap(a.origin_, b.origin_);
}
