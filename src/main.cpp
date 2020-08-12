#include<iostream>
#include<ConwaysGame.h>

int main(int argc, char** argv){
    int scale = 10;
    for(int i = 0; i < argc; i++){
        std::cout << argv[i] << " ";
    }
    if(argc > 1){
        scale = std::stoi(argv[1]);
    }
    ConwaysGame game(1300, 660, scale, 30);
    game.run();
	return 0;
}
