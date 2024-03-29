/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls 
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__ 
#define __USERPROG_KSYSCALL_H__ 

#include "kernel.h"
#include "thread.h"
#include "list.h"


void SysHalt()
{
  kernel->interrupt->Halt();
}


int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

int SysCreate(char *filename, int initialSize)
{
    return kernel->interrupt->CreateFile(filename, initialSize);
}
OpenFileId SysOpen(char *filename)
{
    return kernel->interrupt->OpenFile(filename);
}
int SysWrite(char *buffer, int size, OpenFileId id)
{
    return kernel->interrupt->WriteFile(buffer,size,id);
}
int SysClose(OpenFileId id){
    return kernel->interrupt->CloseFile(id);
}
int SysRead(char *buffer, int size, OpenFileId id)
{
    return kernel->interrupt->ReadFile(buffer,size,id);
}




#endif /* ! __USERPROG_KSYSCALL_H__ */
