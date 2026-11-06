// --- Fonts
SDL_Surface *font;
SDL_Surface *scorefont;
SDL_Surface *scorefont1;
SDL_Surface *scorefont2;
SDL_Surface *menufont;
SDL_Surface *menufont1;

// --- Screen
SDL_Surface *screen;        // offscreen drawing buffer (all game rendering targets this)
SDL_Window *window;         // SDL2 application window
SDL_Renderer *renderer;     // renderer that scales "screen" onto the window
SDL_Texture *screentex;     // streaming texture used to upload "screen" each frame
int defer_present = 0;      // when set, flip_screen() composes without presenting (one present per frame)
SDL_Surface *background;
SDL_Surface *backs;
SDL_Surface *tiles;
SDL_Surface *screenbak;

// --- FX
SDL_Surface *bright;
SDL_Surface *creditsbuffer;
SDL_Surface *creditsbuffer1;
SDL_Surface *creditsfont;
SDL_Surface *left;
SDL_Surface *right;

// --- Monsters
SDL_Surface *monsters;

// --- Player
SDL_Surface *player;

// --- Intro
SDL_Surface *dragontech;
SDL_Surface *line1;
SDL_Surface *line2;
SDL_Surface *comic_01;
SDL_Surface *black;
