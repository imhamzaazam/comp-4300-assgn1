#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector> 


class AShape {
    public:
    sf::CircleShape circleShape;
    sf::RectangleShape rectangleShape;
    sf::Vector2f speed;
    sf::Text text;
    sf::Vector2f position;

    AShape(float circleRadius): circleShape(circleRadius){}

    AShape(float rectWidth, float rectHeight) 
        : rectangleShape(sf::Vector2f(rectWidth, rectHeight)){}

     void setCirclePosition(const sf::Vector2f& pos) {
        circleShape.setPosition(pos);
    }

    void setRectanglePosition(const sf::Vector2f& pos) {
        rectangleShape.setPosition(pos);
    }

    bool isCircleBounding(int wWidth, int wHeight){
        if(circleShape.getGlobalBounds().left < 0 || circleShape.getGlobalBounds().left 
        + circleShape.getGlobalBounds().width > wWidth){
            return true;
        }
        if (circleShape.getGlobalBounds().top < 0 || circleShape.getGlobalBounds().top + (circleShape.getRadius()*2) > wHeight){
            return true;
        }
      

    }
    


};


class MyShapes {
    public: 
        std::vector<AShape *> shapes;
};

class Window{
    public:
    int wWidth;
    int wHeight;
};
class RGB{
    public:
    int red;
    int green;
    int blue;
};

class GameConfig{
    public:
    Window window;
    RGB rgb;
    int fontSize; 
    std::string fontPath;
    sf::Font myFont;
    sf::Text text;
    std::vector<sf::RectangleShape> myrectangleShapes;
    std::vector<sf::CircleShape> mycircleShapes;
    MyShapes myShapes;
    float shapePositionX, shapePositionY;
    std::string shapeName;
    float shapeSpeedX, shapeSpeedY;
    float shapeWidth, shapeHeight;
    float radius;

    
    GameConfig(){}
    void readFromFile(const std::string& filename){
        std::ifstream fin(filename); 
        std::string property;

        while(fin>>property){
             if(property == "Window"){
                fin>>window.wWidth>>window.wHeight;
             }
             if(property == "Font"){
                fin>>fontPath;
                if (!myFont.loadFromFile(fontPath)){
                    std::cerr << "Could not load font\n";
                    exit(-1);
                }
                fin>>fontSize>>rgb.red>>rgb.green>>rgb.blue;
                text.setFont(myFont);
                text.setCharacterSize(fontSize);
                text.setFillColor(sf::Color(rgb.red, rgb.green, rgb.blue));
             }
             if(property == "Circle"){
                fin >> shapeName >> shapePositionX >> shapePositionY >> shapeSpeedX >> shapeSpeedY 
                >> rgb.red >> rgb.green >>rgb.blue >> radius;
                AShape* mycircleptr = new AShape(radius);
                mycircleptr->circleShape.setPosition(shapePositionX, shapePositionY);
                mycircleptr->circleShape.setFillColor(sf::Color(rgb.red, rgb.green, rgb.blue));
                mycircleptr->speed = sf::Vector2f(shapeSpeedX, shapeSpeedY); // Fixed here
                mycircleptr->text.setFont(myFont);
                mycircleptr->text.setCharacterSize(fontSize);
                mycircleptr->text.setString(shapeName);
                
                mycircleptr->text.setPosition(
                    mycircleptr->circleShape.getPosition().x + mycircleptr->circleShape.getRadius() - (mycircleptr->text.getLocalBounds().width/2),
                    mycircleptr->circleShape.getPosition().y + mycircleptr->circleShape.getRadius() - (mycircleptr->text.getLocalBounds().height/2)
                );
                myShapes.shapes.push_back(mycircleptr);
             }
             if(property == "Rectangle"){

                fin >> shapeName >>shapePositionX >> shapePositionY >> shapeSpeedX >> shapeSpeedY 
                >> rgb.red >> rgb.green >> rgb.blue >> shapeWidth >> shapeHeight;
                AShape* myRectangleptr = new AShape(shapeWidth, shapeHeight);
               
                myRectangleptr->rectangleShape.setPosition(shapePositionX, shapePositionY);
                myRectangleptr->rectangleShape.setFillColor(sf::Color(rgb.red, rgb.green, rgb.blue));
                myRectangleptr->speed = sf::Vector2f(shapeSpeedX, shapeSpeedY); 
                myRectangleptr->text.setFont(myFont);
                myRectangleptr->text.setString(shapeName);


                myRectangleptr->text.setPosition(myRectangleptr->rectangleShape.getPosition().x +
                (myRectangleptr->rectangleShape.getLocalBounds().width/2) - (myRectangleptr->text.getLocalBounds().width/2),
                myRectangleptr->rectangleShape.getPosition().y +
                (myRectangleptr->rectangleShape.getLocalBounds().height/2) - 
                (myRectangleptr->text.getLocalBounds().height/2));
                std::cout << myRectangleptr->text.getLocalBounds().height << "\n";
                std::cout << myRectangleptr->rectangleShape.getLocalBounds().height << "\n";



                myShapes.shapes.push_back(myRectangleptr);
             }
             
           
        } 

    }
};



int main()
{
    GameConfig gameConfig;
    gameConfig.readFromFile("config.txt");
   
    const int wWidth = gameConfig.window.wWidth;
    const int wHeight = gameConfig.window.wHeight;
    
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
    window.setFramerateLimit(120);

    sf::Font myFont;
    
    if (!myFont.loadFromFile("fonts/KillerTech.ttf")){
        std::cerr << "Could not load font\n";
        exit(-1);
    }

   
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed){
                window.close();
            }
            if(event.type == sf::Event::KeyPressed){
                std::cout << "Key pressed with Code" << event.key.code << "\n";
            }
            
        }

        // for(auto& shape: gameConfig.myShapes.shapes){
            
        //     shape->circleShape.setPosition(shape->circleShape.getPosition().x-shape->speed.x,
        //     shape->circleShape.getPosition().y - shape->speed.y );
        //     if(shape->circleShape.getRadius()>0){
        //         shape->text.setPosition(shape->circleShape.getPosition());  
            
        //     }

        //     shape->rectangleShape.setPosition(shape->rectangleShape.getPosition().x-shape->speed.x,
        //     shape->rectangleShape.getPosition().y- shape->speed.y);
        //     if(shape->rectangleShape.getLocalBounds().height > 0){
        //         shape->text.setPosition(shape->rectangleShape.getPosition());
        //     }
            
            
        // }
        window.clear();

        for(auto& shape: gameConfig.myShapes.shapes){
            window.draw(shape->circleShape);
            window.draw(shape->rectangleShape);
        }

        for (auto& shape: gameConfig.myShapes.shapes){
            window.draw(shape->text);
        }
        
        
        window.display();
    }

    return 0;
}

