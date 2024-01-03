#include "js_interop.h"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
// WEB PLATFORMS

EM_JS(int, SysScreenWidth, (), {
  return innerWidth;
});

EM_JS(int, SysScreenHeight, (), {
  return innerHeight;
});

EM_JS(void, StartListeningForResizeEvents, (), {
  addEventListener("resize", () => {
    _WindowResized(SysScreenWidth(), SysScreenHeight());
  });
});

#else
// NATIVE PLATFORMS

// TODO: make this real
int SysScreenWidth() {
  return 600;
}

// TODO: make this real
int SysScreenHeight() {
  return 400;
}

void StartListeningForResizeEvents() {}
#endif
