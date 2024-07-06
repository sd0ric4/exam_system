#include <ncurses.h>
#include <locale.h>
#include <wchar.h>
void print_menu(WINDOW *menu_win, int highlight, int choices[], wchar_t *choices_text[], int n_choices) {
    int x, y, i;

    x = 4;
    y = 2;
    box(menu_win, 0, 0);
    for(i = 0; i < n_choices; ++i) { 
        if(highlight == i + 1) { 
            wattron(menu_win, A_REVERSE); 
            mvwprintw(menu_win, y, x, "%ls", choices_text[i]);
            wattroff(menu_win, A_REVERSE);
        } else
            mvwprintw(menu_win, y, x, "%ls", choices_text[i]);

        if(choices[i] == 1) {
            mvwprintw(menu_win, y, x - 2, "[*]");
        } else {
            mvwprintw(menu_win, y, x - 2, "[ ]");
        }

        ++y;
    }
    wrefresh(menu_win);
}

int main() {
    WINDOW *menu_win;
    int highlight = 1;
    int choice = 0;
    int c;

    // 设置语言环境
    setlocale(LC_ALL, "");

    wchar_t *choices[] = { 
        L"选项1",
        L"选项2",
        L"选项3",
        L"选项4",
    };
    int n_choices = sizeof(choices) / sizeof(wchar_t *);
    int choices_selected[n_choices];
    for(int i = 0; i < n_choices; i++) choices_selected[i] = 0;

    initscr();
    clear();
    noecho();
    cbreak();    // Line buffering disabled. pass on everything
    curs_set(0);

    // 获取终端大小
    int height, width;
    getmaxyx(stdscr, height, width);

    int win_height = n_choices + 4; // 2 for borders and 2 for padding
    int win_width = 40; // Set a default width

    int startx = (width - win_width) / 2;
    int starty = (height - win_height) / 2;

    menu_win = newwin(win_height, win_width, starty, startx);
    keypad(menu_win, TRUE);
    mvprintw(0, 0, "Use arrow keys to go up and down, Press space to select an option, Enter to exit");
    refresh();
    print_menu(menu_win, highlight, choices_selected, choices, n_choices);
    while(1) { 
        c = wgetch(menu_win);
        switch(c) { 
            case KEY_UP:
                if(highlight == 1)
                    highlight = n_choices;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if(highlight == n_choices)
                    highlight = 1;
                else 
                    ++highlight;
                break;
            case ' ':
                choices_selected[highlight - 1] = !choices_selected[highlight - 1];
                break;
            case 10: // Enter key
                choice = highlight;
                break;
            default:
                refresh();
                break;
        }
        print_menu(menu_win, highlight, choices_selected, choices, n_choices);
        if(c == 10) // Enter key
            break;
    } 
    clrtoeol();
    refresh();
    endwin();
    return 0;
}
