# microb - cross-language GUI toolkit #

This repo contains the work-in-progress library **microb**: a cross
language GUI toolkit. I plan on using this toolkit to implement 2
apps:

1. A rich-text terminal where apps render stuff via IPC (so for
   example, you don't render a button, but you send the button
   creation message via IPC to the rendering process).
2. A program for running sandboxed online applications, like a web browser 
   but uses Lua and WebAssembly as it's main scripting languages: it will 
   have no HTML nor CSS.
