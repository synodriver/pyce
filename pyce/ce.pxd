# cython: language_level=3
# cython: cdivision=True
# distutils: language=c++
from libcpp cimport bool
from libc.stdint cimport uintptr_t
cdef extern from "<wchar.h>" nogil:
    ctypedef wchar_t

ctypedef bool BOOL
ctypedef unsigned long DWORD


cdef extern from "yu.h" nogil:
    ctypedef struct PMODULEENTRY32
    ctypedef struct PPROCESSENTRY32
    ctypedef struct PTHREADENTRY32
    ctypedef struct PHEAPLIST32
    ctypedef struct PHEAPENTRY32
    ctypedef wchar_t WCHAR
    ctypedef void* HANDLE

    cppclass TToolhelp:
        TToolhelp(DWORD dwFlags = 0, DWORD dwProcessID = 0) except +
        ~TToolhelp()
        BOOL CreateSnapshot(DWORD dwFlags, DWORD dwProcessID = 0)
        BOOL ProcessFirst(PPROCESSENTRY32 ppe)
        BOOL ProcessNext(PPROCESSENTRY32 ppe)
        BOOL ProcessFind(DWORD dwProcessId, PPROCESSENTRY32 ppe)
        BOOL ModuleFirst(PMODULEENTRY32 pme)
        BOOL ModuleNext(PMODULEENTRY32 pme)
        BOOL ModuleFind_BaseAddr(void *pvBaseAddr, PMODULEENTRY32 pme)
        BOOL ModuleFind_ModName(WCHAR * pszModName, PMODULEENTRY32 pme)
        BOOL ThreadFirst(PTHREADENTRY32  pte)
        BOOL ThreadNext(PTHREADENTRY32 pte)
        BOOL HeapListFirst(PHEAPLIST32 phl)
        BOOL HeapListNext(PHEAPLIST32 phl)
        int HowManyHeaps()
        BOOL HeapFirst(PHEAPENTRY32 phe, DWORD dwProcessID, DWORD dwHeapID)
        BOOL HeapNext(PHEAPENTRY32 phe)
        int HowManyBlocksInHeap(DWORD dwProcessID, DWORD dwHeapId)
        BOOL IsAHeap(HANDLE hProcess, void * pvBlock, DWORD * pdwFlags)
        BOOL EnableDebugPrivilege(BOOL fEnable = True)
        BOOL ReadProcessMemory(DWORD dwProcessID, void* pvBaseAddress, void* pvBuffer,
		DWORD cbRead, DWORD *pdwNumberOfBytesRead = NULL)

    uintptr_t ScanAddress(HANDLE process, char *markCode, int nOffset, unsigned long dwReadLen = 4,
                          uintptr_t StartAddr = 0x400000, uintptr_t EndAddr = 0x7FFFFFFF, int InstructionLen = 0)
    BOOL InjectDLL(const wchar_t * DllFullPath, const DWORD dwRemoteProcessId)
