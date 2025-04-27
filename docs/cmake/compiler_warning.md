```
set(MSVC_WARNINGS
    /W4
    /permissive-)

set(CLANG_WARNINGS
    -Wall
    -Wextra
    -Wpedantic)

set(GCC_WARNINGS ${CLANG_WARNINGS})

if(${ENABLED_AS_ERRORS})
    set(MSVC_WARNINGS ${MSVC_WARNINGS} /WX)
    set(CLANG_WARNINGS ${CLANG_WARNINGS} -Werror)
    set(GCC_WARNINGS ${GCC_WARNINGS} -Werror)
endif()
```