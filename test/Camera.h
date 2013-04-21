#ifndef CAMERA_H
#define CAMERA_H

#include "common_types.h"

class Camera
{
public:
    Camera();

    void GetViewMatrix( core::matrix4x4& matrix ) const;
    void GetProjectionMatrix ( core::matrix4x4& matrix ) const;

    void LookAt();
    void SetProjectionParams();

private:


};




#endif