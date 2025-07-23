#include <iostream>
#include <fstream>
#include <cassert>

#include "splinekit/WeightManagerMat.hpp"
#include "splinekit/Curve.hpp"
#include "splinekit/Points.hpp"

typedef splinekit::CurveT<float, splinekit::WeightManagerMatT> Curve;
typedef splinekit::PointsT<float> Points;

const std::filesystem::path savesDirectory{"./saves/cache_test"};

// [point index][dimensions * length (parameters)]
float curvePoints1[3][2 * 2] = {
    {-1.0f , -1.0f ,      3.0f ,-4.0f },
    { 1.0f ,  1.0f ,     -2.0f ,-4.0f },
    {-1.0f ,  1.0f ,     -1.0f , 1.0f }
};
float curvePoints2[2][2 * 2] = {
    {-1.0f , -1.0f ,      9.0f ,-16.0f },
    { 1.0f ,  1.0f ,     -4.0f ,-16.0f }
};



int main(){
    std::cout << splinekit::LIB_VERSION.toString() << " (tested on v0.0.3-a)" << std::endl;
    //uint32_t numBase26 = 100;
    //std::string numStr26 = Points::uintToAlpha(numBase26);
    //std::cout << numBase26 << " : " << numStr26 << " : " << Points::alphaToUint(numStr26) << std::endl;

    splinekit::Curve curve1{splinekit::WeightManagerMat::CommonCurve::HERMITE_D1, 2};
    // std::cout << curve.getPoints().length << " parameters, " << curve.getPoints().dimensions << " dimensions" << std::endl;
    assert(curve1.getPoints().dimensions == 2);
    assert(curve1.getPoints().length == 2);
    curve1.addPoint(curvePoints1[0]);
    curve1.addPoint(curvePoints1[1]);
    curve1.addPoint(curvePoints1[2]);

    splinekit::Curve curve2{splinekit::WeightManagerMat::CommonCurve::HERMITE_D1, 2};
    curve2.addPoint(curvePoints2[0]);
    curve2.addPoint(curvePoints2[1]);

    std::vector<Curve::Extrema> velExtrema1;
    std::vector<Curve::Extrema> velExtrema2;
    std::vector<Curve::Extrema> accExtrema1;
    std::vector<Curve::Extrema> accExtrema2;
    std::cout << curve1.calculateLengthDeltaPos(0, 200) << ", " << curve1.calculateLengthIntegralVelLinear(0, 200) << ", " << curve1.calculateLengthIntegralVelQuadratic(0, 200) << ", " << curve1.calculateLengthSimpsons13(0, 200) <<  ", " << curve1.calculateLengthSimpsons38(0, 200) <<  ", " << curve1.calculateLengthBoole(0, 200) << std::endl;
    curve1.calculateAllExtrema(velExtrema1, 1, true, true, true, 0.001, 10000, 100, 0.0001, 0.001, 0.01);
    curve2.calculateAllExtrema(velExtrema2, 1, true, true, true, 0.001, 10000, 100, 0.0001, 0.001, 0.01);
    curve1.calculateAllExtrema(accExtrema1, 2, false, true, true, 0.001, 10000, 100, 0.0001, 0.001, 0.01);
    curve2.calculateAllExtrema(accExtrema2, 2, false, true, true, 0.001, 10000, 100, 0.0001, 0.001, 0.01);

    std::cout << "Velocity extrema1" << std::endl;
    for(auto & p : velExtrema1){
        std::cout << p.t << " (" << p.vec.get()[0] << ", " << p.vec.get()[1] << ")" << std::endl;
    }
    std::cout << "Velocity extrema2" << std::endl;
    for(auto & p : velExtrema2){
        std::cout << p.t << " (" << p.vec.get()[0] << ", " << p.vec.get()[1] << ")" << std::endl;
    }
    std::cout << "Acceleration extrema1" << std::endl;
    for(auto & p : accExtrema1){
        std::cout << p.t << " (" << p.vec.get()[0] << ", " << p.vec.get()[1] << ")" << std::endl;
    }
    std::cout << "Acceleration extrema2" << std::endl;
    for(auto & p : accExtrema2){
        std::cout << p.t << " (" << p.vec.get()[0] << ", " << p.vec.get()[1] << ")" << std::endl;
    }
}