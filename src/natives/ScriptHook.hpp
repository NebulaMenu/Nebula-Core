#pragma once
#include <windows.h>
#include "script.h"
#include "../natives/Natives.hpp"

#ifndef WAIT
#define WAIT(ms) scriptWait(ms)
#endif
