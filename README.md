# rpg

in change these paths fitting your installation of raylib:

```Makefile
INCLUDE_PATH = -I/opt/homebrew/include
LIB_PATH = -L/opt/homebrew/Cellar/raylib/5.0/lib
LDFLAGS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit
```
