#ifndef COLOR_H
#define COLOR_H

#define COLOR_ESCAPE  '^'
#define is_color_string( p )  ( p && *( p ) == COLOR_ESCAPE && *( ( p ) + 1 ) && *( ( p ) + 1 ) != COLOR_ESCAPE )

#define COLOR_BLACK     '0'
#define COLOR_RED       '1'
#define COLOR_GREEN     '2'
#define COLOR_YELLOW    '3'
#define COLOR_BLUE      '4'
#define COLOR_CYAN      '5'
#define COLOR_MAGENTA   '6'
#define COLOR_WHITE     '7'
#define color_index( c )   ( ( ( c ) - '0' ) & 7 )

#define S_COLOR_BLACK   "^0"
#define S_COLOR_RED     "^1"
#define S_COLOR_GREEN   "^2"
#define S_COLOR_YELLOW  "^3"
#define S_COLOR_BLUE    "^4"
#define S_COLOR_CYAN    "^5"
#define S_COLOR_MAGENTA "^6"
#define S_COLOR_WHITE   "^7"


#endif /* COLOR_H */
