#ifndef __RESOURCES_X__
#define __RESOURCES_X__

//
//	BASE DIRECTORY
//
#ifdef __MACH__
    #define RES_DIRECTORY "./"
#else
    #define RES_DIRECTORY "Resources\\"
#endif

//
//	CONTENT SIZE SETTINGS
//
//#ifdef __MACH__
    #define MAX_COLS 12
    #define MAX_ROWS 10
//#else
//    #define MAX_COLS 12
//    #define MAX_ROWS 9
//#endif

#define MAX_COLS_IPAD3 12
#define MAX_ROWS_IPAD3 10


//
//	IMAGE RESOURCES
//
#define IMG_CLOSE_NORMAL RES_DIRECTORY "CloseNormal.png"
#define IMG_CLOSE_SELECTED RES_DIRECTORY "CloseSelected.png"

#define IMG_MAIN_TILESET RES_DIRECTORY "tileset.png"
#define IMG_MAIN_TILESET_PLIST RES_DIRECTORY "tileset.plist"

#define IMG_SNOWFLAKE RES_DIRECTORY "snowflake.png"

#endif