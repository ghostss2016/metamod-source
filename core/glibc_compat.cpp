// Force linking to older GLIBC symbols for compatibility
// Targets GLIBC < 2.31 (Steam Runtime 3 Sniper / Debian 11)
#include <stdlib.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdarg.h>

extern "C" {
    void* __wrap_dlopen(const char* filename, int flags);
    void* __wrap_dlsym(void* handle, const char* symbol);
    int __wrap_dlclose(void* handle);
    int __wrap_dladdr(const void* addr, Dl_info* info);
    int __wrap_dlinfo(void* handle, int request, void* info);
    char* __wrap_dlerror(void);
    int __wrap_pthread_once(pthread_once_t* once_control, void (*init_routine)(void));
}

__asm__(".symver __glibc_dlopen, dlopen@GLIBC_2.2.5");
__asm__(".symver __glibc_dlsym, dlsym@GLIBC_2.2.5");
__asm__(".symver __glibc_dlclose, dlclose@GLIBC_2.2.5");
__asm__(".symver __glibc_dladdr, dladdr@GLIBC_2.2.5");
__asm__(".symver __glibc_dlinfo, dlinfo@GLIBC_2.3.3");
__asm__(".symver __glibc_dlerror, dlerror@GLIBC_2.2.5");
__asm__(".symver __glibc_pthread_once, pthread_once@GLIBC_2.2.5");

extern "C" void* __glibc_dlopen(const char*, int);
extern "C" void* __glibc_dlsym(void*, const char*);
extern "C" int __glibc_dlclose(void*);
extern "C" int __glibc_dladdr(const void*, Dl_info*);
extern "C" int __glibc_dlinfo(void*, int, void*);
extern "C" char* __glibc_dlerror(void);
extern "C" int __glibc_pthread_once(pthread_once_t*, void (*)(void));

extern "C" void* __wrap_dlopen(const char* filename, int flags) {
    return __glibc_dlopen(filename, flags);
}
extern "C" void* __wrap_dlsym(void* handle, const char* symbol) {
    return __glibc_dlsym(handle, symbol);
}
extern "C" int __wrap_dlclose(void* handle) {
    return __glibc_dlclose(handle);
}
extern "C" int __wrap_dladdr(const void* addr, Dl_info* info) {
    return __glibc_dladdr(addr, info);
}
extern "C" int __wrap_dlinfo(void* handle, int request, void* info) {
    return __glibc_dlinfo(handle, request, info);
}
extern "C" char* __wrap_dlerror(void) {
    return __glibc_dlerror();
}
extern "C" int __wrap_pthread_once(pthread_once_t* once_control, void (*init_routine)(void)) {
    return __glibc_pthread_once(once_control, init_routine);
}

__asm__(".symver __glibc_pthread_key_create, __pthread_key_create@GLIBC_2.2.5");
extern "C" int __glibc_pthread_key_create(pthread_key_t*, void (*)(void*));
extern "C" int __wrap___pthread_key_create(pthread_key_t* key, void (*destructor)(void*)) {
    return __glibc_pthread_key_create(key, destructor);
}

extern "C" int __wrap_fstat64(int fd, struct stat64* statbuf) {
    return syscall(SYS_fstat, fd, statbuf);
}
extern "C" int __wrap_fstat(int fd, struct stat* statbuf) {
    return syscall(SYS_fstat, fd, statbuf);
}
extern "C" int __wrap_stat64(const char* path, struct stat64* statbuf) {
    return syscall(SYS_stat, path, statbuf);
}
extern "C" int __wrap_stat(const char* path, struct stat* statbuf) {
    return syscall(SYS_stat, path, statbuf);
}

extern "C" long __strtol_old(const char* nptr, char** endptr, int base);
__asm__(".symver __strtol_old, strtol@GLIBC_2.2.5");
extern "C" long __wrap___isoc23_strtol(const char* nptr, char** endptr, int base) {
    return __strtol_old(nptr, endptr, base);
}

extern "C" unsigned long __strtoul_old(const char* nptr, char** endptr, int base);
__asm__(".symver __strtoul_old, strtoul@GLIBC_2.2.5");
extern "C" unsigned long __wrap___isoc23_strtoul(const char* nptr, char** endptr, int base) {
    return __strtoul_old(nptr, endptr, base);
}

extern "C" unsigned long long __strtoull_old(const char* nptr, char** endptr, int base);
__asm__(".symver __strtoull_old, strtoull@GLIBC_2.2.5");
extern "C" unsigned long long __wrap___isoc23_strtoull(const char* nptr, char** endptr, int base) {
    return __strtoull_old(nptr, endptr, base);
}

extern "C" long long __strtoll_old(const char* nptr, char** endptr, int base);
__asm__(".symver __strtoll_old, strtoll@GLIBC_2.2.5");
extern "C" long long __wrap___isoc23_strtoll(const char* nptr, char** endptr, int base) {
    return __strtoll_old(nptr, endptr, base);
}

extern "C" int __old_vfscanf(FILE* stream, const char* format, va_list ap);
__asm__(".symver __old_vfscanf, vfscanf@GLIBC_2.2.5");
extern "C" int __wrap___isoc23_fscanf(FILE* stream, const char* format, ...) {
    va_list ap; va_start(ap, format);
    int ret = __old_vfscanf(stream, format, ap);
    va_end(ap);
    return ret;
}
extern "C" int __old_vsscanf(const char* s, const char* format, va_list ap);
__asm__(".symver __old_vsscanf, vsscanf@GLIBC_2.2.5");
extern "C" int __wrap___isoc23_sscanf(const char* s, const char* format, ...) {
    va_list ap; va_start(ap, format);
    int ret = __old_vsscanf(s, format, ap);
    va_end(ap);
    return ret;
}

struct dl_find_object;
extern "C" __attribute__((weak)) int _dl_find_object(void* address, struct dl_find_object* result) {
    return -1;
}

namespace std {
    extern "C" __attribute__((weak)) void _ZSt21ios_base_library_initv() {}
}

extern "C" float __wrap_fmodf(float x, float y) {
    if (y == 0.0f) return 0.0f;
    return x - (int)(x / y) * y;
}
extern "C" double __wrap_fmod(double x, double y) {
    if (y == 0.0) return 0.0;
    return x - (long long)(x / y) * y;
}

extern "C" __attribute__((visibility("default"))) char __libc_single_threaded = 0;
