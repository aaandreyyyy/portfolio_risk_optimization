#include "App.h"
#include "CmdParser.h"
#include <iostream>


int main(int argc, char **argv) try {
    CmdParser parser;
    if (!parser.Parse(argc, argv)) {
        return 0;
    }
    App app;
    app.Run(parser);
} catch (...) {
    return -1;
}
