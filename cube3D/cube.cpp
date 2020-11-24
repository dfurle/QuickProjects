#include <stdio.h>
#include <random>
#include <time.h>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <iostream>


#define Vec sf::Vector2f


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
        thickness = 1;
        point[0].position = ln1;
        point[0].color = sf::Color::White;
        point[1].position = ln2;
        point[1].color = sf::Color::White;
        update();
    }
    Line(sf::Vector2f ln1, sf::Color c1, sf::Vector2f ln2, sf::Color c2){
        thickness = 2;
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
    void draw(sf::RenderWindow& window){
        window.draw(line, 4, sf::Quads);
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


class Side{
public:
sf::Vertex points[4];

Side(sf::Vector2f pt1,sf::Vector2f pt2,sf::Vector2f pt3,sf::Vector2f pt4)
{
    points[0] = pt1;
    points[1] = pt2;
    points[2] = pt3;
    points[3] = pt4;
}

Side():
Side(Vec(0,0),Vec(0,0), Vec(0,0), Vec(0,0))
{
}



};


const sf::Vector2u SIZE(1000,800);
float dt;

sf::Vector2f project(float project[2][3], sf::Vector3f point){
    sf::Vector2f projected(0,0);
    projected.x = project[0][0] * point.x + project[0][1] * point.y + project[0][2] * point.z;
    projected.y = project[1][0] * point.x + project[1][1] * point.y + project[1][2] * point.z;
    return projected;
}

sf::Vector3f rotate(float project[3][3], sf::Vector3f point){
    sf::Vector3f projected(0,0,0);
    projected.x = project[0][0] * point.x + project[0][1] * point.y + project[0][2] * point.z;
    projected.y = project[1][0] * point.x + project[1][1] * point.y + project[1][2] * point.z;
    projected.z = project[2][0] * point.x + project[2][1] * point.y + project[2][2] * point.z;
    return projected;
}

float angle = 0.0f;

void init(){
}

int main(int argc, char* argv[]){
    init();
    sf::RenderWindow window(sf::VideoMode(SIZE.x, SIZE.y), "My Game");
    window.setPosition(sf::Vector2i(2600,350));
    window.setFramerateLimit(60);
    sf::Clock clock;
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
        sf::Vector3f points[8];
        points[0] = sf::Vector3f(-0.5, -0.5, -0.5);
        points[1] = sf::Vector3f(0.5, -0.5, -0.5);
        points[2] = sf::Vector3f(0.5, 0.5, -0.5);
        points[3] = sf::Vector3f(-0.5, 0.5, -0.5);

        points[4] = sf::Vector3f(-0.5, -0.5, 0.5);
        points[5] = sf::Vector3f(0.5, -0.5, 0.5);
        points[6] = sf::Vector3f(0.5, 0.5, 0.5);
        points[7] = sf::Vector3f(-0.5, 0.5, 0.5);
        float rot_matX[3][3] = {
            {0,0,1},
            {cos(angle),-sin(angle),0},
            {sin(angle),cos(angle),0}
        };
        float rot_matY[3][3] = {
            {cos(angle),-sin(angle),0},
            {0,0,1},
            {sin(angle),cos(angle),0}
        };
        float rot_matZ[3][3] = {
            {cos(angle),-sin(angle),0},
            {sin(angle),cos(angle),0},
            {0,0,1}
        };
        sf::Vector3f rotated[8];
        sf::Vector2f projected[8];
        for(int i = 0; i < 8; i++){
            rotated[i] = rotate(rot_matX, points[i]);
            rotated[i] = rotate(rot_matY, rotated[i]);
            rotated[i] = rotate(rot_matZ, rotated[i]);
            float scale = 1 / (2 - rotated[i].z);
            float ortho[2][3] = {
                {1,0,0},
                {0,1,0}
            };
            projected[i] = project(ortho, rotated[i]);
            projected[i].x *= 200;
            projected[i].y *= 200;
            projected[i] += sf::Vector2f(SIZE.x/2.f, SIZE.y/2.f);
        }
        std::vector<Line> line;
        for(int i = 0; i < 4; i++){
            line.push_back(Line(sf::Vector2f(projected[i]), sf::Color::Black, sf::Vector2f(projected[(i+1)%4]), sf::Color::Black));
            line.push_back(Line(sf::Vector2f(projected[i+4]), sf::Color::Black, sf::Vector2f(projected[(i+1)%4+4]), sf::Color::Black));
            line.push_back(Line(sf::Vector2f(projected[i]), sf::Color::Black, sf::Vector2f(projected[i+4]), sf::Color::Black));
        }
        
        // sf::Vertex sd[4];
        // sd[0].position = projected[0];
        // sd[1].position = projected[1];
        // sd[2].position = projected[2];
        // sd[3].position = projected[3];
        // for(int q = 0; q < 4; q++)
        //     sd[q].color = sf::Color::Green;

        window.clear(sf::Color::Black);
        int max = 0;
        for(int i = 1; i < 8; i++){
            if(std::max(rotated[max].z, rotated[i].z) == rotated[i].z)
                max = i;
        }

        int draw[3];

        // sf::Vertex sides[6][4];
        Side sides[6];

        sides[0].points[0].position = projected[0];
        sides[0].points[1].position = projected[1];
        sides[0].points[2].position = projected[2];
        sides[0].points[3].position = projected[3];
        for(int q = 0; q < 4; q++)
            sides[0].points[q].color = sf::Color::Green;

        sides[1].points[0].position = projected[4];
        sides[1].points[1].position = projected[5];
        sides[1].points[2].position = projected[6];
        sides[1].points[3].position = projected[7];
        for(int q = 0; q < 4; q++)
            sides[1].points[q].color = sf::Color::Blue;
            
        sides[2].points[0].position = projected[0];
        sides[2].points[1].position = projected[4];
        sides[2].points[2].position = projected[7];
        sides[2].points[3].position = projected[3];
        for(int q = 0; q < 4; q++)
            sides[2].points[q].color = sf::Color::Yellow;

        sides[3].points[0].position = projected[1];
        sides[3].points[1].position = projected[5];
        sides[3].points[2].position = projected[6];
        sides[3].points[3].position = projected[2];
        for(int q = 0; q < 4; q++)
            sides[3].points[q].color = sf::Color::Red;

        sides[4].points[0].position = projected[0];
        sides[4].points[1].position = projected[4];
        sides[4].points[2].position = projected[5];
        sides[4].points[3].position = projected[1];
        for(int q = 0; q < 4; q++)
            sides[4].points[q].color = sf::Color::Magenta;

        sides[5].points[0].position = projected[7];
        sides[5].points[1].position = projected[6];
        sides[5].points[2].position = projected[2];
        sides[5].points[3].position = projected[3];
        for(int q = 0; q < 4; q++)
            sides[5].points[q].color = sf::Color::Cyan;



        Side* p;
        p = std::find_if(sides, sides+6, [=](Side e) {
            for(int i = 0; i < 4; i++){
                if(e.points[i].position.x == projected[max].x && e.points[i].position.y == projected[max].y)
                    return true;
            }
            return false;
        });
        if(p != sides+6){
            // for(int i = 0; i < 4; i++)
            //     p->points[i].color = sf::Color::White;
        }

        // for(Line ln : line)
        //     ln.draw(window);
        for(Side s : sides){
            bool flag = false;
            for(sf::Vertex v : s.points){
                if(v.position == projected[max])
                    flag = true;
            }
            if(flag)
                window.draw(s.points, 4, sf::Quads);
        }
        // window.draw(s.points, 4, sf::Quads);
        
        // for(int i = 0; i < 8; i++){
        //     sf::CircleShape pt(3);
        //     pt.setPosition(projected[i]);
        //     pt.setOrigin(sf::Vector2f(pt.getLocalBounds().width/2.,pt.getLocalBounds().height/2.));
        //     if(i == max){
        //         pt.setFillColor(sf::Color::Red);
        //     }
        //     window.draw(pt);
        // }

        window.display();
        angle += 0.01;

    }

    return 0;
}