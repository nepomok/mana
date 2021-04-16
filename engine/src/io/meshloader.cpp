#include <stdexcept>

#include "engine/render/vertex.hpp"
#include "engine/io/meshloader.hpp"

#include "extern/OBJ_Loader.h"

namespace mana {
    Vec3f toMana(objl::Vector3 vec) {
        return {vec.X, vec.Y, vec.Z};
    }

    Vec2f toMana(objl::Vector2 vec) {
        return {vec.X, vec.Y};
    }

    Mesh MeshLoader::load(const std::string &filepath) {
        objl::Loader loader;
        if (loader.LoadFile(filepath)) {
            Mesh ret;
            objl::Mesh objMesh = loader.LoadedMeshes[0];
            ret.indexed = !objMesh.Indices.empty();
            ret.indices = objMesh.Indices;
            for (auto &vert : objMesh.Vertices) {
                Vec2f tex = toMana(vert.TextureCoordinate);
                tex.y *= -1;
                ret.vertices.emplace_back(Vertex(toMana(vert.Position), toMana(vert.Normal), tex));
            }
            ret.primitive = TRI;
            return ret;
        } else {
            std::string error = "Failed to load obj file ";
            error.append(filepath);
            throw std::runtime_error(error);
        }
    }

    std::vector<Mesh> MeshLoader::loadMultiple(const std::string &filepath) {
        objl::Loader loader;
        if (loader.LoadFile(filepath)) {
            std::vector<Mesh> ret;
            for (auto &mesh : loader.LoadedMeshes) {
                Mesh r;
                r.indexed = !mesh.Indices.empty();
                r.indices = mesh.Indices;
                for (auto &vert : mesh.Vertices) {
                    Vec2f tex = toMana(vert.TextureCoordinate);
                    tex.y *= -1;
                    r.vertices.emplace_back(Vertex(toMana(vert.Position), toMana(vert.Normal), tex));
                }
                r.primitive = TRI;
                ret.emplace_back(r);
            }
            return ret;
        } else {
            std::string error = "Failed to load obj file ";
            error.append(filepath);
            throw std::runtime_error(error);
        }
    }
}