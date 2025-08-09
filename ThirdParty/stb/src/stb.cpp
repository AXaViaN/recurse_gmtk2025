#include <AX/Tool/Reporter.h>

// TODO_ASC: Define STBI_MALLOC, STBI_REALLOC, and STBI_FREE to avoid using malloc,realloc,free

#define STBI_ASSERT(x) AX_Assert(x)

#define STBI_FAILURE_USERMSG

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
