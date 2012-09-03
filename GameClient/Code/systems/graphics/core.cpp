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
#include "core.h"

#pragma comment(lib, "d3d11.lib")


namespace systems {
namespace graphics {

Core::Core() {
  memset(&gfx,0,sizeof(gfx));
}

Core::~Core() {

}

int Core::Initialize(core::windows::Window* window) {
  /*#ifdef _DEBUG
    char filename[MAX_PATH+10];
    GetModuleFileName( NULL, filename, MAX_PATH );
    auto index = strrchr(filename,'\\')-filename;
    //filename[index] = '\0';
    strcpy(filename+index,"\\Logs\\\0");
    log.Initialize(filename);
  #endif*/
  window_ = window;
  RECT dimensions;
  GetClientRect( window_->handle(), &dimensions );
  gfx.width_ = dimensions.right - dimensions.left;
  gfx.height_ = dimensions.bottom - dimensions.top;
  auto result = CreateDeviceResources();
  if (result == S_OK)
    result = CreateWindowSizeDependentResources();
  return result;
}

int Core::Deinitialize() {
  SafeRelease(&gfx.depth_stencil_);
  SafeRelease(&gfx.target_);
  SafeRelease(&gfx.swapchain_);
  SafeRelease(&gfx.context_);
  SafeRelease(&gfx.device_);

  return S_OK;
}


int Core::CreateDeviceResources() {
  D3D_DRIVER_TYPE driver_types[] = {
    D3D_DRIVER_TYPE_HARDWARE
  };
  D3D_FEATURE_LEVEL feature_levels[] = {
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1
  };
  uint32_t driver_types_count = ARRAYSIZE(driver_types);
  uint32_t feature_levels_count = ARRAYSIZE(feature_levels);

  uint32_t creation_flags = 0;//D3D11_CREATE_DEVICE_BGRA_SUPPORT|D3D11_CREATE_DEVICE_SINGLETHREADED;
  #ifdef _DEBUG
    creation_flags |= 0;//D3D11_CREATE_DEVICE_DEBUG;
  #endif
  HRESULT result;
DXGI_SWAP_CHAIN_DESC swapchain_desc;
    memset(&swapchain_desc,0,sizeof(swapchain_desc));
    swapchain_desc.BufferCount = 1;
    swapchain_desc.BufferDesc.Width = gfx.width_;
    swapchain_desc.BufferDesc.Height = gfx.height_;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_B8G8R8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM
    swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow = window_->handle();
    swapchain_desc.Windowed = true;
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.SampleDesc.Quality = 0;
  for (uint32_t driver=0;driver<driver_types_count;++driver) {
    result = D3D11CreateDeviceAndSwapChain(
                0,
                driver_types[driver],
                0,
                creation_flags,
                feature_levels,
                feature_levels_count,
                D3D11_SDK_VERSION,
                &swapchain_desc,
                &gfx.swapchain_,
                &gfx.device_,
                &gfx.feature_level_,
                &gfx.context_);
    if (SUCCEEDED(result)) {
      break;
    }
  }

  return result;
}

int Core::CreateWindowSizeDependentResources() {

	// Calculate the necessary swap chain and render target size in pixels.
  float windowWidth = gfx.width_;
  float windowHeight = gfx.height_;

	auto m_orientation = 0;
	bool swapDimensions =
		m_orientation == 0 ||
		m_orientation == 1;
	auto m_renderTargetSizeWidth = swapDimensions ? windowHeight : windowWidth;
	auto m_renderTargetSizeHeight = swapDimensions ? windowWidth : windowHeight;

  if(gfx.swapchain_ != nullptr)	{
		gfx.swapchain_->ResizeBuffers(2,
      static_cast<UINT>(m_renderTargetSizeWidth),
			static_cast<UINT>(m_renderTargetSizeHeight),
			DXGI_FORMAT_B8G8R8A8_UNORM,
			0);
	}	else{
    /*DXGI_SWAP_CHAIN_DESC swapchain_desc;
    memset(&swapchain_desc,0,sizeof(swapchain_desc));
    swapchain_desc.BufferCount = 2;
    swapchain_desc.BufferDesc.Width = gfx.width_;
    swapchain_desc.BufferDesc.Height = gfx.height_;
    swapchain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;//DXGI_FORMAT_B8G8R8A8_UNORM;//DXGI_FORMAT_R8G8B8A8_UNORM
    swapchain_desc.BufferDesc.RefreshRate.Numerator = 60;
    swapchain_desc.BufferDesc.RefreshRate.Denominator = 1;
    swapchain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapchain_desc.OutputWindow = window_->handle();
    swapchain_desc.Windowed = true;
    swapchain_desc.SampleDesc.Count = 1;
    swapchain_desc.SampleDesc.Quality = 0;
  


    IDXGIDevice * pDXGIDevice;
    auto result = gfx.device_->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);
    IDXGIAdapter * pDXGIAdapter;
    result = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);
    IDXGIFactory * pIDXGIFactory;
    pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIDXGIFactory);
    result = pIDXGIFactory->CreateSwapChain(gfx.device_,&swapchain_desc,&gfx.swapchain_);
    pIDXGIFactory->MakeWindowAssociation(window_->handle(),DXGI_MWA_NO_ALT_ENTER);
    SafeRelease(&pDXGIDevice);
    SafeRelease(&pDXGIAdapter);
    SafeRelease(&pIDXGIFactory);
    if( FAILED( result ) ) {
      return S_FALSE;
    }*/
    
  }


  ID3D11Texture2D* backbuffer_texture;
  auto result = gfx.swapchain_->GetBuffer(0,__uuidof(ID3D11Texture2D),(LPVOID*)&backbuffer_texture);
  if (FAILED(result)) {
    return S_FALSE;
  }
  result = gfx.device_->CreateRenderTargetView(backbuffer_texture,0,&gfx.target_);
  SafeRelease(&backbuffer_texture);
  if( FAILED( result ) ) {
    return S_FALSE;
  }

	CD3D11_TEXTURE2D_DESC depth_stencil_desc(
    DXGI_FORMAT_D24_UNORM_S8_UINT,
    static_cast<UINT>(m_renderTargetSizeWidth),
    static_cast<UINT>(m_renderTargetSizeHeight),
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL);

  
	/*ID3D11Texture2D* depth_stencil_texture;
  gfx.device_->CreateTexture2D(&depth_stencil_desc,nullptr,&depth_stencil_texture);
	CD3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc(D3D11_DSV_DIMENSION_TEXTURE2D);
  result = gfx.device_->CreateDepthStencilView(depth_stencil_texture,&depth_stencil_view_desc,&gfx.depth_stencil_);
  SafeRelease(&depth_stencil_texture);
  if( FAILED( result ) ) {
    return S_FALSE;
  }*/
  
  gfx.context_->OMSetRenderTargets(1,&gfx.target_,nullptr);
  
	// Set the rendering viewport to target the entire window.
	CD3D11_VIEWPORT viewport(0.0f,0.0f,m_renderTargetSizeWidth,m_renderTargetSizeHeight);
  gfx.context_->RSSetViewports(1, &viewport);

  return result;
}

}
}
