# cython: language_level=3
# cython: cdivision=True
# distutils: language=c++
import cython
from cpython.pycapsule cimport PyCapsule_New
from libcpp cimport bool
from pyce cimport ce

@cython.final
cdef class TToolhelp:
    cdef ce.TToolhelp * ins

    def __cinit__(self, ce.DWORD dwFlags = 0, ce.DWORD dwProcessID = 0):
        self.ins = new ce.TToolhelp(dwFlags, dwProcessID)

    def __dealloc__(self):
        del self.ins

    cpdef inline bool CreateSnapshot(self, ce.DWORD dwFlags, ce.DWORD dwProcessID = 0):
        return self.ins.CreateSnapshot(dwFlags, dwProcessID)

cpdef inline object OpenProcess(ce.DWORD dwDesiredAccess, bool  bInheritHandle, ce.DWORD dwProcessId):
    cdef ce.HANDLE handle = ce.OpenProcess(dwDesiredAccess, bInheritHandle, dwProcessId)
    return PyCapsule_New(handle, NULL, NULL)

cpdef inline bool InjectDLL(str DllFullPath, ce.DWORD dwRemoteProcessId):
    return ce.InjectDLL(<ce.wchar_t *> <char *> DllFullPath.encode(), dwRemoteProcessId)
