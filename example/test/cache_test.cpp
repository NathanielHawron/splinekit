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
float curvePoints[3][2 * 2] = {
    {-1.0f , -1.0f ,      2.0f , 0.5f },
    { 1.0f ,  1.0f ,      0.5f , 2.0f },
    {-1.0f ,  1.0f ,     -1.0f , 1.0f }
};

int main(){
    std::cout << splinekit::LIB_VERSION.toString() << " (tested on v0.0.2-a)" << std::endl;
    //uint32_t numBase26 = 100;
    //std::string numStr26 = Points::uintToAlpha(numBase26);
    //std::cout << numBase26 << " : " << numStr26 << " : " << Points::alphaToUint(numStr26) << std::endl;

    splinekit::Curve curve{splinekit::WeightManagerMat::CommonCurve::HERMITE_D1, 2};
    (dynamic_cast<const splinekit::WeightManagerMat*>(curve.getWeightManager().get()))->printMatrix();
    // std::cout << curve.getPoints().length << " parameters, " << curve.getPoints().dimensions << " dimensions" << std::endl;
    assert(curve.getPoints().dimensions == 2);
    assert(curve.getPoints().length == 2);
    curve.addPoint(curvePoints[0]);
    curve.addPoint(curvePoints[1]);
    curve.addPoint(curvePoints[2]);

    std::filesystem::create_directory(savesDirectory.parent_path());
    std::filesystem::create_directory(savesDirectory);
    curve.getPoints().saveAsCSV(savesDirectory / "control.csv");
    curve.getPoints().saveAsTXT(savesDirectory / "control.txt", ", ");

    splinekit::Points sample10{2,1};
    curve.calculatePointsLinear(sample10, 10, 0);
    sample10.saveAsCSV(savesDirectory / "sample10.csv");
    sample10.saveAsTXT(savesDirectory / "sample10.txt", ", ");

    splinekit::Points sample100{2,1};
    curve.calculatePointsLinear(sample100, 100, 0);
    sample100.saveAsCSV(savesDirectory / "sample100.csv");
    sample100.saveAsTXT(savesDirectory / "sample100.txt", ", ");

    splinekit::Points sample10Vel{2,2};
    curve.calculatePointsLinear(sample10Vel, 10, 0, 0);
    curve.calculatePointsLinear(sample10Vel, 10, 1, 1);
    sample10Vel.saveAsCSV(savesDirectory / "sample10Vel.csv");
    sample10Vel.saveAsTXT(savesDirectory / "sample10Vel.txt", ", ");

    splinekit::Points sample20Jerk{2,4};
    curve.calculatePointsLinear(sample20Jerk, 20, 0, 0);
    curve.calculatePointsLinear(sample20Jerk, 20, 1, 1);
    curve.calculatePointsLinear(sample20Jerk, 20, 2, 2);
    curve.calculatePointsLinear(sample20Jerk, 20, 3, 3);
    sample20Jerk.saveAsCSV(savesDirectory / "sample20Jerk.csv");
    sample20Jerk.saveAsTXT(savesDirectory / "sample20Jerk.txt", ", ");
}