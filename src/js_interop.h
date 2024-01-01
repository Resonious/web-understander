#ifndef _JS_INTEROP
#define _JS_INTEROP

int SysScreenWidth();
int SysScreenHeight();
void StartListeningForResizeEvents();
void NativeBeginDrawing();
void NativeEndDrawing();
int NativeTextInput(const char *id, int x, int y, int w, int h);
char *NativeTextValue(const char *id);

#endif
