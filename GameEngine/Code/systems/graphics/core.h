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
#ifndef SYSTEMS_GRAPHICS_CORE_H
#define SYSTEMS_GRAPHICS_CORE_H

#include <WinCore/windows/windows.h>
#ifdef _DEBUG
#include <WinCore/log/log_manager.h>
#endif
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

namespace systems {
namespace graphics {

class Core {
 public:
  ID3D11Device* device_;
  ID3D11DeviceContext* context_;
  IDXGISwapChain* swapchain_;
  ID3D11RenderTargetView* target_;
  ID3D11DepthStencilView* depth_stencil_;

  Core();
  virtual ~Core();
  int Initialize(core::windows::Window* window);
  int Deinitialize();
  uint32_t width() { return width_; } 
  uint32_t height() { return height_; }
 protected:
  D3D_FEATURE_LEVEL feature_level_;
  uint32_t width_,height_;
  core::windows::Window* window_;
  int CreateDeviceResources();
  int CreateWindowSizeDependentResources();
};

}
}

#endif