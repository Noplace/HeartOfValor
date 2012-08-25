#include <WinCore/windows/windows.h>

namespace systems {
namespace io {

VOID CALLBACK FileIOCompletionRoutine(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {

}


int asyncFile(const char* filename) {
  auto file_handle = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,nullptr,OPEN_EXISTING,FILE_FLAG_NO_BUFFERING|FILE_FLAG_OVERLAPPED,nullptr);

  if (file_handle == INVALID_HANDLE_VALUE)
    return S_FALSE;

  LARGE_INTEGER file_size;
  GetFileSizeEx(file_handle,&file_size);
  void* buf;
  OVERLAPPED overlapped;
  memset(&overlapped,0,sizeof(overlapped));
  ReadFileEx(file_handle,buf,200,&overlapped,FileIOCompletionRoutine);
  //ReadFileEx(

  return S_OK;
}


}
}