/*
    Rutina de efectos especiales
*/
SDL_Surface *fadeobject;
SDL_Rect fadingpos; // posicion del objeto que esta desvaneciendose
SDL_Rect realfadingpos; // posicion del objeto que esta desvaneciendose en el area de juego
float fade_y;

float scanpos_y=R_gamearea_y - (P_h*P_h);


void respawn() // rutina que hace reaparecer a Pachi
{
    int scanspeed=1500;
    int figx=int(mplayer[dificulty].x+R_gamearea_x); // figure column, screen x
    int figw=mplayer[dificulty].w;                   // figure width (= P_w)
    float scanfinal=mplayer[dificulty].y+P_h;         // fall is over past this

    // Rebuild the whole falling column from scratch every frame.  First restore
    // the background over the vertical strip the figure falls through.  A literal
    // port of the SDL 1.2 code accumulates one scan-line per presented frame; on
    // the slower SDL 2 renderer that smears every scan-line from the top of the
    // play area down to its target and fills the strip into a solid block, losing
    // the figure shape.  Redrawing each frame instead makes the result identical
    // at any present rate.
    SDL_Rect col;    col.x=figx;                  col.y=R_gamearea_y; col.w=figw; col.h=448;
    SDL_Rect colbak; colbak.x=figx-R_gamearea_x;  colbak.y=0;         colbak.w=figw; colbak.h=448;
    SDL_BlitSurface(screenbak,&colbak,screen,&col);

    // Draw the figure vertically stretched: scan-line a of the sprite (only its
    // non-transparent pixels, via the colour key) is repeated over a P_h-pixel
    // band ending at its current falling position.  Consecutive scan-lines are
    // P_h apart, so their bands tile seamlessly into one continuous, figure-shaped
    // column; the painted length of each x-column equals the vertical extent of
    // that part of the finished figure.  Once a scan-line reaches its resting
    // height it stays there every frame, so the figure accumulates from the feet
    // up (the lowest scan-lines arrive first) while the upper ones are still
    // falling, rather than popping into place at the end.
    SDL_Rect src; src.w=figw; src.h=1; src.x=figw*mplayer[dificulty].frame;
    SDL_Rect dst; dst.w=figw; dst.h=1; dst.x=figx;
    int pile_top=-1;   // smallest y reached by a settled row; -1 = nothing landed yet
    int bridged=0;     // whether the lowest still-falling row was joined to the pile
    for(int a=P_h-1;a>=0;a--)
    {
	src.y=a;
	int cur=int(scanpos_y+(a*P_h));                     // leading pixel of row a
	int yrest=int(mplayer[dificulty].y+R_gamearea_y+a); // its resting height
	if(cur>=yrest)
	{
	    dst.y=yrest;                                    // landed: stays in place
	    SDL_BlitSurface(player,&src,screen,&dst);
	    if(pile_top<0 || yrest<pile_top) pile_top=yrest; // track the top of the pile
	}
	else
	{
	    // Rows are P_h apart, but settled rows sit 1px apart, so the lowest still-
	    // falling row would otherwise stop up to P_h above the built-up figure,
	    // leaving a visible gap.  Bridge that one row down onto the pile.
	    int bottom=cur;
	    int floorcap=yrest;
	    if(!bridged && pile_top>=0) { bottom=pile_top-1; floorcap=pile_top; bridged=1; }
	    for(int ty=cur-P_h+1; ty<=bottom; ty++)         // falling: P_h-long trail
	    {
		if((ty>R_gamearea_y)&&(ty<floorcap))
		{
		    dst.y=ty;
		    SDL_BlitSurface(player,&src,screen,&dst);
		}
	    }
	}
    }
    flip_screen();   // present the whole falling column in one flicker-free frame

    if(scanpos_y<scanfinal)
	scanpos_y+=scanspeed*imove;              // delta-time paced: ~constant duration
    else
    {
	scanpos_y=R_gamearea_y - (P_h*P_h);
	respawned=1;
	imove=0.01;
	SDL_BlitSurface(screenbak,&colbak,screen,&col); // clear the column for the game loop
    }
}


void initfade_object(int x, int y, int object)
{
    alphade=255;
    object-=200;
    fadeobject=SDL_CreateRGBSurface(0,R_tileside,R_tileside,16,0,0,0,0);
    SDL_SetColorKey(fadeobject,SDL_TRUE,(fadeobject->format,0,255,0));
    SDL_Rect objectsrc;
	objectsrc.w=R_tileside;objectsrc.h=R_tileside;
	objectsrc.y=R_tileside*10;objectsrc.x=R_tileside*object;
    SDL_BlitSurface(tiles,&objectsrc,fadeobject,NULL);
    fadingpos.x=x*R_tileside;fadingpos.y=y*R_tileside;
    fadingpos.w=R_tileside;fadingpos.h=R_tileside;
    fade_y=fadingpos.y;
}

float fade_object(float alpha)
{
    int fadealpha = int(alpha);
    set_surface_alpha(fadeobject, 1, fadealpha);
    realfadingpos.x=R_gamearea_x+fadingpos.x;realfadingpos.y=R_gamearea_y+fadingpos.y;realfadingpos.w=fadingpos.w;realfadingpos.h=fadingpos.h;

    SDL_BlitSurface(screenbak,&fadingpos,screen,&realfadingpos);
    SDL_BlitSurface(fadeobject,NULL,screen,&realfadingpos);
    flip_screen();
    if(fadealpha<5)
    {
    	fadingobject=0;
	SDL_BlitSurface(screenbak,&fadingpos,screen,&realfadingpos);
	print_room();
	flip_screen();
    }
    else
    {
	alpha-=500*imove;
	fade_y-=150*imove;
	if(fade_y<32)
	    fade_y+=150*imove;
	fadingpos.y=int(fade_y);
    }
    return(alpha);
}


void screen_fx()
{
    Mix_Volume(7,128);
    if(seconds==15 || seconds==45)
    {
	if(stage<6) // en el cementerio
	{
	    if(int(oldsec-1)==int(seconds))
	    {
		Mix_PlayChannel(7,storm,0);
		blinkscreen(220,220,255,128);
		load_room();
		print_room();
    	    }
	}    
    }
}

void bright_obj(int bright_x, int bright_y)
{
    SDL_Rect brsrc;
	brsrc.x=(brightframe*40);
	brsrc.y=0;brsrc.w=40;brsrc.h=40;
    SDL_Rect brdst;
	brdst.x=bright_x*R_tileside-4;brdst.y=bright_y*R_tileside-4;brdst.w=40;brdst.h=40;
    SDL_Rect brdstreal;
	brdstreal.x=bright_x*R_tileside-4+R_gamearea_x;brdstreal.y=bright_y*R_tileside-4+R_gamearea_y;brdstreal.w=40;brdstreal.h=40;

    SDL_Rect objsrc;
	objsrc.w=R_tileside;objsrc.w=R_tileside;
	objsrc.x=(mroom.data[bright_x][bright_y] - (int(mroom.data[bright_x][bright_y]/20) * 20))*R_tileside;
	objsrc.y=(int(mroom.data[bright_x][bright_y]/20) * R_tileside);                     
    SDL_Rect objdst;
	objdst.x=bright_x*R_tileside;
	objdst.y=bright_y*R_tileside;
	objdst.w=R_tileside;objdst.h=R_tileside;
    SDL_Rect objdstreal;
	objdstreal.x=bright_x*R_tileside+R_gamearea_x;
	objdstreal.y=bright_y*R_tileside+R_gamearea_y;
	objdstreal.w=R_tileside;objdst.h=R_tileside;

    SDL_BlitSurface(screenbak,&brdst,screen,&brdstreal);
//    SDL_BlitSurface(tiles,&objsrc,screen,&objdstreal);
    SDL_BlitSurface(bright,&brsrc,screen,&brdstreal);
    flip_screen();
}

void show_arrow(int arrow, int show)
{
    SDL_Rect dst;
	dst.w=150; dst.h=30;dst.y=R_gamearea_y+1;
    SDL_Rect src;
	src.w=dst.w; src.h=dst.h; src.y=dst.y - R_gamearea_y;
    switch(arrow)
    {
	case arrow_left:
	    dst.x=R_gamearea_x+1;
	    src.x=dst.x-R_gamearea_x;
	    if(show==1)
		SDL_BlitSurface(left, NULL, screen, &dst);
	    else
		SDL_BlitSurface(screenbak, &src, screen, &dst);
		
	    flip_screen();
	    break;
	case arrow_right:
	    dst.x=R_gamearea_x+(R_maxtiles_h*R_tileside)-(10+dst.w);
	    src.x=dst.x-R_gamearea_x;
	    if(show==1)
		SDL_BlitSurface(right, NULL, screen, &dst);
	    else
		SDL_BlitSurface(screenbak, &src, screen, &dst);

	    flip_screen();
	    break;
	case arrow_up:
//fprintf(stderr,"up");
	    break;
	case arrow_down:
//fprintf(stderr,"down");
	    break;
    }
}
