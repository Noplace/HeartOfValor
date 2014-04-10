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
#include "game_client.h"

#ifdef _DEBUG
void EnableCrashingOnCrashes() 
{ 
    typedef BOOL (WINAPI *tGetPolicy)(LPDWORD lpFlags); 
    typedef BOOL (WINAPI *tSetPolicy)(DWORD dwFlags); 
    const DWORD EXCEPTION_SWALLOWING = 0x1;

    HMODULE kernel32 = LoadLibraryA("kernel32.dll"); 
    tGetPolicy pGetPolicy = (tGetPolicy)GetProcAddress(kernel32, 
                "GetProcessUserModeExceptionPolicy"); 
    tSetPolicy pSetPolicy = (tSetPolicy)GetProcAddress(kernel32, 
                "SetProcessUserModeExceptionPolicy"); 
    if (pGetPolicy && pSetPolicy) 
    { 
        DWORD dwFlags; 
        if (pGetPolicy(&dwFlags)) 
        { 
            // Turn off the filter 
            pSetPolicy(dwFlags & ~EXCEPTION_SWALLOWING); 
        } 
    } 
}
#endif

class GameClient : public core::windows::Application {
 public: 
   MainWindow win;
   GameClient(HINSTANCE instance , LPSTR command_line, int show_command) : Application() {
    if (RanBefore("RPGDemo-GameClient") == true) {
      MessageBox(nullptr,"App already running","Error",MB_ICONWARNING|MB_OK);
      exit(0);
    }
    #ifdef _DEBUG
    EnableCrashingOnCrashes();
    #endif
    #ifndef _WIN64
      unsigned old_fp_state;
      _controlfp_s(&old_fp_state, _PC_53, _MCW_PC);
      //_controlfp(_RC_NEAR, _MCW_RC);
    #endif
    srand((unsigned int)time(0));
    CoInitializeEx(nullptr,COINIT_MULTITHREADED);
    win.Initialize();
   }
  ~GameClient() {

  }

  int Run() {
    MSG msg;
    do {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      } else {
        win.Step();
      }
    } while(msg.message!=WM_QUIT);
    return static_cast<int>(msg.wParam);
  }
};


int WINAPI runtut( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow );
int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
 
  //int test_padsynth();
  //test_padsynth();
  //fft_test();
  //samples_test();
  //void test_karplusStrong();
  //test_karplusStrong();

  GameClient app(hInstance,lpCmdLine,nShowCmd);
  //AddThreeAndPrint();
  return  app.Run();//runtut(hInstance,hPrevInstance,lpCmdLine,nShowCmd);//app.Run();
}