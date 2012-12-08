#ifndef __RESOURCES_X__
#define __RESOURCES_X__


//
//	CONTENT SIZE SETTINGS
//

#define MAX_COLS			20
#define MAX_ROWS			20

#define MAX_COLS_IPAD3		14
#define MAX_ROWS_IPAD3		10

#define MAX_COLS_WINDOWS	12
#define MAX_ROWS_WINDOWS	9

//	
//	GEM SIZES AND OPACITY
//
#ifdef __MACH__
    static float GEM_SCALE = 0.5f;
#else
    static float GEM_SCALE = 0.4f;
#endif

#define GEM_WIDTH	100 * GEM_SCALE
#define GEM_HEIGHT	120 * GEM_SCALE
#define GEM_SPACING	10 * GEM_SCALE

#define SPARE_ROW_FADE_START	50
#define SPARE_ROW_FADE_END		255

#define GEM_SCALE_IPAD3		1.0f
#define GEM_SCALE_IPHONE4	0.7f


//
//	BASE DIRECTORY
//
#ifdef __MACH__
    #define RES_DIRECTORY "./"
    #define BACKS_DIR ""
#else
    #define RES_DIRECTORY "..\\..\\Resources\\"
    #define BACKS_DIR "backs\\"
#endif


//
//	IMAGE RESOURCES
//
#define IMG_CLOSE_NORMAL RES_DIRECTORY "CloseNormal.png"
#define IMG_CLOSE_SELECTED RES_DIRECTORY "CloseSelected.png"

#define IMG_MAIN_TILESET RES_DIRECTORY "tileset.png"
#define IMG_MAIN_TILESET_PLIST RES_DIRECTORY "tileset.plist"

#define IMG_SNOWFLAKE RES_DIRECTORY "snowflake.png"

#define IMG_BLUE_BACK RES_DIRECTORY BACKS_DIR "blueBG.png"
#define IMG_GREEN_BACK RES_DIRECTORY BACKS_DIR "greenBG.png"
#define IMG_ORANGE_BACK RES_DIRECTORY BACKS_DIR "orangeBG.png"

#define IMG_SPOOKY RES_DIRECTORY "spooky_logo.png"
#define IMG_BUTTON RES_DIRECTORY "button.png"

#define IMG_CLOCK	RES_DIRECTORY "clock.png"
#define IMG_CLOCK_FORE	RES_DIRECTORY "clock_fore.png"

#endif