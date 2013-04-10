#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <memory>
#include <iosfwd>

#include "common_types.h"

////////////////////////////////////
// This is just a holder for
// the geometry & animation data
class Model
{
public:
    ~Model();
    
    // REQUIRES: fileName exists on disk and is correct obj file
    // PROMISE: 
    static std::unique_ptr<Model>  CreateFromObj(std::string const& fileName);

    bool HasNormals() const;
    bool HasTexcoords() const;
    
private:
    struct Index
    {
        core::uint vIdx;
        core::uint nIdx;
        core::uint tIdx;
        Index() : vIdx(0xFFFFFFFF), nIdx(0xFFFFFFFF), tIdx(0xFFFFFFFF) {}
    };
    
    void LoadFromObjStream(std::ifstream& objStream);
    core::uint StoreVertex(core::vector3 const& v);
    core::uint StoreNormal(core::vector3 const& n);
    core::uint StoreTexcoord(core::vector2 const& t);
    core::uint StoreFace(std::vector<Index> const& indices);
    
    Model();
	Model(Model const&);
	Model& operator = (Model const&);
    
    std::vector<core::vector3> vertices_;
    std::vector<core::vector3> normals_;
    std::vector<core::vector2> texcoords_;
    
    
    
    std::vector<Index> indices_;
};

#endif 