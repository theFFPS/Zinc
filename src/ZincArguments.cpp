#include <ZincArguments.h>
#include <ZincConstants.h>
#include <iostream>
#include <cstring>

namespace zinc {

ZincArguments g_zincArguments;

void ZincArguments::parse(int argc, char **argv) {
    if (argc < 2) return;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "--debug")) {
            i++;
            if (i != argc) m_debug = !strcmp(argv[i], "on");
        } else if (!strcmp(argv[i], "--workdir")) {
            i++;
            if (i != argc) m_workDir = argv[i];
        } else if (!strcmp(argv[i], "--help")) {
            std::cout << "zinc\n";
            std::cout << "syntax \"" << argv[0] << " [args...]\"\n";
            std::cout << "options\n";
            std::cout << "    --debug on/off                 enable/disable debug mode\n";
            std::cout << "    --workdir path                 specify custom workdir path\n";
            std::cout << "    --help                         display this message\n";
            std::cout << "    --version                      display version\n";
            exit(0);
        } else if (!strcmp(argv[i], "--version")) {
            std::cout << "zinc\n";
            std::cout << "zinc version " << ZINC_VERSION << "\n";
            std::cout << "minecraft version " << LATEST_MINECRAFT_VERSION << " (" << LATEST_MINECRAFT_VERSION_PROTOCOL << ")\n";
            exit(0);
        } else {
            std::cout << "zinc\n";
            std::cout << "undefined argument \"" << argv[i] << "\"\n";
            std::cout << "use \"" << argv[0] << " --help\" for help\n";
            exit(1);
        }
    }
}

}