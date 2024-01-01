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

EM_JS(void, NativeTextInput, (const char *cid, int x, int y, int w, int h), {
  const id = UTF8ToString(cid);
  let element = document.getElementById(id);
  if (!element) {
    element = document.createElement("input");
    element.id = id;
    element.type = "text";
    element.style.position = "absolute";
    document.body.append(element);
  }

  element.style.left = x + "px";
  element.style.top = y + "px";
  element.style.width = w + "px";
  element.style.height = h + "px";
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
