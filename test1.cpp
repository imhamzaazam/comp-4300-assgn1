#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector> 

int main(int argc, char* argv[]){
    const int wWidth = 640;
    const int wHeight = 480;
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
    
    sf::CircleShape circle(50);
    circle.setFillColor(sf::Color::Green);
    circle.setPosition(300.0f, 300.0f);
    float circleMovespeedX = 0.05f;
    float circleMovespeedY = 0.05f;
    sf::Font myFont;
    if (!myFont.loadFromFile("fonts/KillerTech.ttf")){
        std::cerr << "Could not load font\n";
        exit(-1);
    }

    sf::Text text("Sample text", myFont, 24);
    text.setPosition(sf::Vector2f(0.0f, wHeight - (float)text.getCharacterSize()));

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
           
        }
        circle.setPosition(circle.getPosition().x -circleMovespeedX, 
        circle.getPosition().y - circleMovespeedY); 
        
        if (circle.getGlobalBounds().left < 0 || circle.getGlobalBounds().left + circle.getGlobalBounds().width > wWidth)
        {
            circleMovespeedX *= -1;
        }
        if (circle.getGlobalBounds().top < 0 || circle.getGlobalBounds().top + (circle.getRadius()*2) > wHeight){
            circleMovespeedY *= -1;
        }
      
        
        
        window.clear();
        window.draw(circle);
        window.draw(text);
        window.display();
    }

   

}