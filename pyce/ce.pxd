# cython: language_level=3
# cython: cdivision=True
# distutils: language=c++
from libcpp cimport bool
from libc.stdint cimport uintptr_t

cdef extern from "<wchar.h>" nogil:
    ctypedef int wchar_t

ctypedef unsigned long DWORD

cdef extern from "yu.h" nogil:
    ctypedef struct PMODULEENTRY32
    ctypedef struct PPROCESSENTRY32
    ctypedef struct PTHREADENTRY32
    ctypedef struct PHEAPLIST32
    ctypedef struct PHEAPENTRY32
    ctypedef void * HANDLE

    cdef cppclass TToolhelp:
        TToolhelp(DWORD dwFlags, DWORD dwProcessID) except +
        bool CreateSnapshot(DWORD dwFlags, DWORD dwProcessID)
        bool ProcessFirst(PPROCESSENTRY32 ppe)
        bool ProcessNext(PPROCESSENTRY32 ppe)
        bool ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe)
        bool ModuleFirst(PMODULEENTRY32 pme)
        bool ModuleNext(PMODULEENTRY32 pme)
        bool ModuleFind_BaseAddr(void *pvBaseAddr, PMODULEENTRY32 pme)
        bool ModuleFind_ModName(wchar_t * pszModName, PMODULEENTRY32 pme)
        bool ThreadFirst(PTHREADENTRY32  pte)
        bool ThreadNext(PTHREADENTRY32 pte)
        bool HeapListFirst(PHEAPLIST32 phl)
        bool HeapListNext(PHEAPLIST32 phl)
        int HowManyHeaps()
        bool HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, DWORD dwHeapID)
        bool HeapNext(PHEAPENTRY32 phe)
        int HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId)
        bool IsAHeap(HANDLE hProcess, void * pvBlock, DWORD * pdwFlags)
        bool EnableDebugPrivilege(bool fEnable)
        bool ReadProcessMemory(DWORD dwProcessID, void * pvBaseAddress, void * pvBuffer, DWORD cbRead,
                               DWORD *pdwNumberOfBytesRead)

    uintptr_t ScanAddress(HANDLE process, char *markCode, int nOffset, unsigned long dwReadLen, uintptr_t StartAddr,
                          uintptr_t EndAddr, int InstructionLen)
    bool InjectDLL(wchar_t * DllFullPath, DWORD dwRemoteProcessId)
