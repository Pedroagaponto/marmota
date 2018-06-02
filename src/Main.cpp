#include <fstream>
#include <iostream>
#include "Game.h"
#include "StageState.h"
#include "TitleState.h"

int main(int argc, char** argv) {
#ifdef _WIN32
    std::ofstream out("cout.txt");
    std::streambuf* coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    std::ofstream err("cerr.txt");
    std::streambuf* cerrbuf = std::cerr.rdbuf();
    std::cerr.rdbuf(err.rdbuf());
#endif

    Game* game = Game::GetInstance("Groundhog Dig", 1024, 768);
    game->Push(new TitleState());
    game->Run();

#ifdef _WIN32
    std::cout.rdbuf(coutbuf);
    std::cerr.rdbuf(cerrbuf);
#endif

    return 0;
}
