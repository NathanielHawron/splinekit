#include "splinekit/WeightManagerMat.hpp"
#include "splinekit/Curve.hpp"

#include <iostream>

// Bezier parameters
float bezier2_a_weights[1][4] = {
    {0.0f,0.0f,-1.0f,0.0f}
};
splinekit::WeightManagerMat<float>::Replacement bezier2_a[1] = {
    {2, 1.0f, bezier2_a_weights[0]}
};
splinekit::WeightManagerMat<float>::RowReplacement bezier2_row[1] = {
    {2,3}
};

splinekit::WeightManagerMat<float>::CreateInfo bezier2_createInfo = {
    3,
    0, nullptr,
    1, bezier2_a,
    1, bezier2_row,
    0, nullptr
};

// Hermite parameters
splinekit::WeightManagerMat<float>::DerivativeReplacement hermite1_derivative[2] = {
    {1, 0.0f, 1},
    {2, 1.0f, 1}
};
splinekit::WeightManagerMat<float>::RowReplacement hermite1_row[1] = {
    {2,3}
};
splinekit::WeightManagerMat<float>::DerivativeReplacement hermite2_derivative[4] = {
    {1, 0.0f, 1},
    {2, 0.0f, 2},
    {3, 1.0f, 2},
    {4, 1.0f, 1}
};
splinekit::WeightManagerMat<float>::RowReplacement hermite2_row[1] = {
    {3,5}
};

splinekit::WeightManagerMat<float>::CreateInfo hermite1_createInfo = {
    3,
    2, hermite1_derivative,
    0, nullptr,
    1, hermite1_row,
    0, nullptr
};
splinekit::WeightManagerMat<float>::CreateInfo hermite2_createInfo = {
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
splinekit::WeightManagerMat<float>::Replacement catmull1_a[2] = {
    {1, -1.0f, catmull1_a_weights[0]},
    {2, 2.f, catmull1_a_weights[1]}
};
splinekit::WeightManagerMat<float>::RowReplacement catmull1_row[2] = {
    {0,1},
    {2,3}
};
splinekit::WeightManagerMat<float>::Remap catmull1_remap[4] = {
    {0,{0,-1}},
    {1,{0,0}},
    {2,{0,1}},
    {3,{0,2}}
};
splinekit::WeightManagerMat<float>::CreateInfo catmull1_createInfo = {
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
splinekit::WeightManagerMat<float>::Replacement cardinal1_a[2] = {
    {1, -1.0f, cardinal1_a_weights[0]},
    {2, 2.f, cardinal1_a_weights[1]}
};
splinekit::WeightManagerMat<float>::Replacement cardinal2_a[2] = {
    {1, -1.0f, cardinal2_a_weights[0]},
    {2, 2.f, cardinal2_a_weights[1]}
};
splinekit::WeightManagerMat<float>::CreateInfo cardinal1_createInfo = {
    3,
    2, hermite1_derivative,
    2, cardinal1_a,
    2, catmull1_row,
    4, catmull1_remap
};
splinekit::WeightManagerMat<float>::CreateInfo cardinal2_createInfo = {
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
splinekit::WeightManagerMat<float>::Replacement reduced1_a[2] = {
    {2, 0.5f, reduced1_a_weights[0]},
    {2, 0.5f, reduced1_a_weights[1]}
};
splinekit::WeightManagerMat<float>::RowReplacement reduced1_row[1] = {
    {2,3}
};
splinekit::WeightManagerMat<float>::Remap reduced1_remap[4] = {
    {0,{0,0}},
    {1,{0,1}},
    {2,{1,0}},
    {3,{0,1}}
};

splinekit::WeightManagerMat<float>::CreateInfo reduced1_createInfo = {
    3,
    0, nullptr,
    2, reduced1_a,
    1, reduced1_row,
    4, reduced1_remap
};

int main(){
    splinekit::WeightManager<float>::printWeightDataDescription();
    
    splinekit::WeightManagerMat<float> bezier_0{{1}};
    splinekit::WeightManagerMat<float> bezier_1{{2}};
    splinekit::WeightManagerMat<float> bezier_2{{3}};
    splinekit::WeightManagerMat<float> bezier_2c{bezier2_createInfo};
    splinekit::WeightManagerMat<float> bezier_4{{5}};
    
    std::cout << "bezier_0" << std::endl;
    bezier_0.printWeightData();
    bezier_0.printMatrix();
    std::cout << "bezier_1" << std::endl;
    bezier_1.printWeightData();
    bezier_1.printMatrix();
    std::cout << "bezier_2" << std::endl;
    bezier_2.printWeightData();
    bezier_2.printMatrix();
    std::cout << "bezier_2c" << std::endl;
    bezier_2c.printWeightData();
    bezier_2c.printMatrix();
    std::cout << "bezier_4" << std::endl;
    bezier_4.printWeightData();
    bezier_4.printMatrix();


    splinekit::WeightManagerMat<float> hermite_1{hermite1_createInfo};
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
    reduced_1.printMatrix();
}