/*****************************************************************************************************************
* Copyright (c) 2012 Khalid Ali Al-Kooheji                                                                       *
*                                                                                                                *
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and              *
* associated documentation files (the "Software"), to deal in the Software without restriction, including        *
* without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        *
* copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the       *
* following conditions:                                                                                          *
*                                                                                                                *
* The above copyright notice and this permission notice shall be included in all copies or substantial           *
* portions of the Software.                                                                                      *
*                                                                                                                *
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT          *
* LIMITED TO THE WARRANTIES OF MERCHANTABILITY, * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.          *
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, * DAMAGES OR OTHER LIABILITY,      *
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE            *
* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                                                         *
*****************************************************************************************************************/
#ifndef SYSTEMS_GRAPHICS_CAMERA_H
#define SYSTEMS_GRAPHICS_CAMERA_H

namespace systems {
namespace graphics {

class Camera {
 public:
 

  Camera() : ratio_(0) {

  }
  virtual ~Camera() {
    Deinitialize();
  }

  virtual int Initialize() {
    int hr = S_OK;
    return hr;
  }


  virtual int Deinitialize() {
    return S_OK;
  }

  void Ortho2D(float width,float height) {
    //RECT rect;
	  //GetClientRect(context()->window()->handle(),&rect);
    //D3DXMatrixOrthoLH(&Ortho2D, (FLOAT)rect.right, (FLOAT)rect.bottom, 0.0f, 1.0f);
    //D3DXMatrixOrthoOffCenterLH(&Ortho2D, 0.0f,(FLOAT)rect.right,(FLOAT)rect.bottom,0.0f,0.0f,1.0f);
    ratio_ = width/height;
    //D3DXMatrixOrthoOffCenterLH(&projection_, 0.0f,ratio,1.0f,0.0f,0.0f,1.0f);
    projection_ = DirectX::XMMatrixOrthographicOffCenterLH(0.0f,width,height,0.0f,-10000.0f,10000.0f);
	  //D3DXMatrixIdentity(&view_);
    view_ = DirectX::XMMatrixIdentity();
  }

  void Ortho2D(FLOAT left, FLOAT top, FLOAT right, FLOAT bottom) {
    projection_ = DirectX::XMMatrixOrthographicOffCenterLH(left,right,bottom,top,-10000.0f,10000.0f);
    view_ = DirectX::XMMatrixIdentity();
  }


  void Perspective(float width,float height) {
    ratio_ = width/height;
    projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4,ratio_, 0.01f, 100.0f);
    view_ = DirectX::XMMatrixIdentity();
  }


  float ratio() { return ratio_; }
  DirectX::XMMATRIX& projection() { return projection_;  }
  DirectX::XMMATRIX& view() { return view_;  }
  DirectX::XMMATRIX viewprojection() { return view_*projection_;  }
  DirectX::XMMATRIX projection_transposed() { return DirectX::XMMatrixTranspose(projection_);  }
  DirectX::XMMATRIX view_transposed() { return DirectX::XMMatrixTranspose(view_);  }
  DirectX::XMMATRIX matrix() { return view_*projection_; }
 private:
  DirectX::XMMATRIX world;
  DirectX::XMMATRIX view_;	
  DirectX::XMMATRIX projection_;
  float ratio_;
};

}
}

#endif