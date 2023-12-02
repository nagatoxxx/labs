#ifndef TEXT_H
#define TEXT_H

#define RESET "\033[0m"

#define FG_RED   "\033[38;2;255;00;00m"
#define FG_BLACK "\033[38;2;0;0;0m"
#define FG_GREEN "\033[38;2;0;255;0m"

#define BG_RED   "\033[48;2;255;00;00m"
#define BG_GREEN "\033[48;2;0;255;0m"

#define CROSSED     "\033[9m"
#define CROSSED_OFF "\033[29m"

#define UNDERLINED     "\033[4m"
#define UNDERLINED_OFF "\033[24m"

#define ITALIC     "\033[3m"
#define ITALIC_OFF "\033[23m"

#define BOLD "\033[1m"

#define INF "[i] "
#define ERR BOLD FG_RED

#endif
