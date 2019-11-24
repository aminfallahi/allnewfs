// exception.cc 
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.  
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2. 
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions 
//	is in machine.h.
//----------------------------------------------------------------------


// void Exit_POS(ThreadId id) { //only could exit currentThread
	// if (kernel->currentThread->PID != id) {
		// cerr << "Invalid Thread ID isn't the current Thread, can't exit it!" << endl;
		// kernel->currentThread->Finish();
	// }
	// else {
		// cout<<"Thread with PID "<<kernel->currentThread->PID<<" is going to finish!"<<endl;
		// kernel->currentThread->Finish();
	// }
// }

void
ExceptionHandler(ExceptionType which)
{
    int type = kernel->machine->ReadRegister(2);

    DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

    switch (which) {
	// Task1
    case SyscallException:
      switch(type) {
      	case SC_Halt:
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
			SysHalt();
            cout<<"in exception\n";
			ASSERTNOTREACHED();
			break;
		case SC_MSG:
			DEBUG(dbgSys, "Message received.\n");
			val = kernel->machine->ReadRegister(4);
			{
			char *msg = &(kernel->machine->mainMemory[val]);
			cout << msg << endl;
			}
			SysHalt();
			ASSERTNOTREACHED();
			break;
		case SC_Create:{
            val = kernel->machine->ReadRegister(4);
            {
            char *filename = &(kernel->machine->mainMemory[val]);
			// need to add initial size
			int initialSize = kernel->machine->ReadRegister(5);	
            status = SysCreate(filename,initialSize);
            kernel->machine->WriteRegister(2, (int) status);
            }
            kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
            kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
            kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
            return;
            ASSERTNOTREACHED();
                    break;
        }
		case SC_Open:{
            val = kernel->machine->ReadRegister(4);
            {
            char *filename = &(kernel->machine->mainMemory[val]);
            OpenFileId id = SysOpen(filename);
            kernel->machine->WriteRegister(2,id);
            }
            kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
                        kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
                        kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
            return;
            ASSERTNOTREACHED();
            break;
        }
		case SC_Write:{
            val = kernel->machine->ReadRegister(4);
            char *buffer = &(kernel->machine->mainMemory[val]) ;
            int size = kernel->machine->ReadRegister(5);
            OpenFileId id = kernel->machine->ReadRegister(6);
            {
            int num = SysWrite(buffer,size,id);
            kernel->machine->WriteRegister(2,num);
            }
            kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
            kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
            kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
            return;
            ASSERTNOTREACHED();
            break;
        }
		case SC_Close:{
            val = kernel->machine->ReadRegister(4);
            {
            int result = SysClose(val);
            kernel->machine->WriteRegister(2,result);
            }
            kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
            kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
            kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
            return;
            ASSERTNOTREACHED();
            break;
        }
		case SC_Read:{
            val = kernel->machine->ReadRegister(4);
            char *buffer = &(kernel->machine->mainMemory[val]) ;//from buffer
            int size = kernel->machine->ReadRegister(5);    //"size" bytes
            OpenFileId id = kernel->machine->ReadRegister(6);
            {
            int num = SysRead(buffer,size,id);
            kernel->machine->WriteRegister(2,num);
            }
            kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
            kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
            kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
            return;
            ASSERTNOTREACHED();
            break;
        }
      	case SC_Add:
			DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");
			/* Process SysAdd Systemcall*/
			int result;
			result = SysAdd(/* int op1 */(int)kernel->machine->ReadRegister(4),
			/* int op2 */(int)kernel->machine->ReadRegister(5));
			DEBUG(dbgSys, "Add returning with " << result << "\n");
			/* Prepare Result */
			kernel->machine->WriteRegister(2, (int)result);	
			/* Modify return point */
			{
	  		/* set previous programm counter (debugging only)*/
	  		kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));
	  			
			/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	  		kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);
	  
	 		/* set next programm counter for brach execution */
	 		kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg)+4);
			}
			cout << "result is " << result << "\n";	
			return;	
			ASSERTNOTREACHED();
            break;
		case SC_Exit:
			DEBUG(dbgAddr, "Program exit\n");
            val=kernel->machine->ReadRegister(4);
            cout << "return value:" << val << endl;
			kernel->currentThread->Finish();
            break;
      default:
	      cerr << "Unexpected system call " << type << "\n";
	      break;
      }
      break;
	//page fault
	case PageFaultException:{
		cout<<"Page Fault Exception!"<<endl;
		//Fetch the virtual address where has PageFaultException
		int pageFaultA = (int)kernel->machine->ReadRegister(BadVAddrReg);
		//Fetch the virtual page number of the thread's pageTable
		int pageFaultPN = (int)pageFaultA / PageSize;
		//Check the free physical page number in main memory	
		int PPN = kernel->freeMap->FindAndSet();
		//Fetch the page fault entry of the currentThread
		TranslationEntry* pageEntry = kernel->currentThread->space->getPageEntry(pageFaultPN);
		if(PPN != -1){ //Free physical page
			//Update the page entry
			pageEntry->physicalPage = PPN;
		    pageEntry->valid = TRUE;
		
		    //Read data from swapSpace file and copy it into main memory
		    kernel->swapSpace->ReadAt(
			&(kernel->machine->mainMemory[PPN * PageSize]), 
			PageSize, pageEntry->virtualPage * PageSize);
		
		    //Update FIFOEntryList, append the used physical page at the end of list
		    kernel->FIFOEntryList->Append(pageEntry);
	    }
		else{ //No free physical page
			//Fetch the evicted physical page from FIFOEntryList
			TranslationEntry* evictedPage = kernel->FIFOEntryList->RemoveFront();
			//Fetch the physical page number of the evictedPage
			int PhysicalPN = evictedPage->physicalPage;

            //Copy evicted physical page data from main memory into swapSpace file
			kernel->swapSpace->WriteAt(
			&(kernel->machine->mainMemory[PhysicalPN * PageSize]),
			PageSize, evictedPage->virtualPage * PageSize);
          
		    //Update the evictedPage entry
			evictedPage->physicalPage = -1;
			evictedPage->valid = FALSE;

			//Update the page entry
			pageEntry->physicalPage = PhysicalPN;
		    pageEntry->valid = TRUE;

            //Read data from swapSpace file and copy it into main memory
			kernel->swapSpace->ReadAt(
			&(kernel->machine->mainMemory[PhysicalPN * PageSize]),
			PageSize, pageEntry->virtualPage * PageSize);

			//Update FIFOEntryList, append the used physical page at the end of list
            kernel->FIFOEntryList->Append(pageEntry);
			cout << "No free physical page available! Swap VPN #" << pageEntry->virtualPage << " of thread with PID " 
			<< kernel->currentThread->PID << " for PPN #" << PhysicalPN << endl;
		}
		return;
	}break;

    default:
        cerr << "Unexpected user mode exception" << (int)which << "\n";
        break;
    }
    ASSERTNOTREACHED();
}
