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

bool key_down[256];
int key_state[256];

const char key_maps[10] = {
  'Z','X','C','V','B','N','M','<','>','?'
};

MainWindow::MainWindow() : core::windows::Window() {
  memset(&key_down,0,sizeof(key_down));
  memset(&key_state,0,sizeof(key_state));

}

MainWindow::~MainWindow() {

}

void MainWindow::Initialize() {
  PrepareClass("Game Client");
  this->Create();
  SetClientSize(640,480);
  Center();

  //SetMenu(LoadMenu(NULL,MAKEINTRESOURCE(IDR_MAINMENU)));

  game_instance.Initialize(this);

  Show();
  memset(&timing,0,sizeof(timing));
  timer_.Calibrate();
  timing.prev_cycles = timer_.GetCurrentCycles();
  //SetThreadAffinityMask(GetCurrentThread(),
}

void MainWindow::Step() {
  //synth_player_->Tick();
  //Sleep(100);
  const double dt =  1000.0 / 60.0;//16.667f;
  timing.current_cycles = timer_.GetCurrentCycles();
  double time_span =  (timing.current_cycles - timing.prev_cycles) * timer_.resolution();
  if (time_span > 250.0) //clamping time
    time_span = 250.0;

  timing.span_accumulator += time_span;
  while (timing.span_accumulator >= dt) {
    timing.span_accumulator -= dt;
    game_instance.Update(dt);
    ++timing.fps_counter;
  }

  timing.total_cycles += timing.current_cycles-timing.prev_cycles;
  timing.prev_cycles = timing.current_cycles;

   
  if (timing.render_time_span >= 16.667) {
    game_instance.Render();
    timing.render_time_span = 0;
  }

  timing.fps_time_span += time_span;
  timing.render_time_span += time_span;
  
  if (timing.fps_time_span >= 1000.0) {
    timing.fps = timing.fps_counter;
    timing.fps_counter = 0;
    timing.fps_time_span = 0;

    //char caption[256];
    //sprintf(caption,"CPS: %llu ",nes.cycles_per_second());
    //sprintf_s(caption,"FPS: %02d",timing.fps);
    //SetWindowText(handle(),caption);
 
  }
  

}

int MainWindow::OnCreate(WPARAM wParam,LPARAM lParam) {
  return 0;
}

int MainWindow::OnDestroy(WPARAM wParam,LPARAM lParam) {
  game_instance.Deinitialize();
  PostQuitMessage(0);
  return 0;
}

int MainWindow::OnKeyDown(WPARAM wParam,LPARAM lParam) {
  if (wParam == 'A') {
    
  }

  if (wParam == 'S') {
    
  }

  if (wParam == 'D') {
    
  }

  if (wParam == 'F') {

  }

  key_down[wParam&0xFF] = true;
  if (wParam == 'Z') {
    
  }


  return 0;
}

int MainWindow::OnKeyUp(WPARAM wParam,LPARAM lParam) {
  key_down[wParam&0xFF] = false;

  return 0;
}

int MainWindow::OnActivate(WPARAM wParam,LPARAM lParam) {
  if ((wParam&0xFFFF) == WA_INACTIVE) {

  } else {

  }
  return 0;
}