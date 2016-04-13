/**
 *  _____
 * |  __ \
 * | |__) | __ _  _   _
 * |  _  / / _` || | | |
 * | | \ \| (_| || |_| |
 * |_|  \_\\__,_| \__, |
 *                 __/ |
 *                |___/
 *  ______                        _         _    _
 * |  ____|                      | |       | |  (_)
 * | |__  ___   _   _  _ __    __| |  __ _ | |_  _   ___   _ __
 * |  __|/ _ \ | | | || '_ \  / _` | / _` || __|| | / _ \ | '_ \
 * | |  | (_) || |_| || | | || (_| || (_| || |_ | || (_) || | | |
 * |_|   \___/  \__,_||_| |_| \__,_| \__,_| \__||_| \___/ |_| |_|
 *
 **/

#include <RayFoundation/RayFoundation.h>
#include <ncurses.h>
#include <unistd.h>
#include "Tests.h"

#define SBSize 256

sandBoxNamed(RaySandbox, SBSize * SBSize)

#define RSB RaySandbox()

void drawMemoryMap(WINDOW *outputWindow) {
    size_t iterator;
    wmove(outputWindow, 0, 0);

    forAll(iterator, RSB->memPart->size) {
        wattron(outputWindow, COLOR_PAIR(RSB->memPart->data[iterator] + 1));
        wprintw(outputWindow, "# ");
    }

    wattron(outputWindow, COLOR_PAIR(1));

    wrefresh(outputWindow);
}

void drawerWorker() {
    size_t iterator;
    static WINDOW *outputWindow = nil;
    static rbool initialized = no;
    if (!initialized) {
        initscr();
        curs_set(yes);

        start_color();
        use_default_colors();

        forAll(iterator, 256) {
            init_pair((short) (iterator + 1), COLOR_BLACK, (short) iterator);
        }

        // set up initial windows
        outputWindow = newwin(SBSize, 2 * SBSize, 0, 0);
        initialized = yes;
    }

    if (outputWindow == nil) {
        RError("Bad output window", nil);
        return;
    }

    while (1) {
        drawMemoryMap(outputWindow);
    }

    // cleanup
    delwin(outputWindow);
    endwin();
}


int main(int argc, const char *argv[]) {
    RThread drawer = makeRThread((RThreadFunction) drawerWorker);
    RSB->allocationMode = RSandBoxAllocationModeStandart;

    enableSandBox(RSB);
    enablePool(RPool);

    stringConstantsTable();
    RCTSingleton;

    RListTest();

    ComplexTest();


//    deleter(stringConstantsTable(), RDictionary);
    deleter(RCTSingleton, RClassTable);

//    p(RAutoPool)(RPool);
//    deleter(RPool, RAutoPool);

    p(RSandBox)(RSB);
    sleep(2);
    deleter(RSB, RSandBox);
//    stopConsole();

    return 0;
}