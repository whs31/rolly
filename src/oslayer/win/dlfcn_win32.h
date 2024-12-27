#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

  /**
   * @brief Relocations are performed when the object is loaded.
   */
#define RTLD_NOW    0

  /**
   * @brief Relocations are performed at an implementation-defined time.
   * @remark Windows API does not support lazy symbol resolving (when first reference
   * to a given symbol occurs). So RTLD_LAZY implementation is same as RTLD_NOW.
   */
#define RTLD_LAZY   RTLD_NOW

  /**
   * @brief All symbols are available for relocation processing of other modules.
   */
#define RTLD_GLOBAL (1 << 1)

  /**
   * @brief All symbols are not made available for relocation processing by other modules.
   */
#define RTLD_LOCAL  (1 << 2)

  /**
   * @brief The symbol lookup happens in the normal global scope.
   */
#define RTLD_DEFAULT    ((void *)0)

  /**
   * @brief Specifies the next object after this one that defines name.
   */
#define RTLD_NEXT       ((void *)-1)

  /**
   * @brief Structure filled in by dladdr()
   */
  typedef struct dl_info {
    /**
     * @brief Filename of defining object (thread unsafe and reused on every call to dladdr)
     */
    char const* dli_fname;

    /**
     * @brief Load address of that object
     */
    void* dli_fbase;

    /**
     * @brief Name of nearest lower symbol
     */
    char const* dli_sname;

    /**
     * @brief Exact value of nearest symbol
     */
    void* dli_saddr;
  } Dl_info;

  /**
   * @brief Open a symbol table handle.
   */
  void* dlopen(char const* file, int mode);

  /**
   * @brief Close a symbol table handle.
   */
  int dlclose(void* handle);

  /**
   * @brief Get the address of a symbol from a symbol table handle.
   */
  void* dlsym(void* handle, char const* name);

  /**
   * @brief Get diagnostic information.
   */
  char* dlerror(void);

  /**
   * @brief Translate address to symbolic information (no POSIX standard)
   */
  int dladdr(void const* addr, Dl_info* info);

#ifdef __cplusplus
}
#endif
