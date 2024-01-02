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

// Call this before doing things like NativeTextInput
EM_JS(void, NativeBeginDrawing, (), {
  for (const x in window._nd) {
    window._nd[x] = false;
  }
});

// Call this after doing things like NativeTextInput
EM_JS(void, NativeEndDrawing, (), {
  for (const x in window._nd) {
    if (!window._nd[x]) {
      window._submit[x] = false;
      document.getElementById(x)?.remove();
    }
  }
});

EM_JS(int, NativeTextInput, (const char *id_cstr, const char *default_value, int x, int y, int w, int h), {
  const id = UTF8ToString(id_cstr);
  let form = document.getElementById(id);
  let input;
  if (!form) {
    form = document.createElement("form");
    form.id = id;
    form.onsubmit = _trackSubmit;
    form.style.position = "absolute";

    input = document.createElement("input");
    input.type = "text";
    input.style.width = "100%";
    input.style.height = "100%";
    if (default_value != 0) {
      input.value = UTF8ToString(default_value);
    }

    form.append(input);
    document.body.append(form);
  } else {
    input = form.children[0];
  }
  window._nd[id] = true;

  form.style.left = x + "px";
  form.style.top = y + "px";
  form.style.width = w + "px";
  form.style.height = h + "px";

  const submitted = !!window._submit[id];
  window._submit[id] = false;
  return submitted;
});

EM_JS(char *, NativeTextValue, (const char *id_cstr), {
  const id = UTF8ToString(id_cstr);
  const form = document.getElementById(id);

  let result = "";
  if (form) {
    result = form.children[0].value;
  }

  // TODO: might be more practical to copy into a passed-in buffer
  const ptr = allocate(intArrayFromString(result), "i8", ALLOC_NORMAL);
  return ptr;
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
