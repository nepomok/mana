#include "engine/math/matrix.hpp"

#include <glm/glm.hpp>

namespace mana {
    glm::mat4 toGLM(const Matrix<float, 4, 4> &mat) {
        glm::mat4 ret;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                ret[x][y] = mat.get(x, y);
            }
        }
        return ret;
    }

    Matrix<float, 4, 4> toMana(const glm::mat4 &mat) {
        Matrix<float, 4, 4> ret;
        for (int x = 0; x < 4; x++) {
            for (int y = 0; y < 4; y++) {
                ret.set(x, y, mat[x][y]);
            }
        }
        return ret;
    }

    glm::vec4 toGLM(const Vec4f &vec) {
        return glm::vec4(vec.x, vec.y, vec.z, vec.w);
    }

    Vec4f toMana(const glm::vec4 &vec) {
        return Vec4f(vec.x, vec.y, vec.z, vec.w);
    }

    Vector4<float> operator*(const Matrix<float, 4, 4> &lhs, const Vector4<float> &rhs) {
        /*glm::mat4::col_type ret = toGLM(rhs) * toGLM(lhs);
        return toMana(ret);*/
        Vector4<float> ret;
        for (int row = 0; row < lhs.height(); row++) {
            float *out = nullptr;
            switch (row) {
                case 0:
                    out = &ret.x;
                    break;
                case 1:
                    out = &ret.y;
                    break;
                case 2:
                    out = &ret.z;
                    break;
                case 3:
                    out = &ret.w;
                    break;
                default:
                    assert(false);
            }

            assert(out != nullptr);

            for (int column = 0; column < lhs.width(); column++) {
                float in;
                switch (column) {
                    case 0:
                        in = rhs.x;
                        break;
                    case 1:
                        in = rhs.y;
                        break;
                    case 2:
                        in = rhs.z;
                        break;
                    case 3:
                        in = rhs.w;
                        break;
                    default:
                        assert(false);
                }

                *out += lhs.get(column, row) * in;
            }
        }
        return ret;
    }

    Matrix<float, 4, 4> operator*(const Matrix<float, 4, 4> &lhs, const Matrix<float, 4, 4> &rhs) {
        glm::mat4 lh = toGLM(lhs);
        glm::mat4 rh = toGLM(rhs);
        return toMana(lh * rh);
    }
}