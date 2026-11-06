/***************************************************************************
*                          Pachi el marciano                               *
*                          -----------------                               *
*                     (c) Santiago Radeff (coding)                         *
*                     (c) Nicolas Radeff  (graphics)                       *
*                     (c) Peter Hajba     (music)                          *
*                                                                          *
*                          T-1000@Bigfoot.com                              *
****************************************************************************
    *******************************************************************
    *                                                                 *
    *   This program is free software; you can redistribute it and/or *
    *   modify it under the terms of the GNU General Public License   *
    *   as published by the Free Software Foundation; either version  *
    *   2 of the License, or (at your option) any later version.      *
    *                                                                 *
    *******************************************************************/

void initsdl()
{

    int have_joystick=0;    

    if(SDL_Init(SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) < 0)
    {
        fprintf(stderr,"Could not Initialize SDL.\nError: %s\n", SDL_GetError());
        exit (1);
    }
    if(Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048) < 0)
    {
        fprintf(stderr,"Warning: Couldn't set 44100 Hz 16-bit audio\n: %s\n", SDL_GetError());
    }
    fullscreen=0;

    // SDL2 splits the old SDL_SetVideoMode() into a window plus a renderer.
    window = SDL_CreateWindow("Pachi el marciano",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              screen_w, screen_h, 0);
    if (window == NULL)
    {
        fprintf(stderr, "Can't create the window. Quitting.\nError: %s\n", SDL_GetError());
        exit (1);
    }

    // No SDL_RENDERER_PRESENTVSYNC: the game already paces itself with SDL_Delay().
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
    {
        fprintf(stderr, "Can't create the renderer. Quitting.\nError: %s\n", SDL_GetError());
        exit (1);
    }
    // Make the fixed 800x600 game image scale to the window (and to full screen)
    // with correct aspect ratio and automatic letterboxing.
    SDL_RenderSetLogicalSize(renderer, screen_w, screen_h);

    // "screen" stays a plain software surface, exactly like under SDL 1.2, so the
    // rest of the game keeps blitting and locking it unchanged. It is uploaded to
    // "screentex" and drawn through the renderer once per frame (see flip_screen()).
    // RGB565 matches the default pixel format SDL picks for a 16 bpp surface.
    screen = SDL_CreateRGBSurface(0, screen_w, screen_h, screen_bpp, 0, 0, 0, 0);
    screentex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB565,
                                  SDL_TEXTUREACCESS_STREAMING, screen_w, screen_h);
    if (screen == NULL || screentex == NULL)
    {
        fprintf(stderr, "Can't create the drawing surface. Quitting.\nError: %s\n", SDL_GetError());
        exit (1);
    }

    // Set the window icon. The original passed the SDL_LoadBMP() result straight
    // through and never freed it; guard against a failed load and release the
    // surface afterwards (SDL2 keeps its own copy).
    SDL_Surface *icon = SDL_LoadBMP(DATADIR"/Tgfx/icon.bmp");
    if (icon != NULL)
    {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    have_joystick = SDL_NumJoysticks();
//    fprintf(stderr, "%i joysticks were found.\n", have_joystick );
    if (have_joystick)
    {                                                        
	SDL_JoystickEventState(SDL_ENABLE);                                  
	joystick = SDL_JoystickOpen(0);                                      
    }                                                                           

    SDL_ShowCursor(0);
    SDL_SetWindowGrab(window, SDL_TRUE);
    SDL_SetWindowGrab(window, SDL_FALSE);
}

// Present the offscreen "screen" surface to the window. This single helper
// replaces both flip_screen() and the partial flip_screen()
// calls: with a renderer we simply re-upload the whole frame and present it.
// At 800x600 this is cheap, and it preserves the original "show it now" timing.
void flip_screen()
{
    // While a frame is still being composed (see game_loop), suppress the
    // present so a half-drawn frame is never shown. Without this, the player
    // sprite flickers: clean_player() erases it, and the presents inside
    // print_monsters()/fade_object() would otherwise show that erased frame.
    if (defer_present)
        return;
    SDL_UpdateTexture(screentex, NULL, screen->pixels, screen->pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, screentex, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// SDL2 has no display_format(): convert to the format of our "screen"
// surface, which is what every former display_format() call effectively did.
SDL_Surface *display_format(SDL_Surface *surface)
{
    return SDL_ConvertSurface(surface, screen->format, 0);
}

// SDL2 replaces SDL_SetAlpha(): a non-zero flag enabled per-surface alpha
// blending at the given level, a zero flag disabled it again.
void set_surface_alpha(SDL_Surface *surface, int enable, Uint8 alpha)
{
    if (enable)
    {
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_BLEND);
        SDL_SetSurfaceAlphaMod(surface, alpha);
    }
    else
        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);
}
