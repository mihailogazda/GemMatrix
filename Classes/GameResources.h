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
#ifdef __MACH__
    #define MAX_COLS 16
    #define MAX_ROWS 11
#else
    #define MAX_COLS 12
    #define MAX_ROWS 9
#endif

//
//	IMAGE RESOURCES
//
#define IMG_CLOSE_NORMAL RES_DIRECTORY "CloseNormal.png"
#define IMG_CLOSE_SELECTED RES_DIRECTORY "CloseSelected.png"

#define IMG_MAIN_TILESET RES_DIRECTORY "tileset.png"
#define IMG_MAIN_TILESET_PLIST RES_DIRECTORY "tileset.plist"

#define IMG_SNOWFLAKE RES_DIRECTORY "snowflake.png"