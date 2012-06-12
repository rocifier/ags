
#ifndef __AC_ROOM_H
#define __AC_ROOM_H

#include "ac/ac_defines.h"      // PCKD
#include "ac/ac_interaction.h"  // NewInteraction
#include "ac/ac_customproperties.h"
#include "ac/ac_messageinfo.h"
#include "ac/ac_animationstruct.h"
#include "ac/ac_point.h"
#include "cs/cc_script.h"       // ccScript


// thisroom.options[0] = startup music
// thisroom.options[1] = can save/load on screen (0=yes, 1=no)
// thisroom.options[2] = player character disabled? (0=no, 1=yes)
// thisroom.options[3] = player special view (0=normal)
//                 [4] = music volume (0=normal, <0 quiter, >0 louder)

const int ST_TUNE = 0, ST_SAVELOAD = 1, ST_MANDISABLED = 2, ST_MANVIEW = 3, ST_VOLUME = 4;

#pragma pack(1)
struct sprstruc {
    short sprnum  PCKD;   // number from array
    short x,y     PCKD;   // x,y co-ords
    short room    PCKD;   // room number
    short on      PCKD;
    sprstruc() { on = 0; }

#ifdef ALLEGRO_BIG_ENDIAN
    void ReadFromFile(FILE *fp)
    {
        sprnum = __getshort__bigendian(fp);
        x = __getshort__bigendian(fp);
        y = __getshort__bigendian(fp);
        room = __getshort__bigendian(fp);
        on = __getshort__bigendian(fp);
    }
#endif
};
#pragma pack()

#define MAXANIMS      10
#define MAX_FLAGS     15
#define MAXOBJNAMELEN 30
#define MAX_BSCENE    5   // max number of frames in animating bg scene
#define NOT_VECTOR_SCALED -10000
#define TINT_IS_ENABLED 0x80000000
struct roomstruct {
    block         walls, object, lookat;          // 'object' is the walk-behind
    block         regions;
    color         pal[256];
    short         numobj;                         // num hotspots, not sprites
    short         objyval[MAX_OBJ];               // baselines of walkbehind areas
    // obsolete v2.00 action editor stuff below
    short         whataction[NUM_CONDIT+3];       // what to do if condition appears
    short         val1[NUM_CONDIT+3];             // variable, eg. screen number to go to
    short         val2[NUM_CONDIT+3];             // 2nd var, optional, eg. which side of screen to come on
    short         otcond[NUM_CONDIT+3];           // do extra misc condition
    char          points[NUM_CONDIT+3];           // extra points for doing it
    // end obsolete v2.00 action editor
    short         left,right,top,bottom;          // to walk off screen
    short         numsprs,nummes;                 // number of initial sprites and messages
    sprstruc      sprs[MAX_INIT_SPR];             // structures for each sprite
    NewInteraction *intrObject[MAX_INIT_SPR];
    InteractionScripts **objectScripts;
    int           objbaseline[MAX_INIT_SPR];                // or -1 (use bottom of object graphic)
    short         objectFlags[MAX_INIT_SPR];
    char          objectnames[MAX_INIT_SPR][MAXOBJNAMELEN];
    char          objectscriptnames[MAX_INIT_SPR][MAX_SCRIPT_NAME_LEN];
    CustomProperties objProps[MAX_INIT_SPR];
    char          password[11];
    char          options[10];                    // [0]=startup music
    char          *message[MAXMESS];
    MessageInfo   msgi[MAXMESS];
    short         wasversion;                     // when loaded from file
    short         flagstates[MAX_FLAGS];
    FullAnimation anims[MAXANIMS];
    short         numanims;
    short         shadinginfo[16];    // walkable area-specific view number
    // new version 2 roommake stuff below
    int           numwalkareas;
    PolyPoints    wallpoints[MAX_WALK_AREAS];
    int           numhotspots;
    _Point        hswalkto[MAX_HOTSPOTS];
    char*         hotspotnames[MAX_HOTSPOTS];
    char          hotspotScriptNames[MAX_HOTSPOTS][MAX_SCRIPT_NAME_LEN];
    NewInteraction *intrHotspot[MAX_HOTSPOTS];
    NewInteraction *intrRoom;
    NewInteraction *intrRegion[MAX_REGIONS];
    InteractionScripts **hotspotScripts;
    InteractionScripts **regionScripts;
    InteractionScripts *roomScripts;
    int           numRegions;
    short         regionLightLevel[MAX_REGIONS];
    int           regionTintLevel[MAX_REGIONS];
    short         width,height;                             // in 320x200 terms (scrolling room size)
    short         resolution;                               // 1 = 320x200, 2 = 640x400
    short         walk_area_zoom[MAX_WALK_AREAS + 1];       // 0 = 100%, 1 = 101%, -1 = 99%
    short         walk_area_zoom2[MAX_WALK_AREAS + 1];      // for vector scaled areas
    short         walk_area_light[MAX_WALK_AREAS + 1];      // 0 = normal, + lighter, - darker
    short         walk_area_top[MAX_WALK_AREAS + 1];     // top YP of area
    short         walk_area_bottom[MAX_WALK_AREAS + 1];  // bottom YP of area
    char          *scripts;
    ccScript      *compiled_script;
    int           cscriptsize;
    int           num_bscenes, bscene_anim_speed;
    int           bytes_per_pixel;
    block         ebscene[MAX_BSCENE];
    color         bpalettes[MAX_BSCENE][256];
    InteractionVariable *localvars;
    int           numLocalVars;
    char          ebpalShared[MAX_BSCENE];  // used internally by engine atm
    CustomProperties roomProps;
    CustomProperties hsProps[MAX_HOTSPOTS];
    int           gameId;
    int           lastLoadNumHotspots;
    int           lastLoadNumObjects;
    int           lastLoadNumRegions;

    roomstruct();
    //void allocall();
    //void freeall();
    void freemessage();
};

#endif // __AC_ROOM_H