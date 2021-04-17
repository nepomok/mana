#include <stdexcept>

#include "render/opengl/oglshaderprogram.hpp"
#include "render/opengl/ogltypeconverter.hpp"

#include "engine/math/matrixmath.hpp"

#define SHADER_INCLUDE std::string("#include \"mana.glsl\"\n")

/**
 * The glsl source which is injected when SHADER_INCLUDE is found.
 */
const char *SHADER_INJECT = R"###(
#define MANA_MAX_LIGHTS 10

uniform mat4 MANA_M;
uniform mat4 MANA_V;
uniform mat4 MANA_P;
uniform mat4 MANA_MVP;

uniform vec3 MANA_VIEWPOS;

struct MANA_T_LIGHT_DIRECTIONAL {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MANA_T_LIGHT_DIRECTIONAL MANA_LIGHTS_DIRECTIONAL[MANA_MAX_LIGHTS];

uniform int MANA_LIGHT_COUNT_DIRECTIONAL;

struct MANA_T_LIGHT_POINT {
    vec3 position;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MANA_T_LIGHT_POINT MANA_LIGHTS_POINT[MANA_MAX_LIGHTS];

uniform int MANA_LIGHT_COUNT_POINT;

struct MANA_T_LIGHT_SPOT {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MANA_T_LIGHT_SPOT MANA_LIGHTS_SPOT[MANA_MAX_LIGHTS];

uniform int MANA_LIGHT_COUNT_SPOT;

vec4 MANA_F_CALCULATELIGHT_DIRECTIONAL(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    vec4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_DIRECTIONAL; i++)
    {
        vec3 ambient = MANA_LIGHTS_DIRECTIONAL[i].ambient * vec3(diffuseColor);

        vec3 norm = normalize(fNorm);
        vec3 lightDir = normalize(-MANA_LIGHTS_DIRECTIONAL[i].direction);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  MANA_LIGHTS_DIRECTIONAL[i].diffuse * vec3(diff * diffuseColor);

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_DIRECTIONAL[i].specular * vec3(spec * specularColor);

        ret += vec4(ambient + diffuse + specular, 1);
    }

    return ret;
}

vec4 MANA_F_CALCULATELIGHT_POINT(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    vec4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_POINT; i++)
    {
        float distance    = length(MANA_LIGHTS_POINT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_POINT[i].constant + MANA_LIGHTS_POINT[i].linear * distance + MANA_LIGHTS_POINT[i].quadratic * (distance * distance));

        vec3 ambient = MANA_LIGHTS_POINT[i].ambient * vec3(diffuseColor);

        vec3 norm = normalize(fNorm);
        vec3 lightDir = normalize(MANA_LIGHTS_POINT[i].position - fPos);

        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse =  MANA_LIGHTS_POINT[i].diffuse * vec3(diff * diffuseColor);

        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_POINT[i].specular * vec3(spec * specularColor);
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;

        ret += vec4(ambient + diffuse + specular, 1);
    }

    return ret;
}

vec4 MANA_F_CALCULATELIGHT_SPOT(vec3 fPos, vec3 fNorm, vec4 diffuseColor, vec4 specularColor, float roughness)
{
    vec4 ret;
    for (int i = 0; i < MANA_LIGHT_COUNT_SPOT; i++)
    {
        vec3 lightDir = normalize(MANA_LIGHTS_SPOT[i].position - fPos);

        // ambient
        vec3 ambient = MANA_LIGHTS_SPOT[i].ambient * diffuseColor.rgb;

        // diffuse
        vec3 norm = normalize(fNorm);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = MANA_LIGHTS_SPOT[i].diffuse * diff * diffuseColor.rgb;

        // specular
        vec3 viewDir = normalize(MANA_VIEWPOS - fPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), roughness);
        vec3 specular = MANA_LIGHTS_SPOT[i].specular * spec * specularColor.rgb;

        // spotlight (soft edges)
        float theta = dot(lightDir, normalize(-MANA_LIGHTS_SPOT[i].direction));
        float epsilon = (MANA_LIGHTS_SPOT[i].cutOff - MANA_LIGHTS_SPOT[i].outerCutOff);
        float intensity = clamp((theta - MANA_LIGHTS_SPOT[i].outerCutOff) / epsilon, 0.0, 1.0);
        diffuse  *= intensity;
        specular *= intensity;

        // attenuation
        float distance    = length(MANA_LIGHTS_SPOT[i].position - fPos);
        float attenuation = 1.0 / (MANA_LIGHTS_SPOT[i].constant + MANA_LIGHTS_SPOT[i].linear * distance + MANA_LIGHTS_SPOT[i].quadratic * (distance * distance));

        diffuse   *= attenuation;
        specular *= attenuation;

        vec3 result = ambient + diffuse + specular;
        ret += vec4(result, 1);
    }
    return ret;
}

vec4 MANA_F_CALCULATELIGHT(vec3 fPos, vec3 fNorm, vec4 fDiffuse, vec4 fSpecular, float roughness)
{
    return MANA_F_CALCULATELIGHT_DIRECTIONAL(fPos, fNorm, fDiffuse, fSpecular, roughness)
            + MANA_F_CALCULATELIGHT_POINT(fPos, fNorm, fDiffuse, fSpecular, roughness)
            + MANA_F_CALCULATELIGHT_SPOT(fPos, fNorm, fDiffuse, fSpecular, roughness);
}

)###";

namespace mana {
    namespace opengl {
        std::string preprocessShader(const std::string &shader) {
            auto index = shader.find(SHADER_INCLUDE);
            if (index != std::string::npos) {
                std::string start = shader.substr(0, index);
                std::string end = shader.substr(index + SHADER_INCLUDE.length());
                return start + SHADER_INJECT + end;
            }
            return shader;
        }

        OGLShaderProgram::OGLShaderProgram() : programID(0), vertexShader(), fragmentShader() {}

        OGLShaderProgram::OGLShaderProgram(const std::string &vertexShader, const std::string &fragmentShader)
                : vertexShader(vertexShader), fragmentShader(fragmentShader) {
            std::string vs = preprocessShader(vertexShader);
            std::string fs = preprocessShader(fragmentShader);

            const char *vertexSource = vs.c_str();
            const char *fragmentSource = fs.c_str();

            unsigned int vsH = glCreateShader(GL_VERTEX_SHADER);
            glShaderSource(vsH, 1, &vertexSource, NULL);
            glCompileShader(vsH);
            int success;
            glGetShaderiv(vsH, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(vsH, 512, NULL, infoLog);
                glDeleteShader(vsH);
                std::string error = "Failed to compile vertex shader: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }

            unsigned int fsH = glCreateShader(GL_FRAGMENT_SHADER);
            glShaderSource(fsH, 1, &fragmentSource, NULL);
            glCompileShader(fsH);
            glGetShaderiv(fsH, GL_COMPILE_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetShaderInfoLog(fsH, 512, NULL, infoLog);
                glDeleteShader(vsH);
                glDeleteShader(fsH);
                std::string error = "Failed to compile fragment shader: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }

            programID = glCreateProgram();
            glAttachShader(programID, vsH);
            glAttachShader(programID, fsH);
            glLinkProgram(programID);

            glDeleteShader(vsH);
            glDeleteShader(fsH);

            glGetProgramiv(programID, GL_LINK_STATUS, &success);
            if (!success) {
                char infoLog[512];
                glGetProgramInfoLog(programID, 512, NULL, infoLog);
                std::string error = "Failed to compile shader program: ";
                error.append(infoLog);
                throw std::runtime_error(error);
            }
        }

        OGLShaderProgram::~OGLShaderProgram() {
            glDeleteProgram(programID);
        }

        void OGLShaderProgram::activate() {
            glUseProgram(programID);
        }

        void OGLShaderProgram::setBool(const std::string &name, bool value) {
            glUniform1i(glGetUniformLocation(programID, name.c_str()), (int) value);
        }

        void OGLShaderProgram::setInt(const std::string &name, int value) {
            glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
        }

        void OGLShaderProgram::setFloat(const std::string &name, float value) {
            glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
        }

        void OGLShaderProgram::setVec2(const std::string &name, const Vec2b &value) {
            glUniform2i(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
        }

        void OGLShaderProgram::setVec2(const std::string &name, const Vec2i &value) {
            glUniform2i(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
        }

        void OGLShaderProgram::setVec2(const std::string &name, const Vec2f &value) {
            glUniform2f(glGetUniformLocation(programID, name.c_str()), value.x, value.y);
        }

        void OGLShaderProgram::setVec3(const std::string &name, const Vec3b &value) {
            glUniform3i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
        }

        void OGLShaderProgram::setVec3(const std::string &name, const Vec3i &value) {
            glUniform3i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
        }

        void OGLShaderProgram::setVec3(const std::string &name, const Vec3f &value) {
            glUniform3f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z);
        }

        void OGLShaderProgram::setVec4(const std::string &name, const Vec4b &value) {
            glUniform4i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void OGLShaderProgram::setVec4(const std::string &name, const Vec4i &value) {
            glUniform4i(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void OGLShaderProgram::setVec4(const std::string &name, const Vec4f &value) {
            glUniform4f(glGetUniformLocation(programID, name.c_str()), value.x, value.y, value.z, value.w);
        }

        void OGLShaderProgram::setMat2(const std::string &name, const Mat2f &value) {
            throw std::runtime_error("Not Implemented");
        }

        void OGLShaderProgram::setMat3(const std::string &name, const Mat3f &value) {
            throw std::runtime_error("Not Implemented");
        }

        void OGLShaderProgram::setMat4(const std::string &name, const Mat4f &value) {
            glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, (GLfloat *)&value);
        }
    }
}