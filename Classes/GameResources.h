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

#define GEM_ROCK_PROBABILITY_MAX 40

#define GAME_FONT "Impact"

//
//	BASE DIRECTORY
//
#ifdef __MACH__
    #define RES_DIRECTORY "./"
    #define BACKS_DIR ""
#else
    #define RES_DIRECTORY "..\\Resources\\"
    #define BACKS_DIR "backs\\"
#endif

//
//	LEVELS
//
#ifdef __MACH__
    #define LEVEL_RESOURCE "levels.xml"
#else
    #define LEVEL_RESOURCE RES_DIRECTORY "levels.xml"
#endif

//
//	SETTINGS
//
#define SETTING_LEVEL_CURRENT	"currentLevel"
#define SETTING_TOTAL_POINTS	"totalPoints"

//
//	IMAGE RESOURCES
//
#define IMG_CLOSE_NORMAL "CloseNormal.png"
#define IMG_CLOSE_SELECTED "CloseSelected.png"


#define IMG_MAIN_TILESET "tileset.png"
#define IMG_MAIN_TILESET_PLIST "tileset.plist"

#define IMG_SNOWFLAKE "snowflake.png"

#define IMG_BLUE_BACK BACKS_DIR "blueBG.png"
#define IMG_GREEN_BACK BACKS_DIR "greenBG.png"
#define IMG_GREENYELLOW BACKS_DIR "greenyellowBG.png"

#define IMG_SPOOKY "spooky_logo.png"
#define IMG_BUTTON "button.png"

#define IMG_CLOCK	"clock.png"
#define IMG_CLOCK_FORE	"clock_fore.png"

#define IMG_PROGRESS "progress.png"
#define IMG_PROGRESS_FORE "progress_fore.png"


#endif