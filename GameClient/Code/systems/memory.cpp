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
#include "memory.h"

namespace systems {
namespace memory {

MemoryAllocator::MemoryAllocator() : initialized_(false) {
}

void MemoryAllocator::Initialize(size_t max_size_bytes) {
  if (initialized_ == true) 
    return;
  max_size_bytes_ = max_size_bytes;
  base = VirtualAlloc((LPVOID)0,max_size_bytes,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE);
  next_free_unit = (uintptr_t)base;
  end = (uintptr_t)base + max_size_bytes;

  Unit* unit = (Unit*)next_free_unit;
  unit->flags = 0;
  unit->size_bytes = 0;

    
  InitializeCriticalSection(&cs);
  #ifdef _DEBUG
  debug::printf("Memory::Initialize ( base = 0x%016x )\n",base);
  #endif
  initialized_ = true;
}

void MemoryAllocator::Deinitialize() {
  if (initialized_ == false)
    return;
  VirtualFree(base,max_size_bytes_,MEM_DECOMMIT);
  VirtualFree(base,0,MEM_RELEASE);
  initialized_ = false;
}

void* MemoryAllocator::Allocate(size_t size_bytes) {
  static auto alloc_offset = [](const Unit* unit){
    return (uintptr_t)unit+sizeof(Unit);
  };
  EnterCriticalSection(&cs);
  bool used=false;
  Unit* unit = (Unit*)next_free_unit;

  if ( ((uintptr_t)unit+unit->size_bytes+size_bytes+(sizeof(Unit)<<1)) >= end) {
    return nullptr;
  }

  while (unit->size_bytes != 0) {
    if (unit->size_bytes < size_bytes || unit->flags != 0) {
      #ifdef _DEBUG
      debug::printf("Memory::Allocate\tsmaller block, moving to next ( size = %d , requested = %d )\n",unit->size_bytes,size_bytes);
      #endif
      if (alloc_offset(unit)+unit->size_bytes+size_bytes >= end) {
        return nullptr;
      }
      unit = (Unit*)(alloc_offset(unit)+unit->size_bytes);
        

    } else {
      break;
    }
  }

  unit->flags = 1;
  if ((intptr_t)(unit->size_bytes - size_bytes) > (intptr_t)(sizeof(Unit)+1)) {
    Unit* next_unit = (Unit*)((uintptr_t)unit+sizeof(Unit)+size_bytes);
    next_unit->flags = 0;
    next_unit->size_bytes = unit->size_bytes - size_bytes - sizeof(Unit);
    #ifdef _DEBUG
    debug::printf("Memory::Allocate\tcreating free block of %d bytes @ 0x%016x,0x%016x\n",next_unit->size_bytes,next_unit,alloc_offset(next_unit));
    #endif
  } else {

  }

  unit->size_bytes = size_bytes;
  #ifdef _DEBUG
  debug::printf("Memory::Allocate\tallocating %d bytes @ 0x%016x,0x%016x\n",unit->size_bytes,unit,alloc_offset(unit));
  #endif
  next_free_unit = alloc_offset(unit) + size_bytes;
  auto result = (void*)alloc_offset(unit);
  LeaveCriticalSection(&cs);
  return result;
}

void MemoryAllocator::Deallocate(void* pointer) {
  EnterCriticalSection(&cs);
  Unit* unit = (Unit*)((uintptr_t)pointer-sizeof(Unit));
  unit->flags = 0;
  next_free_unit = (uintptr_t)unit;
  #ifdef _DEBUG
  debug::printf("Memory::Deallocate\tdeallocating %d bytes @ %016x\n",unit->size_bytes,next_free_unit);
  #endif
  LeaveCriticalSection(&cs);
}


}
}