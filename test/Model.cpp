//
//  Model.cpp
//  test
//
//  Created by Dmitry Kozlov on 08.04.13.
//  Copyright (c) 2013 Dmitry Kozlov. All rights reserved.
//

#include "Model.h"
#include <fstream>
#include <sstream>
#include <cassert>
#include <algorithm>
#include <string>
#include <iterator>
#include <map>

// get rid of crazy ms "switch to non-portable sscanf_s" warning
#pragma warning(push)
#pragma warning(disable:4996)

using namespace std;
using namespace core;


Model::Model()
{
}
    
Model::~Model()
{
}

void Model::LoadFromObjStream(ifstream& objStream)
{
    string line;
    while(getline(objStream,line))
    {
        // normal
        if (line.find("vn") != string::npos)
        {
            istringstream inStream(line);
            string tmp;
            real x,y,z;
            
            inStream >> tmp >> x >> y >> z;
            
            StoreNormal(vector3(x,y,z));
        }
        // texcoord
        else if (line.find("vt") != string::npos)
        {
            istringstream inStream(line);
            string tmp;
            real x,y;
            
            inStream >> tmp >> x >> y;
            
            StoreTexcoord(vector2(x,y));
        }
        // vertex
        else if (line.find("v") != string::npos)
        {
            istringstream inStream(line);
            string tmp;
            real x,y,z;
            
            inStream >> tmp >> x >> y >> z;
            
            StoreVertex(vector3(x,y,z));
        }
        // no support for groups, materials etc yet
    }
    
    // second pass to extract faces
    objStream.clear();
    objStream.seekg(0, ios::beg);
    
    std::vector<string> tokens;
    while(getline(objStream,line))
    {
        if (line.find("f") != string::npos)
        {
            istringstream inStream(line);
            copy(istream_iterator<string>(inStream), istream_iterator<string>(), back_inserter(tokens));
                      
            // handle tokens only triangles for the moment
            assert(tokens.size() == 4);
            tokens.erase(tokens.begin());
            
            // remove extra spaces
            for_each(tokens.begin(), tokens.end(),
                          [&](string& s)
                          {
                              s.erase(remove(s.begin(), s.end(), ' '), s.end());
                          }
                          );
            
            // reserve memory for a triangle
            std::vector<Index> indices;
            indices.reserve(3);
            Index tempIdx;
            
            // There are 4 possible cases here
            // case 1: f 1 2 3
            // case 2: f 1/1 2/2 3/3
            // case 3: f 1/1/1 2/2/2 3/3/3
            // case 4: f 1//1 2//2 3//3
            if (tokens[0].find_first_of("//") != string::npos)
            {
                // case 4:
                // scanf seems to be the fastest option here
                for_each(tokens.begin(), tokens.end(),
                         [&](std::string& s)
                         {
                             sscanf(s.c_str(), "%d//%d", &tempIdx.vIdx, &tempIdx.nIdx);
							 // account for 1-based arrays in obj file
							 --tempIdx.vIdx;
							 --tempIdx.nIdx;
                             indices.push_back(tempIdx);
                         }
                         );
            }
            else if (count(tokens[0].begin(), tokens[0].end(), '/') == 1)
            {
                // case 2:
                // case 4:
                // scanf seems to be the fastest option here
                for_each(tokens.begin(), tokens.end(),
                         [&](std::string& s)
                         {
                             sscanf(s.c_str(), "%d/%d", &tempIdx.vIdx, &tempIdx.tIdx);
							 --tempIdx.vIdx;
							 --tempIdx.tIdx;
                             indices.push_back(tempIdx);
                         }
                         );
            }
            else if (count(tokens[0].begin(), tokens[0].end(), '/') == 2)
            {
                // case 3:
                // scanf seems to be the fastest option here
                for_each(tokens.begin(), tokens.end(),
                         [&](std::string& s)
                         {
                             sscanf(s.c_str(), "%d/%d/%d", &tempIdx.vIdx, &tempIdx.tIdx, &tempIdx.nIdx);
							 --tempIdx.vIdx;
							 --tempIdx.nIdx;
							 --tempIdx.tIdx;
                             indices.push_back(tempIdx);
                         }
                         );
            }
            else
            {
                // case 1:
                // scanf seems to be the fastest option here
                for_each(tokens.begin(), tokens.end(),
                         [&](std::string& s)
                         {
                             sscanf(s.c_str(), "%d", &tempIdx.vIdx);
							 --tempIdx.vIdx;
                             indices.push_back(tempIdx);
                         }
                         );
            }
                

            StoreFace(indices);
            
            indices.clear();
            tokens.clear();
        }
    }
    
    PackInterleavedData();
}

inline uint Model::StoreVertex(vector3 const& v)
{
    vertices_.push_back(v);
    return static_cast<uint>(vertices_.size() - 1);
}

inline uint Model::StoreNormal(vector3 const& n)
{
    normals_.push_back(n);
    return static_cast<uint>(normals_.size() - 1);
}

inline uint Model::StoreTexcoord(vector2 const& t)
{
    texcoords_.push_back(t);
    return static_cast<uint>(texcoords_.size() - 1);
}

inline uint Model::StoreFace(std::vector<Index> const& indices)
{
    // Add support for n-gons later
    assert(indices.size() == 3);
    copy(indices.begin(), indices.end(), back_inserter(indices_));
    return static_cast<uint>(indices_.size() - 1);
}

unique_ptr<Model>  Model::CreateFromObj(string const& fileName)
{
    ifstream in(fileName);
    
    if (in)
    {
        unique_ptr<Model> modelPtr(new Model());
        modelPtr->LoadFromObjStream(in);
        return modelPtr;
    }
    else
    {
        throw std::runtime_error("Shit happens: check the path to your models");
        return nullptr;
    }
}

Model::Vertex const* Model::GetVertexArrayPointer() const
{
    assert(interleavedData_.size() > 0);
    return &interleavedData_[0];
}

unsigned short const* Model::GetIndexArrayPointer() const
{
    assert(interleavedIndices_.size() > 0);
    return &interleavedIndices_[0];
}

core::uint Model::GetVertexCount() const
{
    assert(interleavedData_.size() > 0);
    return static_cast<uint>(interleavedData_.size());
}

core::uint Model::GetIndexCount() const
{
    assert(interleavedIndices_.size() > 0);
    return static_cast<uint>(interleavedIndices_.size());
}

core::uint Model::GetVertexSizeInBytes() const
{
    return sizeof(Vertex);
}

void Model::PackInterleavedData()
{
    map<Index, unsigned short> tempIdx;
    for_each(indices_.begin(), indices_.end(), [&](Index const& index)
             {
                 auto idx = tempIdx.find(index);
                 
                 if ( idx != tempIdx.end())
                 {
                     interleavedIndices_.push_back(idx->second);
                 }
                 else
                 {
                     Vertex vertexData;
                     
                     vertexData.position = vertices_[index.vIdx];
                     
                     if (HasNormals())
                         vertexData.normal = normals_[index.nIdx];
                     
                     if (HasTexcoords())
                         vertexData.texcoord = texcoords_[index.tIdx];
                    
                     interleavedData_.push_back(vertexData);
                     
                     unsigned short indexToStore = static_cast<unsigned short>(interleavedData_.size()-1);
                     
                     interleavedIndices_.push_back(indexToStore);
                     
                     tempIdx[index] = indexToStore;
                 }
             }
             );
    
}

inline bool Model::HasNormals() const
{
    return !normals_.empty();
}

inline bool Model::HasTexcoords() const
{
    return !texcoords_.empty();
}

#pragma warning(pop)