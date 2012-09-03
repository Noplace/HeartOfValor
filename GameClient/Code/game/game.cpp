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
#include "../game_client.h"

using namespace DirectX;

#pragma pack(push,1)
struct CB_VS_PER_OBJECT {
  XMMATRIX m_mWorldViewProjection;
  XMMATRIX m_mWorld;
  XMVECTOR m_MaterialAmbientColor;
  XMVECTOR m_MaterialDiffuseColor;
};

struct CB_VS_PER_FRAME {
  XMVECTOR m_vLightDir;
  float    m_fTime;
  XMVECTOR m_LightDiffuse;
};
#pragma pack(pop)


struct SimpleVertex
{
    XMFLOAT3 pos;
    XMFLOAT4 color;
    XMFLOAT2 uv;
};


ID3D11SamplerState*         g_pSamLinear = NULL;
ID3D11Buffer*                       g_pcbVSPerObject11 = NULL;
ID3D11Buffer*                       g_pcbVSPerFrame11 = NULL;
ID3D11Buffer* vb;

systems::graphics::Camera cam;

int Game::Initialize(core::windows::Window* window) {
  Engine::Initialize(window);

  static const char* vs_shader = "D:\\Personal\\Projects\\HeartOfValor\\GameClient\\Content\\HLSL\\main_vs.cso";
  static const char* ps_shader = "D:\\Personal\\Projects\\HeartOfValor\\GameClient\\Content\\HLSL\\main_ps.cso";
  

  DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
  #if defined( _DEBUG )
    dwShaderFlags |= D3DCOMPILE_DEBUG;
  #endif

  uint8_t* vs_data;
  size_t vs_length;
  core::io::ReadWholeFileBinary(vs_shader,&vs_data,vs_length);
  gfx.device_->CreateVertexShader(vs_data,vs_length,NULL,&main_vs);

  uint8_t* ps_data;
  size_t ps_length;
  
  core::io::ReadWholeFileBinary(ps_shader,&ps_data,ps_length);
  gfx.device_->CreatePixelShader(ps_data,ps_length,NULL,&main_ps);


  // Create a layout for the object data
  const D3D11_INPUT_ELEMENT_DESC layout[] =
  {
      { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT   , 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      //{ "COLOR"   , 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
      //{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT      , 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
  };

  gfx.device_->CreateInputLayout(layout,ARRAYSIZE(layout),vs_data,vs_length,&input_layout_) ;
 

  core::io::DestroyFileBuffer(&vs_data);
  core::io::DestroyFileBuffer(&ps_data);


  
  cam.Initialize();
  cam.Perspective(gfx.width_,gfx.height_);
  
  XMVECTOR Eye = XMVectorSet( 0.0f, 3.0f, -6.0f, 0.0f );
  XMVECTOR At = XMVectorSet( 0.0f, 0.0f, 0.0f, 0.0f );
  XMVECTOR Up = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
  cam.view() = XMMatrixLookAtLH( Eye, At, Up );  
  //cam.Ortho2D(gfx.width_,gfx.height_);


  // Create state objects
  D3D11_SAMPLER_DESC samDesc;
  ZeroMemory( &samDesc, sizeof(samDesc) );
  samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samDesc.MaxAnisotropy = 1;
  samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samDesc.MaxLOD = D3D11_FLOAT32_MAX;
  gfx.device_->CreateSamplerState( &samDesc, &g_pSamLinear );
  



  // Create constant buffers
  D3D11_BUFFER_DESC cbDesc;
  ZeroMemory( &cbDesc, sizeof(cbDesc) );
  cbDesc.Usage = D3D11_USAGE_DYNAMIC;
  cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cbDesc.ByteWidth = sizeof( CB_VS_PER_OBJECT );


  CB_VS_PER_OBJECT mem;
  mem.m_mWorldViewProjection = cam.projection_transposed(); //XMMatrixTranspose(cam.matrix());
  mem.m_mWorld = cam.view_transposed();//XMMatrixIdentity();
  mem.m_MaterialDiffuseColor = XMVectorSet(1,1,1,1);
  mem.m_MaterialAmbientColor = XMVectorSet(1,1,1,1);
  D3D11_SUBRESOURCE_DATA data;
  ZeroMemory( &data, sizeof(data) );
  data.pSysMem = &mem;
  gfx.device_->CreateBuffer( &cbDesc, &data, &g_pcbVSPerObject11 );


  ZeroMemory( &cbDesc, sizeof(cbDesc) );
  cbDesc.Usage = D3D11_USAGE_DYNAMIC;
  cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cbDesc.ByteWidth = sizeof( CB_VS_PER_FRAME );
  gfx.device_->CreateBuffer( &cbDesc, NULL, &g_pcbVSPerFrame11);
  
 



   static SimpleVertex vertices[] =
  {
    { XMFLOAT3(  0.5f, 0.5f, 0.5f)},//XMFLOAT4( 1.0f,1.0f,1.0f,1.0f ), XMFLOAT2( 0.0f, 0.0f ) },
    { XMFLOAT3( 0.5f, -0.5f, 0.5f)},//XMFLOAT4( 1.0f,1.0f,1.0f,1.0f ), XMFLOAT2( 0.5f, 0.0f ) },
    { XMFLOAT3( -0.5f, -0.5f, 0.5f)},// XMFLOAT4( 1.0f,1.0f,1.0f,1.0f ),XMFLOAT2( 0.5f, 0.5f ) },
    { XMFLOAT3( -0.5f, -0.5f, 0.5f)},//XMFLOAT4( 1.0f,1.0f,1.0f,1.0f ), XMFLOAT2( 0.2f, 0.5f ) },
    };

  {

    D3D11_BUFFER_DESC vb_desc;
    ZeroMemory( &vb_desc, sizeof(vb_desc) );
    vb_desc.Usage = D3D11_USAGE_DEFAULT;
    vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vb_desc.CPUAccessFlags = 0;
    vb_desc.ByteWidth = sizeof( SimpleVertex ) * 4;

    D3D11_SUBRESOURCE_DATA vb_data;
    ZeroMemory( &vb_data, sizeof(vb_data) );
    vb_data.pSysMem = vertices;
    
    gfx.device_->CreateBuffer(&vb_desc,&vb_data,&vb);

    //engine_->gfx_context().device_context()->UpdateSubresource((ID3D11Resource*)g_vb.internal_pointer,0,NULL,vertices,sizeof(vertices),0);
  }

 
  gfx.context_->VSSetShader( main_vs, NULL, 0 );
  gfx.context_->PSSetShader( main_ps, NULL, 0 );
  gfx.context_->PSSetSamplers( 0, 1, &g_pSamLinear );




  return S_OK;
}


int Game::Deinitialize() {
  SafeRelease(&input_layout_);
  SafeRelease(&main_ps);
  SafeRelease(&main_vs);
  return Engine::Deinitialize();
}

int Game::Update(double dt) {

    /*HRESULT hr;
    D3D11_MAPPED_SUBRESOURCE MappedResource;
    gfx.context_->Map( g_pcbVSPerFrame11, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ;
    CB_VS_PER_FRAME* pVSPerFrame = ( CB_VS_PER_FRAME* )MappedResource.pData;
    pVSPerFrame->m_vLightDir = XMVectorSet( 0,0.707f,-0.707f,0 );
    pVSPerFrame->m_fTime = (float)0;
    pVSPerFrame->m_LightDiffuse =  XMVectorSet( 1.f, 1.f, 1.f, 1.f );
    gfx.context_->Unmap( g_pcbVSPerFrame11, 0 );
    gfx.context_->VSSetConstantBuffers( 1, 1, &g_pcbVSPerFrame11 );

    gfx.context_->Map( g_pcbVSPerObject11, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource ) ;
    CB_VS_PER_OBJECT* pVSPerObject = ( CB_VS_PER_OBJECT* )MappedResource.pData;
    pVSPerObject->m_mWorldViewProjection = XMMatrixTranspose(  cam.view() * cam.projection() );
    pVSPerObject->m_mWorld = XMMatrixIdentity();
    pVSPerObject->m_MaterialAmbientColor = XMVectorSet( 0.3f, 0.3f, 0.3f, 1.0f );
    pVSPerObject->m_MaterialDiffuseColor = XMVectorSet( 0.7f, 0.7f, 0.7f, 1.0f );
    gfx.context_->Unmap( g_pcbVSPerObject11, 0 );
    gfx.context_->VSSetConstantBuffers( 0, 1, &g_pcbVSPerObject11 );
    */
  
  gfx.context_->VSSetConstantBuffers( 0, 1, &g_pcbVSPerObject11 );
  gfx.context_->VSSetConstantBuffers( 1, 1, &g_pcbVSPerFrame11 );
  


  return S_OK;
}

int Game::Render() {
  float clearColor[4] = {0, 0, 0, 0};
  gfx.context_->ClearRenderTargetView(gfx.target_,clearColor);
  gfx.context_->ClearDepthStencilView(gfx.depth_stencil_,D3D11_CLEAR_DEPTH,1.0f,0);

  gfx.context_->IASetInputLayout( input_layout_ );
  gfx.context_->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
  UINT stride[] = {sizeof(SimpleVertex)};
  UINT offset[]={0};
  gfx.context_->IASetVertexBuffers(0,1,&vb,stride,offset);
  gfx.context_->Draw(3,0);

  gfx.swapchain_->Present(0,0);
  return S_OK;
}