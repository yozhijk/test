#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include <memory>
#include <iosfwd>

#include "common_types.h"

////////////////////////////////////
// This is just a holder for
// the geometry & animation data
class Mesh
{
public:
    ~Mesh();

    struct Vertex
    {
        core::vector3 position;
        core::vector2 texcoord;
        core::vector3 normal;
    };

    // REQUIRES: fileName exists on disk and is correct obj file
    // PROMISE: Will return valid model or the exception will be thrown
    static std::unique_ptr<Mesh>  CreateFromObj(std::string const& fileName);

    Vertex const* GetVertexArrayPointer() const;
    unsigned short const* GetIndexArrayPointer() const;
    core::uint GetVertexCount() const;
    core::uint GetIndexCount() const;
    core::uint GetVertexSizeInBytes() const;

private:
    struct Index
    {
        core::uint vIdx;
        core::uint nIdx;
        core::uint tIdx;
        Index() : vIdx(0xFFFFFFFF), nIdx(0xFFFFFFFF), tIdx(0xFFFFFFFF) {}
        bool operator < (Index const& other) const
        {
            return (vIdx < other.vIdx) ? true : 
                (vIdx > other.vIdx) ? false :
                (nIdx < other.nIdx) ? true :
                (nIdx > other.nIdx) ? false :
                (tIdx < other.tIdx) ? true : false;
        }
    };

    void PackInterleavedData();
    void LoadFromObjStream(std::ifstream& objStream);
    core::uint StoreVertex(core::vector3 const& v);
    core::uint StoreNormal(core::vector3 const& n);
    core::uint StoreTexcoord(core::vector2 const& t);
    core::uint StoreFace(std::vector<Index> const& indices);

    bool HasNormals() const;
    bool HasTexcoords() const;

    Mesh();
    Mesh(Mesh const&);
    Mesh& operator = (Mesh const&);

    std::vector<core::vector3> vertices_;
    std::vector<core::vector3> normals_;
    std::vector<core::vector2> texcoords_;
    std::vector<Index> indices_;

    std::vector<Vertex> interleavedData_;
    std::vector<unsigned short> interleavedIndices_;
};

#endif 