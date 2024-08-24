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


};


class MyShapes {
    public: 
        std::vector<std::shared_ptr<AShape>> shapes;
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
    uint16_t fontSize; 
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
                auto mycircleptr = std::make_shared<AShape>(radius);
                mycircleptr->circleShape.setPosition(shapePositionX, shapePositionY);
                mycircleptr->circleShape.setFillColor(sf::Color(rgb.red, rgb.green, rgb.blue));
                mycircleptr->speed = sf::Vector2f(shapeSpeedX, shapeSpeedY); // Fixed here
                mycircleptr->text.setFont(myFont);
                mycircleptr->text.setString(shapeName);
                
                myShapes.shapes.push_back(mycircleptr);
             }
             if(property == "Rectangle"){

                fin >> shapeName >>shapePositionX >> shapePositionY >> shapeSpeedX >> shapeSpeedY 
                >> rgb.red >> rgb.green >> rgb.blue >> shapeWidth >> shapeHeight;
                auto myRectangleptr = std::make_shared<AShape>(shapeWidth, shapeHeight);
                myRectangleptr->rectangleShape.setPosition(shapePositionX, shapePositionY);
                myRectangleptr->rectangleShape.setFillColor(sf::Color(rgb.red, rgb.green, rgb.blue));
                myRectangleptr->speed = sf::Vector2f(shapeSpeedX, shapeSpeedY); 
                myRectangleptr->text.setFont(myFont);
                myRectangleptr->text.setString(shapeName);
                myShapes.shapes.push_back(myRectangleptr);
             }
             
           
        } 

    }
};



int main()
{
    GameConfig gameConfig;
    gameConfig.readFromFile("bin/config.txt");
   
    const int wWidth = gameConfig.window.wWidth;
    const int wHeight = gameConfig.window.wHeight;
    
    sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "SFML Works!");
    window.setFramerateLimit(60);

   
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

        for(auto& shape: gameConfig.myShapes.shapes){
            
            shape->circleShape.setPosition(shape->circleShape.getPosition().x-shape->speed.x,
            shape->circleShape.getPosition().y - shape->speed.y );

            if(shape->circleShape.getRadius()>0){
                shape->text.setPosition(
                    shape->circleShape.getPosition().x + shape->circleShape.getLocalBounds().width/2 - shape->text.getLocalBounds().width/2,
                    shape->circleShape.getPosition().y + shape->circleShape.getLocalBounds().height/2 - shape->text.getLocalBounds().height/2
                );  
                float shapeX = shape->circleShape.getPosition().x;
                float shapeWidth = shape->circleShape.getLocalBounds().width;
                if(shapeX < 0 || shapeX + shapeWidth > gameConfig.window.wWidth){
                    shape->speed.x *= -1.0f;
                }
                float shapeY  = shape->circleShape.getPosition().y;
                float shapeHeight = shape->circleShape.getLocalBounds().height;
                if (shapeY < 0 || shapeY + shapeHeight > gameConfig.window.wHeight){
                    shape->speed.y *=-1.0f;
                }
            }

            shape->rectangleShape.setPosition(shape->rectangleShape.getPosition().x-shape->speed.x,
            shape->rectangleShape.getPosition().y- shape->speed.y);
            if(shape->rectangleShape.getLocalBounds().height > 0){
                shape->text.setPosition(
                    shape->rectangleShape.getPosition().x + shape->rectangleShape.getLocalBounds().width/2 - shape->text.getLocalBounds().width/2,
                    shape->rectangleShape.getPosition().y + shape->rectangleShape.getLocalBounds().height/2 - shape->text.getLocalBounds().height/2
                );
                float shapeX = shape->rectangleShape.getPosition().x;
                float shapeWidth = shape->rectangleShape.getLocalBounds().width;
                if(shapeX < 0 || shapeX + shapeWidth > gameConfig.window.wWidth){
                    shape->speed.x *= -1.0f;
                }
                float shapeY  = shape->rectangleShape.getPosition().y;
                float shapeHeight = shape->rectangleShape.getLocalBounds().height;
                if (shapeY < 0 || shapeY + shapeHeight > gameConfig.window.wHeight){
                    shape->speed.y *=-1.0f;
                }
                
            }

            
            
            
        }
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

