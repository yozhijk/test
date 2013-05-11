#include "DX11ShaderManager.h"
#include "utils.h"
#include <cassert>
#include <atlbase.h>

#define VS_EXTENSION "VS.fxo"
#define PS_EXTENSION "PS.fxo"

#define THROW_IF_FAILED(x,m) if((x)!=S_OK) throw std::runtime_error(m)


using namespace core;
using namespace std;

DX11ShaderProgram::DX11ShaderProgram(DX11ShaderProgram&& rhs)
{
    std::swap(vertexShader_, rhs.vertexShader_);
    std::swap(pixelShader_, rhs.pixelShader_);
    std::swap(inputLayout_, rhs.inputLayout_);
}

DX11ShaderProgram& DX11ShaderProgram::operator = (DX11ShaderProgram&& rhs)
{
    std::swap(vertexShader_, rhs.vertexShader_);
    std::swap(pixelShader_, rhs.pixelShader_);
    std::swap(inputLayout_, rhs.inputLayout_);

    return *this;
}

DX11ShaderManager::DX11ShaderManager()
{
}

DX11ShaderManager::~DX11ShaderManager()
{
    for (auto cIter = shaderCache_.cbegin(); cIter != shaderCache_.cend(); ++cIter)
    {
        cIter->second.GetVertexShader()->Release();
        cIter->second.GetPixelShader()->Release();
        cIter->second.GetInputLayout()->Release();
    }
}

DX11ShaderProgram const& DX11ShaderManager::GetShaderProgram(std::string const& name, ID3D11Device* device)
{
    auto cIter = shaderCache_.find(name);

    if (cIter != shaderCache_.end())
    {
        return cIter->second;
    }
    else
    {
        /// Load and create shaders
        string vertexShaderName = name + VS_EXTENSION;
        string pixelShaderName = name + PS_EXTENSION;

        std::vector<char> bytecode;

        load_file_contents(pixelShaderName, bytecode, true);

        CComPtr<ID3D11PixelShader> pPixelShader = nullptr;
        THROW_IF_FAILED(device->CreatePixelShader(&bytecode[0], bytecode.size(), nullptr, &pPixelShader), "Problem while loading pixel shader binaries");

        load_file_contents(vertexShaderName, bytecode, true);

        CComPtr<ID3D11VertexShader> pVertexShader = nullptr;
        THROW_IF_FAILED(device->CreateVertexShader(&bytecode[0], bytecode.size(), nullptr, &pVertexShader), "Problem while loading vertex shader binaries");

        ///Create default input layout, replace with IL customization in the future
        D3D11_INPUT_ELEMENT_DESC inputDesc[] = 
        {
            {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
            {"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
        };

        UINT inputDescElemCount = sizeof(inputDesc)/sizeof(D3D11_INPUT_ELEMENT_DESC);

        CComPtr<ID3D11InputLayout> pInputLayout = nullptr;
        THROW_IF_FAILED(device->CreateInputLayout(inputDesc, inputDescElemCount, &bytecode[0], bytecode.size(), &pInputLayout), "Failed to create input layout");

        shaderCache_[name] = DX11ShaderProgram(pVertexShader.Detach(), pPixelShader.Detach(), pInputLayout.Detach());

        return shaderCache_[name];
    }
}


DX11ShaderProgram::DX11ShaderProgram(ID3D11VertexShader* vertexShader, ID3D11PixelShader* pixelShader, ID3D11InputLayout* inputLayout) 
    : vertexShader_(vertexShader)
    , pixelShader_(pixelShader)
    , inputLayout_(inputLayout)
{
}

DX11ShaderProgram::~DX11ShaderProgram()
{

}

ID3D11VertexShader* DX11ShaderProgram::GetVertexShader() const
{
    return vertexShader_;
}

ID3D11PixelShader*  DX11ShaderProgram::GetPixelShader() const
{
    return pixelShader_;
}

ID3D11InputLayout*  DX11ShaderProgram::GetInputLayout() const
{
    return inputLayout_;
}

