#ifndef _JS_INTEROP
#define _JS_INTEROP

int SysScreenWidth();
int SysScreenHeight();
void StartListeningForResizeEvents();
void NativeTextInput(const char *, int, int, int, int);

#endif
