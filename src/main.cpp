#include<iostream>
#include<ConwaysGame.h>

int main(int argc, char** argv){
    int scale = 10;
    if(argc > 1){
        scale = std::stoi(argv[1]);
    }
    ConwaysGame game(scale);
    game.run();
	return 0;
}
