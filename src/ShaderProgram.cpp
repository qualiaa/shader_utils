#include "ShaderProgram.hpp"

#include <algorithm>
#include <functional>
#include <tuple>
#include <vector>
#include "GLException.hpp"

ShaderProgram::ShaderProgram()
    : id_{glCreateProgram()}
{
}

template <typename ForwardIt>
ShaderProgram::ShaderProgram(std::move_iterator<ForwardIt> start,
                             std::move_iterator<ForwardIt> end)
    : ShaderProgram{}
{
    static_assert(
        std::is_same<typename ForwardIt::value_type, ShaderObject>::value,
        "Must contain objects of type ShaderObject"
    );

    std::for_each(start, end, [this](auto&& x) { attachShader(std::move(x)); });
}

ShaderProgram::ShaderProgram(ShaderProgram&& that)
    : id_{that.id_}
    , attached_shaders_{that.attached_shaders_}
{
    that.id_ = 0;
    that.attached_shaders_.clear();
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& that)
{
    ShaderProgram temp(std::move(that));
    swap(*this,temp);
    return *this;
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id_);
}

void ShaderProgram::attachShader(ShaderObject&& so)
{
    attached_shaders_.push_back(std::make_shared<ShaderObject>(std::move(so)));
    glAttachShader(id_, *attached_shaders_.back());
}

void ShaderProgram::attachShader(ShaderPtr const& p)
{
    auto beg = attached_shaders_.begin();
    auto end = attached_shaders_.end();

    auto res = std::find_if(beg, end,
            [&p] (ShaderPtr const& x) {
                return p.get() == x.get();
            });

    if (res == end) {
        glAttachShader(id_, *attached_shaders_.back());
        attached_shaders_.push_back(p);
    }
}

void ShaderProgram::detachShader(ShaderPtr const& p) {
    auto beg = attached_shaders_.begin();
    auto end = attached_shaders_.end();

    auto res =
        std::remove_if(beg, end,
            [&p] (ShaderPtr const& x) {
                return p.get() == x.get();
            });

    if (res != end) {
        glDetachShader(id_, *p);
        attached_shaders_.erase(res,end);
    }
}

void ShaderProgram::link()
{
    glLinkProgram(id_);

    // Throw exception on error
    GLboolean status; std::string log;
    std::tie(status, log) = getLinkErrors();
    if (status == GL_FALSE) {
        throw GLShaderLinkError{log};
    }
}

std::pair<GLboolean, std::string> ShaderProgram::getLinkErrors() const
{
    std::string error_string;

    GLint link_status;
    glGetProgramiv(id_, GL_LINK_STATUS, &link_status);

    if (link_status == GL_FALSE) {
        GLint link_log_length;
        glGetProgramiv(id_, GL_INFO_LOG_LENGTH, &link_log_length);
        error_string.resize(link_log_length);
        glGetProgramInfoLog(id_, link_log_length, NULL, &(error_string[0]));
    }

    return {link_status, error_string};
}

void swap(ShaderProgram& a, ShaderProgram& b) noexcept
{
    using std::swap;

    swap(a.id_, b.id_);
    swap(a.attached_shaders_, b.attached_shaders_);
}
