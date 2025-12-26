#pragma once
#include "server/web/web_sessionstore.h"
class Ctx;

namespace g {
  extern WebSessionStore* webSessions;
  extern Ctx *ctx;
  inline bool web_requires_auth = false;
}
