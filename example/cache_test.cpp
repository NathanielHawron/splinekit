#include "splinekit/WeightManagerMat.hpp"
#include "splinekit/Curve.hpp"

#include <iostream>


int main(){
    std::cout << splinekit::versionString() << " (tested on v0.0.1-a)" << std::endl;

    splinekit::WeightManagerMat::printWeightDataDescription();

    float res[2];
    
    splinekit::Curve bezier_2{splinekit::WeightManagerMat::CommonCurve::BEZIER_O3, 2};
    
    float P_b2[2][2 * 3] = {
        {-1.0f,-1.0f,   -0.75f,0.0f,    0.0f,1.0f},
        { 1.0f, 1.0f,    0.0f, 0.0f,    0.0f,0.0f},
    };
    
    bezier_2.addPoint(P_b2[0]);
    bezier_2.addPoint(P_b2[1]);

    std::cout << "bezier_2" << std::endl;
    bezier_2.getWeightManager().get()->printWeightData();
    bezier_2.getWeightManager().get()->printMatrix();
    for(float i=0;i<1.0f;i+=0.125f){
        bezier_2.calculatePoint(res, i);
        std::cout << "(" << res[0] << ", " << res[1] << "), ";
    }
    bezier_2.calculatePoint(res, 1.0f);
    std::cout << "(" << res[0] << ", " << res[1] << ")" << std::endl;



    splinekit::Curve hermite_1{splinekit::WeightManagerMat::CommonCurve::HERMITE_D1, 2};
    float P_h1[2][2 * 2] = {
        {-1.0f,-1.0f,    0.25f,2.0f},
        { 1.0f, 0.0f,   -0.25f,2.5f},
    };
    
    hermite_1.addPoint(P_h1[0]);
    hermite_1.addPoint(P_h1[1]);

    std::cout << "hermite_1" << std::endl;
    hermite_1.getWeightManager().get()->printWeightData();
    hermite_1.getWeightManager().get()->printMatrix();
    for(float i=0;i<2.0f;i+=0.125f){
        hermite_1.calculatePoint(res, i);
        std::cout << "(" << res[0] << ", " << res[1] << "), ";
    }
    hermite_1.calculatePoint(res, 2.0f);
    std::cout << "(" << res[0] << ", " << res[1] << ")" << std::endl;
}