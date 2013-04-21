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

class DX11ShaderManager
{
public:
    DX11ShaderManager();
    ~DX11ShaderManager();

private:
    DX11ShaderManager(DX11ShaderManager const&);
    DX11ShaderManager& operator = (DX11ShaderManager const&);
};


#endif