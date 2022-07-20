from pyce.ce cimport TToolhelp as TToolhelp_C, DWORD, bool


cdef class TToolhelp:
    cdef TToolhelp_C* ins

    def __cinit__(self, DWORD dwFlags = 0, DWORD dwProcessID = 0):
        self.ins = new TToolhelp_C(dwFlags,dwProcessID)

    def __dealloc__(self):
        del self.ins

    cpdef inline bool CreateSnapshot(self, DWORD dwFlags, DWORD dwProcessID = 0):
        return self.ins.CreateSnapshot( dwFlags, dwProcessID )