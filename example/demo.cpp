#include "splinekit/WeightManagerMat.hpp"
#include "splinekit/Curve.hpp"

#include <iostream>

// Bezier parameters
float bezier2_a_weights[1][4] = {
    {0.0f,0.0f,-1.0f,0.0f}
};
splinekit::WeightManagerMatT<float>::Replacement bezier2_a[1] = {
    {2, 1.0f, bezier2_a_weights[0]}
};
splinekit::WeightManagerMatT<float>::RowReplacement bezier2_row[1] = {
    {2,3}
};

splinekit::WeightManagerMatT<float>::CreateInfo bezier2_createInfo = {
    3,
    0, nullptr,
    1, bezier2_a,
    1, bezier2_row,
    0, nullptr
};

// Hermite parameters
splinekit::WeightManagerMatT<float>::DerivativeReplacement hermite1_derivative[2] = {
    {1, 0.0f, 1},
    {2, 1.0f, 1}
};
splinekit::WeightManagerMatT<float>::RowReplacement hermite1_row[1] = {
    {2,3}
};
splinekit::WeightManagerMatT<float>::DerivativeReplacement hermite2_derivative[4] = {
    {1, 0.0f, 1},
    {2, 0.0f, 2},
    {3, 1.0f, 2},
    {4, 1.0f, 1}
};
splinekit::WeightManagerMatT<float>::RowReplacement hermite2_row[1] = {
    {3,5}
};

splinekit::WeightManagerMatT<float>::CreateInfo hermite1_createInfo = {
    3,
    2, hermite1_derivative,
    0, nullptr,
    1, hermite1_row,
    0, nullptr
};
splinekit::WeightManagerMatT<float>::CreateInfo hermite2_createInfo = {
    5,
    4, hermite2_derivative,
    0, nullptr,
    1, hermite2_row,
    0, nullptr
};

// Catmull-rom parameters
float catmull1_a_weights[2][4] = {
    {0,-0.5,0,1},
    {1,0,0.5,0}
};
splinekit::WeightManagerMatT<float>::Replacement catmull1_a[2] = {
    {1, -1.0f, catmull1_a_weights[0]},
    {2, 2.f, catmull1_a_weights[1]}
};
splinekit::WeightManagerMatT<float>::RowReplacement catmull1_row[2] = {
    {0,1},
    {2,3}
};
splinekit::WeightManagerMatT<float>::Remap catmull1_remap[4] = {
    {0,{0,-1}},
    {1,{0,0}},
    {2,{0,1}},
    {3,{0,2}}
};
splinekit::WeightManagerMatT<float>::CreateInfo catmull1_createInfo = {
    3,
    2, hermite1_derivative,
    2, catmull1_a,
    2, catmull1_row,
    4, catmull1_remap
};

// Cardinal parameters
float cardinal1_a_weights[2][4] = {
    {0,-1,0,1},
    {1,0,1,0}
};
float cardinal2_a_weights[2][4] = {
    {0,-0.25,0,1},
    {1,0,0.25,0}
};
splinekit::WeightManagerMatT<float>::Replacement cardinal1_a[2] = {
    {1, -1.0f, cardinal1_a_weights[0]},
    {2, 2.f, cardinal1_a_weights[1]}
};
splinekit::WeightManagerMatT<float>::Replacement cardinal2_a[2] = {
    {1, -1.0f, cardinal2_a_weights[0]},
    {2, 2.f, cardinal2_a_weights[1]}
};
splinekit::WeightManagerMatT<float>::CreateInfo cardinal1_createInfo = {
    3,
    2, hermite1_derivative,
    2, cardinal1_a,
    2, catmull1_row,
    4, catmull1_remap
};
splinekit::WeightManagerMatT<float>::CreateInfo cardinal2_createInfo = {
    3,
    2, hermite1_derivative,
    2, cardinal2_a,
    2, catmull1_row,
    4, catmull1_remap
};
// Reduced spline parameters
float reduced1_a_weights[2][4] = {
    {0,-1,1,0},
    {0,0,0,0}
};
splinekit::WeightManagerMatT<float>::Replacement reduced1_a[2] = {
    {2, 0.5f, reduced1_a_weights[0]},
    {2, 0.5f, reduced1_a_weights[1]}
};
splinekit::WeightManagerMatT<float>::RowReplacement reduced1_row[1] = {
    {2,3}
};
splinekit::WeightManagerMatT<float>::Remap reduced1_remap[4] = {
    {0,{0,0}},
    {1,{0,1}},
    {2,{1,0}},
    {3,{0,1}}
};

splinekit::WeightManagerMatT<float>::CreateInfo reduced1_createInfo = {
    3,
    0, nullptr,
    2, reduced1_a,
    1, reduced1_row,
    4, reduced1_remap
};

typedef splinekit::WeightManagerMatT<float> WM_t;
typedef splinekit::CurveT<float, splinekit::WeightManagerMatT> Curve_t;

int main(){
    std::cout << splinekit::versionString() << " (tested on v0.0.0-a)" << std::endl;

    splinekit::WeightManager<float>::printWeightDataDescription();

    Curve_t bezier_0{WM_t::CommonCurve::BEZIER_O1, 2, 1};
    
    float P_b0[3][2] = {
        {-1.0f,-1.0f},
        { 0.0f, 1.0f},
        { 1.0f, 0.5f}
    };
    
    bezier_0.addPoint(P_b0[0]);
    bezier_0.addPoint(P_b0[1]);
    bezier_0.addPoint(P_b0[2]);

    float res[2];

    std::cout << "bezier_0" << std::endl;
    bezier_0.getWeightManager().get()->printWeightData();
    bezier_0.getWeightManager().get()->printMatrix();
    for(float i=0;i<3.0f;i+=0.5f){
        bezier_0.calculatePoint(res, i);
        std::cout << "(" << res[0] << ", " << res[1] << "), ";
    }
    bezier_0.calculatePoint(res, 3.0f);
    std::cout << "(" << res[0] << ", " << res[1] << ")" << std::endl;
    
    Curve_t bezier_2{WM_t::CommonCurve::BEZIER_O3, 2};

    float P_b2[2][2 * 3] = {
        {-1.0f,-1.0f,   -0.75f,0.0f,    0.0f,1.0f},
        { 1.0f, 1.0f,    0.0f, 0.0f,    0.0f,0.0f},
    };
    
    bezier_2.addPoint(P_b2[0]);
    bezier_2.addPoint(P_b2[1]);

    std::cout << "bezier_2" << std::endl;
    bezier_2.getWeightManager().get()->printWeightData();
    bezier_2.getWeightManager().get()->printMatrix();
    for(float i=0;i<1.0f;i+=0.25f){
        bezier_2.calculatePoint(res, i);
        std::cout << "(" << res[0] << ", " << res[1] << "), ";
    }
    bezier_2.calculatePoint(res, 1.0f);
    std::cout << "(" << res[0] << ", " << res[1] << ")" << std::endl;


    /*splinekit::WeightManagerMat<float> hermite_1{hermite1_createInfo};
    splinekit::WeightManagerMat<float> hermite_2{hermite2_createInfo};
    splinekit::WeightManagerMat<float> hermite_2_common{splinekit::WeightManagerMat<float>::CommonCurve::HERMITE_D2};

    std::cout << "hermite_1" << std::endl;
    hermite_1.printWeightData();
    hermite_1.printMatrix();
    std::cout << "hermite_2" << std::endl;
    hermite_2.printWeightData();
    hermite_2.printMatrix();
    std::cout << "hermite_2_common" << std::endl;
    hermite_2.printWeightData();
    hermite_2.printMatrix();
    
    splinekit::WeightManagerMat<float> catmull_1{catmull1_createInfo};

    std::cout << "catmull_1" << std::endl;
    catmull_1.printWeightData();
    catmull_1.printMatrix();

    splinekit::WeightManagerMat<float> cardinal_1{cardinal1_createInfo};
    splinekit::WeightManagerMat<float> cardinal_2{cardinal2_createInfo};

    std::cout << "cardinal_1" << std::endl;
    cardinal_1.printWeightData();
    cardinal_1.printMatrix();
    std::cout << "cardinal_2" << std::endl;
    cardinal_2.printWeightData();
    cardinal_2.printMatrix();

    splinekit::WeightManagerMat<float> reduced_1{reduced1_createInfo};

    std::cout << "reduced_1" << std::endl;
    reduced_1.printWeightData();
    reduced_1.printMatrix();*/
}