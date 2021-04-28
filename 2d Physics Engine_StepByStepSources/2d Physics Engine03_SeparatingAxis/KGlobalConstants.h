#pragma once
#include "KVector2.h"

const float				PI = 3.141592741f;
const float				EPSILON = 0.0001f;
//const float				g_gravityScale = 5.0f; // original 5.0f. 20210428_jintaeks
const float				g_gravityScale = 0.0f; // original 5.0f. 20210428_jintaeks
const KVector2			g_gravity(0, 9.8f * g_gravityScale);
const float				g_dt = 1.0f / 60.0f;
const bool				g_drawPenetration = false;