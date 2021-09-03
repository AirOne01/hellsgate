#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>

// head
void mainWindow();
void appendFrame(WINDOW *win, std::string str, int w_y, int w_x, int *frame_x, int *frame_y, int pair = 0);

int main(int argc, char ** archv)
{
    /* NCURSES START */
    // initializes the screen
    // sets up memory and clears the screen
    initscr();
    // avoids ^C not closing the program
    cbreak();
    
    // program stop variable
    bool stop = false;
    while (!stop) {
        // generates window
        mainWindow();
        // gets input. if terminal size changes, restarts
        stop = ( getch() != KEY_RESIZE );
    }

    // deallocates memory and ends ncurses
    /* NCURSE END */
    endwin();

    return 0;
}

void mainWindow()
{
    // gets window size in a structure
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int height, width, start_y, start_x;
    height = w.ws_row;
    width = w.ws_col;
    start_y = start_x = 0;
    // we are also getting some reference sizes for later
    int w_y = w.ws_row-2;
    int w_x = w.ws_col-2;

    // makes a window with the height and width of the terminal
    WINDOW *win = newwin(height, width, start_y, start_x);
    refresh();

    // all the magic happens here
    // makes borders to the window and refreshes
    box(win, 0, 0);
    mvwprintw(win, 0, (w.ws_col/2-10), "[ The Hell's Gate ]");
    
    // this is where the frames will be generated
    int frame_x, frame_y = 0;
    wmove(win, 1, 1);
    // checks for color support
    if (has_colors()) {
        start_color();
        init_pair(1, COLOR_CYAN, COLOR_BLACK);
        init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    }
    for (int i = 0; i<100; i++) {
        appendFrame(win, "Frame ", w_x, w_y, &frame_x, &frame_y, 2);
    }

    // refreshes the box
    wrefresh(win);
}

void appendFrame(WINDOW *win, std::string str, int w_x, int w_y, int *frame_x, int *frame_y, int pair)
{
    // checks if the terminal is too small to draw
    if (str.length() > w_x) return;
    // if color supported
    if (pair != 0) {
        wattron(win, COLOR_PAIR(pair));
        // prints 
        wprintw(win, str.c_str());
        // diables colors
        wattroff(win, COLOR_PAIR(pair));
    } else {
        // just prints
        wprintw(win, str.c_str());
    }

    // then refreshes
    wrefresh(win);

    return;
}
