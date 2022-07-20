# cython: language_level=3
# cython: cdivision=True
# distutils: language=c++
import cython
from pyce cimport ce

@cython.final
cdef class TToolhelp:
    cdef ce.TToolhelp* ins

    def __cinit__(self, ce.DWORD dwFlags = 0, ce.DWORD dwProcessID = 0):
        self.ins = new ce.TToolhelp(dwFlags,dwProcessID)

    def __dealloc__(self):
        del self.ins

    cpdef inline ce.bool CreateSnapshot(self, ce.DWORD dwFlags, ce.DWORD dwProcessID = 0):
        return self.ins.CreateSnapshot( dwFlags, dwProcessID )
