#pragma once

#include <stdexcept>

struct GLException : std::runtime_error {
    using std::runtime_error::runtime_error;
    using std::runtime_error::what;
};

struct GLShaderCompileError : public GLException {
    using GLException::GLException;
};
struct GLShaderLinkError : public GLException {
    using GLException::GLException;
};
