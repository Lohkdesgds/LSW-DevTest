#pragma once

/*
This will include ALL libraries
- You can actually include only a fraction of it, but sincerely I prefer to include them all.

Dependency order is:
- Between same folder
- One folder up

Folder order:
- Handling
- Tools
- Interface
- Work
*/

#define WIN32_LEAN_AND_MEAN

#include "Handling/Handling.h"
#include "Tools/Tools.h"
#include "Interface/Interface.h"