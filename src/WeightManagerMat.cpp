#include "splinekit/WeightManagerMat.hpp"

#include <iostream>
#include <cstring>
#include <limits>

using namespace splinekit;


template <class T>
typename WeightManager<T>::WeightDataLayout WeightManagerMatT<T>::calculateWeightDataLayout(const CreateInfo &createInfo){
    if(createInfo.order < 1){
        throw std::invalid_argument("[WeightManagerMat] Must have order > 0");
    }
    typename WeightManager<T>::WeightDataLayout res = {
        (uint16_t)(createInfo.order + 1),
        new typename WeightManager<T>::WeightData[createInfo.order + 1]
    };

    enum class WEIGHT_TYPE{
        START,
        INTERMEDIATE,
        END
    };

    WEIGHT_TYPE weightTypes[createInfo.order+1];
    weightTypes[0] = WEIGHT_TYPE::START;
    for(uint16_t i=1;i<createInfo.order;++i){
        weightTypes[i] = WEIGHT_TYPE::INTERMEDIATE;
    }
    weightTypes[createInfo.order] = WEIGHT_TYPE::END;

    for(uint16_t i=0;i<createInfo.derivativeReplacement_c;++i){
        auto &replacement = createInfo.derivativeReplacement_v[i];
        if(replacement.parameterIndex > createInfo.order){
            std::string msg = "[WeightManagerMat] Derivative replacement index out of bounds (";
            msg += std::to_string(i) + "): " + std::to_string(replacement.parameterIndex) + " / " + std::to_string(createInfo.order);
            throw std::invalid_argument(msg);
        }
        if(replacement.t == 0){
            weightTypes[replacement.parameterIndex] = WEIGHT_TYPE::START;
        }else if(replacement.t == 1){
            weightTypes[replacement.parameterIndex] = WEIGHT_TYPE::END;
        }else{
            weightTypes[replacement.parameterIndex] = WEIGHT_TYPE::INTERMEDIATE;
        }
    }
    /*std::cout << "D Replacement: " << std::endl;
    for(uint16_t i=0;i<res.weightCount;++i){
        std::cout << (int)weightTypes[i] << ", ";
    }
    std::cout << std::endl;*/

    for(uint16_t i=0;i<createInfo.replacement_c;++i){
        auto &replacement = createInfo.replacement_v[i];
        if(replacement.parameterIndex > createInfo.order){
            std::string msg = "[WeightManagerMat] Replacement index out of bounds (";
            msg += std::to_string(i) + "): " + std::to_string(replacement.parameterIndex) + " / " + std::to_string(createInfo.order);
            throw std::invalid_argument(msg);
        }
        if(replacement.t == 0){
            weightTypes[replacement.parameterIndex] = WEIGHT_TYPE::START;
        }else if(replacement.t == 1){
            weightTypes[replacement.parameterIndex] = WEIGHT_TYPE::END;
        }else{
            weightTypes[replacement.parameterIndex] = WEIGHT_TYPE::INTERMEDIATE;
        }
    }
    /*if(createInfo.replacement_c > 0){
        std::cout << "A Replacement: " << std::endl;
        for(uint16_t i=0;i<res.weightCount;++i){
            std::cout << (int)weightTypes[i] << ", ";
        }
        std::cout << std::endl;
    }*/

    for(uint16_t i=0;i<createInfo.rowReplacement_c;++i){
        auto &replacement = createInfo.rowReplacement_v[i];
        if(replacement.parameterIndex0 > createInfo.order || replacement.parameterIndex1 > createInfo.order){
            std::string msg = "[WeightManagerMat] Row replacement index out of bounds (";
            msg += std::to_string(i) + "): " + std::to_string(replacement.parameterIndex0) + " / " + std::to_string(createInfo.order);
            msg += " or " + std::to_string(replacement.parameterIndex1) + " / " + std::to_string(createInfo.order);
            throw std::invalid_argument(msg);
        }
        auto temp = weightTypes[replacement.parameterIndex0];
        weightTypes[replacement.parameterIndex0] = weightTypes[replacement.parameterIndex1];
        weightTypes[replacement.parameterIndex1] = temp;
    }
    /*std::cout << "R replacement: " << std::endl;
    for(uint16_t i=0;i<res.weightCount;++i){
        std::cout << (int)weightTypes[i] << ", ";
    }
    std::cout << std::endl;*/
    
    uint16_t startCount = 0, startIndex = 0;
    uint16_t intermediateCount = 0, intermediateIndex = 0;
    uint16_t endCount = 0, endIndex = 0;

    for(uint16_t i=0;i<res.weightCount;++i){
        switch(weightTypes[i]){
            case WEIGHT_TYPE::START:{
                ++startCount;
            }break;
            case WEIGHT_TYPE::INTERMEDIATE:{
                ++intermediateCount;
            }break;
            case WEIGHT_TYPE::END:{
                ++endCount;
            }break;
        }
    }
    for(uint16_t i=0;i<res.weightCount;++i){
        switch(weightTypes[i]){
            case WEIGHT_TYPE::START:{
                res.weightData[i] = {startIndex, 0};
                ++startIndex;
            }break;
            case WEIGHT_TYPE::INTERMEDIATE:{
                res.weightData[i] = {(uint16_t)(startCount + intermediateIndex), 0};
                ++intermediateIndex;
            }break;
            case WEIGHT_TYPE::END:{
                res.weightData[i] = {endIndex, 1};
                ++endIndex;
            }break;
        }
    }
    for(uint16_t i=0;i<createInfo.remap_c;++i){
        Remap &data = createInfo.remap_v[i];
        if(data.parameterIndex > createInfo.order){
            std::string msg = "[WeightManagerMat] Remap index out of bounds (";
            msg += std::to_string(i) + "): " + std::to_string(data.parameterIndex) + " / " + std::to_string(createInfo.order);
            throw std::invalid_argument(msg);
        }
        res.weightData[data.parameterIndex] = data.remap;
    }
    return res;
}

// Hermite parameters
template <class T>
typename splinekit::WeightManagerMatT<T>::DerivativeReplacement hermite1_derivative[2] = {
    {1, 0.0f, 1},
    {2, 1.0f, 1}
};
template <class T>
typename splinekit::WeightManagerMatT<T>::RowReplacement hermite1_row[1] = {
    {2,3}
};
template <class T>
typename splinekit::WeightManagerMatT<T>::DerivativeReplacement hermite2_derivative[4] = {
    {1, 0.0f, 1},
    {2, 0.0f, 2},
    {3, 1.0f, 2},
    {4, 1.0f, 1}
};
template <class T>
typename splinekit::WeightManagerMatT<T>::RowReplacement hermite2_row[1] = {
    {3,5}
};

template <class T>
typename splinekit::WeightManagerMatT<T>::CreateInfo hermite1_createInfo = {
    3,
    2, hermite1_derivative<T>,
    0, nullptr,
    1, hermite1_row<T>,
    0, nullptr
};
template <class T>
typename splinekit::WeightManagerMatT<T>::CreateInfo hermite2_createInfo = {
    5,
    4, hermite2_derivative<T>,
    0, nullptr,
    1, hermite2_row<T>,
    0, nullptr
};

// Catmull-rom parameters
template <class T>
T catmull1_a_weights[2][4] = {
    {0,-0.5,0,1},
    {1,0,0.5,0}
};
template <class T>
typename splinekit::WeightManagerMatT<T>::Replacement catmull1_a[2] = {
    {1, -1.0f, catmull1_a_weights<T>[0]},
    {2, 2.f, catmull1_a_weights<T>[1]}
};
template <class T>
typename splinekit::WeightManagerMatT<T>::RowReplacement catmull1_row[2] = {
    {0,1},
    {2,3}
};
template <class T>
typename splinekit::WeightManagerMatT<T>::Remap catmull1_remap[4] = {
    {0,{0,-1}},
    {1,{0,0}},
    {2,{0,1}},
    {3,{0,2}}
};
template <class T>
typename WeightManagerMatT<T>::CreateInfo WeightManagerMatT<T>::generateCommonCreateInfo(CommonCurve curveType){
    switch(curveType){
        case CommonCurve::BEZIER_O1:
            return {{1}};
        case CommonCurve::BEZIER_O2:
            return {{2}};
        case CommonCurve::BEZIER_O3:
            return {{3}};
        case CommonCurve::BEZIER_O4:
            return {{4}};
        case CommonCurve::BEZIER_O5:
            return {{5}};
        case CommonCurve::HERMITE_D1:
            return CreateInfo{
                3,
                2, hermite1_derivative<T>,
                0, nullptr,
                1, hermite1_row<T>,
                0, nullptr
            };
        case CommonCurve::HERMITE_D2:
            return CreateInfo{
                5,
                4, hermite2_derivative<T>,
                0, nullptr,
                1, hermite2_row<T>,
                0, nullptr
            };
        case CommonCurve::CATMULL_ROM:
            return CreateInfo{
                3,
                2, hermite1_derivative<T>,
                2, catmull1_a<T>,
                2, catmull1_row<T>,
                4, catmull1_remap<T>
            };
        default:
            throw std::invalid_argument("[WeightManagerMat] Common curve not recognized");
    }
}


template <class T>
void WeightManagerMatT<T>::replacePoint(Replacement replacement){
    uint16_t wc = this->getWeightCount();
    T parameterWeight = replacement.parameterWeights[replacement.parameterIndex];
    uint16_t ro = replacement.parameterIndex * wc;
    for(uint16_t i=0;i<wc;++i){
        if(i != replacement.parameterIndex){
            T m = replacement.parameterWeights[i] * parameterWeight;
            uint16_t o = i * wc;
            for(uint16_t j=0;j<wc;++j){
                this->matrix[o + j] -= this->matrix[ro + j] * m;
            }
        }
    }
    for(uint16_t i=0;i<wc;++i){
        this->matrix[ro + i] *= parameterWeight;
    }
}


template <class T>
WeightManagerMatT<T>::WeightManagerMatT(const CreateInfo &createInfo):WeightManager<T>{WeightManagerMatT::calculateWeightDataLayout(createInfo)},matrix{new T[this->getWeightCount()*this->getWeightCount()]}{
    uint16_t weightCount = this->getWeightCount();
    uint16_t order = (uint16_t)(weightCount - 1);
    // Initialize to 0, and first row to 1
    memset(this->matrix,0,sizeof(T)*weightCount*weightCount);
    for(uint_fast16_t i=0;i<weightCount;++i){
        this->matrix[i] = 1;
    }
    // Generate pascal's triangle (where tip is at index 0, and one edge is the first row)
    for(uint_fast16_t j=1;j<weightCount;++j){
        uint_fast16_t o = j * weightCount;
        for(uint_fast16_t i=1;i<weightCount;++i){
            this->matrix[o+i] = this->matrix[o+i-1] + this->matrix[o+i-1-weightCount];
        }
    }
    // Multiply each column by corresponding element in last column, and flip signs in checkerboard pattern
    for(uint_fast16_t i=1;i<order;++i){
        T m = this->matrix[weightCount * (i+1) - 1];
        for(uint_fast16_t j=0;j<=i;++j){
            this->matrix[j*weightCount + i] *= m * (((i&1) == (j&1)) ? 1 : -1);
        }
    }
    // Flip signs on last column
    for(uint_fast16_t i=1+(weightCount&1);i<weightCount;i+=2){
        this->matrix[(weightCount * i) - 1] *= -1;
    }

    // Derivative replacements
    for(uint16_t i=0;i<createInfo.derivativeReplacement_c;++i){
        auto &derivativeReplacement = createInfo.derivativeReplacement_v[i];
        T weights[weightCount];
        this->calculateWeights(derivativeReplacement.t, weights, derivativeReplacement.derivativeCount);
        weights[derivativeReplacement.parameterIndex] = T(1.0) / weights[derivativeReplacement.parameterIndex];
        this->replacePoint({derivativeReplacement.parameterIndex,derivativeReplacement.t,weights});
    }
    // Replacements
    for(uint16_t i=0;i<createInfo.replacement_c;++i){
        auto &replacement = createInfo.replacement_v[i];
        this->replacePoint(replacement);
    }
    // Row replacements
    for(uint16_t i=0l;i<createInfo.rowReplacement_c;++i){
        auto &rowReplacement = createInfo.rowReplacement_v[i];
        uint16_t o0 = rowReplacement.parameterIndex0 * weightCount;
        uint16_t o1 = rowReplacement.parameterIndex1 * weightCount;
        T temp;
        for(uint16_t i=0;i<weightCount;++i){
            temp = this->matrix[o0 + i];
            this->matrix[o0 + i] = this->matrix[o1 + i];
            this->matrix[o1 + i] = temp;
        }
    }
}

template <class T>
WeightManagerMatT<T>::WeightManagerMatT(CommonCurve curveType):WeightManagerMatT<T>(this->generateCommonCreateInfo(curveType)){

}

template <class T>
WeightManagerMatT<T>::~WeightManagerMatT(){
    delete[] this->matrix;
}

template <class T>
void WeightManagerMatT<T>::calculateWeights(T t, T *res) const {
    uint16_t weightCount = this->getWeightCount();
    memset(res, 0, sizeof(T) * weightCount);
    T tPowers[weightCount];
    tPowers[0] = 1;
    for(uint16_t i=1;i<weightCount;++i){
        tPowers[i] = tPowers[i-1] * t;
    }
    for(uint16_t i=0;i<weightCount;++i){
        uint16_t o = i * weightCount;
        for(uint16_t j=0;j<weightCount;++j){
            res[i] += this->matrix[o + j] * tPowers[j];
        }
    }
}

template <class T>
void WeightManagerMatT<T>::calculateWeights(T t, T *res, int32_t d) const {
    uint16_t weightCount = this->getWeightCount();
    memset(res, 0, sizeof(T) * weightCount);
    if(d >= weightCount){
        return;
    }
    T tPowers[weightCount];
    tPowers[d] = 1;
    for(uint16_t i=d+1;i<weightCount;++i){
        tPowers[i] = tPowers[i-1] * t;
    }
    for(uint16_t i=d;i<weightCount;++i){
        for(uint16_t j=0;j<d;++j){
            tPowers[i] *= i-j;
        }
    }
    for(uint16_t i=0;i<weightCount;++i){
        uint16_t o = i * weightCount;
        for(uint16_t j=d;j<weightCount;++j){
            res[i] += this->matrix[o + j] * tPowers[j];
        }
    }
}

template <class T>
void WeightManagerMatT<T>::printMatrix() const {
    for(uint_fast16_t j=0;j<this->getWeightCount();++j){
        uint_fast16_t o = j*(this->getWeightCount());
        std::cout << '[';
        for(uint_fast16_t i=0;i<this->getWeightCount();++i){
            std::cout << this->matrix[o+i] << (i == this->getWeightCount()-1 ? "]" : ", ");
        }
        std::cout << '\n';
    }
}


template class WeightManagerMatT<float>;
template class WeightManagerMatT<double>;