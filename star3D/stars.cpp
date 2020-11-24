#include <stdio.h>
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <math.h>


class Line{
public:
    sf::Vertex line[4];
    sf::Vertex point[2];
    float thickness;

public:
    Line(){
        thickness = 1;
        point[0].position = sf::Vector2f(0.f,0.f);
        point[0].color = sf::Color::White;
        point[1].position = sf::Vector2f(0.f,0.f);
        point[1].color = sf::Color::White;
        update();
    }
    Line(sf::Vector2f ln1, sf::Vector2f ln2){
        thickness = 50;
        point[0].position = ln1;
        point[0].color = sf::Color::White;
        point[1].position = ln2;
        point[1].color = sf::Color::White;
        update();
    }
    Line(sf::Vector2f ln1, sf::Color c1, sf::Vector2f ln2, sf::Color c2){
        thickness = 1;
        point[0].position = ln1;
        point[0].color = c1;
        point[1].position = ln2;
        point[1].color = c2;
        update();
    }
    void setPosition(sf::Vector2f ln1, sf::Vector2f ln2){
        point[0].position = ln1;
        point[1].position = ln2;
        update();
    }
    void setThickess(float thick){
        thickness = thick;
        update();
    }
private:
    void update(){
        sf::Vector2f diff(point[0].position.x-point[1].position.x,point[0].position.y-point[1].position.y);
        sf::Vector2f df_thick, df_thick2;
        float alpha = atan2(diff.y, diff.x) + M_PI_2;
        df_thick = sf::Vector2f(cos(alpha) * thickness/2.f, sin(alpha) * thickness/2.f);
        line[0].position = point[1].position + df_thick;
        line[0].color = point[1].color;
        line[1].position = point[1].position - df_thick;
        line[1].color = point[1].color;
        line[2].position = point[0].position - df_thick;
        line[2].color = point[0].color;
        line[3].position = point[0].position + df_thick;
        line[3].color = point[0].color;
    }
    

};

const size_t num_of_stars = 400;
const sf::Vector2u SIZE(1000,800);
const int tail = 1000;
const float step = 20;

float dt;

sf::Vector3f current_pos[num_of_stars];
sf::Vector3f tail_pos[num_of_stars];

void init(){
    srand(time(NULL));
    for(auto i = 0; i < num_of_stars; i++){
        sf::Vector3f pos(rand() % SIZE.x - SIZE.x/2., rand() % SIZE.y - SIZE.y/2., rand() % SIZE.x);
        current_pos[i] = pos;
        tail_pos[i] = pos;
        tail_pos[i].z += tail;
    }
}

int main(int argc, char* argv[]){
    init();
    sf::RenderWindow window(sf::VideoMode(SIZE.x, SIZE.y), "My Game");
    window.setPosition(sf::Vector2i(2600,350));
    window.setFramerateLimit(60);
    sf::Clock clock;
    Line line(sf::Vector2f(SIZE.x/2.f - 100.f, SIZE.y/2.f + 50.f), sf::Vector2f(SIZE.x/2.f + 100.f, SIZE.y/2.f - 50.f));
    while(window.isOpen()){
        dt = clock.restart().asSeconds();
        sf::Event event;
        while (window.pollEvent(event)){
            switch(event.type){
            case sf::Event::Closed:
                window.close();
                break;

            }

        }
        window.clear(sf::Color::Black);
        sf::Vertex ln[2];
        for(auto i = 0; i < num_of_stars; i++){
            sf::Vector2f pos1(
                (current_pos[i].x/current_pos[i].z)*SIZE.x+SIZE.x/2.f,
                (current_pos[i].y/current_pos[i].z)*SIZE.y+SIZE.y/2.f);
            sf::Vector2f pos2(
                (tail_pos[i].x/tail_pos[i].z)*SIZE.x+SIZE.x/2.f,
                (tail_pos[i].y/tail_pos[i].z)*SIZE.y+SIZE.y/2.f);
            float alpha = sqrt((SIZE.x/6.f)/current_pos[i].z)*255;
            Line line(pos1, sf::Color(255,255, 255, alpha), pos2, sf::Color(255, 255, 255, 0));
            line.setThickess(3.f);
            current_pos[i].z -= step;
            tail_pos[i].z -= step;
            if(current_pos[i].z <= 1){
                current_pos[i].x = rand() % SIZE.x - SIZE.x/2.f;
                current_pos[i].y = rand() % SIZE.y - SIZE.y/2.f;
                current_pos[i].z = rand() % SIZE.x;
                tail_pos[i].x = current_pos[i].x;
                tail_pos[i].y = current_pos[i].y;
                tail_pos[i].z = current_pos[i].z + tail;
            }
            window.draw(line.line, 4, sf::Quads);
        }
        window.display();
    }

    return 0;
}