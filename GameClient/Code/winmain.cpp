#include "game_client.h"
#include "systems\debug.h"
#include <new>

class Test {
 public:
  Test() {
    a = 0;
    a = a+1;
    a+=2;
  }
  void Test123() {
    a+=20;
  }
  ~Test() {
    a = 0;
  }

  int a;
};

systems::memory::MemoryAllocator mem;

int CALLBACK WinMain(
  _In_  HINSTANCE hInstance,
  _In_  HINSTANCE hPrevInstance,
  _In_  LPSTR lpCmdLine,
  _In_  int nCmdShow) {

  void* p[200];
  mem.Initialize(200*1024*1024);
  for (int i=0;i<200;++i)
    p[i] =mem.Allocate(1024*1024);


  mem.Deallocate(p[10]);
  mem.Allocate(20);
  mem.Allocate(200);
  
  mem.Deinitialize();

  return 0;
}