/*
This file is used to test each function from the Points class.
*/


#include <iostream>
#include <cassert>

#include "splinekit/Points.hpp"

typedef splinekit::PointsT<float> Points;

void printPoints(Points &p){
    Points::Point point;
    while(p >> point){
        std::cout << p.getPtr() << ": ";
        for(int i=0;i<point.getLength();++i){
            std::cout << '(';
            for(int j=0;j<point.getDimensions();++j){
                std::cout << point[i][j] << (j+1 == point.getDimensions() ? "), " : ", ");
            }
        }
        std::cout << std::endl;
    }
}
void printPointsChain(Points &p){
    Points::Point point[4];

    p >> point[0] >> point[1] >> point[2] >> point[3];

    for(int k=0;k<4;++k){
        for(int i=0;i<point[k].getLength();++i){
            std::cout << '(';
            for(int j=0;j<point[k].getDimensions();++j){
                std::cout << point[k][i][j] << (j+1 == point[k].getDimensions() ? "), " : ", ");
            }
        }
        std::cout << std::endl;
    }
}


float p[5][2*3] = {
    {-1.0f,-1.0f,    0.0f,1.0f,    0.1f,-1.0f},
    { 1.0f, 1.0f,   -0.5f,1.0f,    0.2f, 1.0f},
    { 2.0f, 0.0f,    0.0f,1.0f,    0.3f,-1.5f},
    { 3.0f,-1.0f,    0.0f,1.0f,    0.4f, 1.5f},
    { 2.0f,-1.0f,    0.0f,1.0f,    0.5f,-1.1f},
};

int main(){
    std::cout << (std::string)splinekit::LIB_VERSION << " (tested on v0.0.1-a)" << std::endl;
    if(splinekit::LIB_VERSION != splinekit::HEADER_VERSION){
        throw 0;
    }


    Points points{2,3};
    assert(points.stride() == 6 && "2x3 should be 6");

    points.add(p[0]);
    points += p[1];

    printPointsChain(points);

    points << p[2] << p[3] << p[4];
    points.add(points.getPoint(1));

    assert(points.size() == 36 && "Added 6 points, should have 36 floats");
    assert(points.pointCount() == 6 && "Added 6 points, should have 6 points");

    printPoints(points);
    printPoints(points);
    printPointsChain(points);
    printPointsChain(points);
    printPoints(points);

    points.add();
    assert(points.size() == 42 && "Added 7 points, should have 42 floats");
    assert(points.pointCount() == 7 && "Added 7 points, should have 7 points");

    Points::Point p7 = points[6];
    p7[1][1] = 1.0f;
    points.insert(p7, 0);
    p7[1][0] = 2.0f;
    printPoints(points);

    points -= 1;
    points.remove(6);
    printPoints(points);
}