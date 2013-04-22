/////////////////////////////////////////////////////
/// @file DX11ShaderManager.h
///     DX11 shader cache mechanism
///
/// @author Dmitry Kolzov
///

#ifndef DX11SHADERMANAGER_H
#define DX11SHADERMANAGER_H

#include <D3D11.h>
#include <string>
#include <map>

class DX11ShaderProgram
{
public:
    DX11ShaderProgram(ID3D11VertexShader* vertexShader = nullptr, ID3D11PixelShader* pixelShader = nullptr, ID3D11InputLayout* inputLayout = nullptr);
    ~DX11ShaderProgram();

    ID3D11VertexShader* GetVertexShader() const;
    ID3D11PixelShader*  GetPixelShader() const;
    ID3D11InputLayout*  GetInputLayout() const;

private:
    //DX11ShaderProgram(DX11ShaderProgram const&);
    //DX11ShaderProgram& operator = (DX11ShaderProgram const&);
    ID3D11VertexShader* vertexShader_;
    ID3D11PixelShader*  pixelShader_;
    ID3D11InputLayout*  inputLayout_;
};

class DX11ShaderManager
{
public:
    DX11ShaderManager();
    ~DX11ShaderManager();

    /// REQUIRES: program base name (proper extension is added to the name before loading)
    /// PROMISE: cached program is returned if there is one in the cache
    ///          otherwise the binaries get loaded from disc and added to the cache
    ///          if there are no corresponding binaries exception is thrown
    ///          device parameter is needed when the program has not yet been cached
    DX11ShaderProgram const& GetShaderProgram(std::string const& name, ID3D11Device* device);

private:
    DX11ShaderManager(DX11ShaderManager const&);
    DX11ShaderManager& operator = (DX11ShaderManager const&);

    std::map<std::string, DX11ShaderProgram> shaderCache_;
};


#endif