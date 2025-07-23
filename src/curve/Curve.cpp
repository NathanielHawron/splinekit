#include "splinekit/Curve.hpp"

#include <cstring>
#include <cmath>

using namespace splinekit;

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const typename WM<T>::CreateInfo &createInfo, uint16_t pointDimensions, uint16_t pointLength):
weightManager{std::make_shared<WM<T>>(createInfo)},
points{pointDimensions, pointLength == 0 ? this->weightManager.get()->getParameterCount() : pointLength}{

}

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const typename WM<T>::CommonCurve &curveType, uint16_t pointDimensions, uint16_t pointSize):
weightManager{std::make_shared<WM<T>>(curveType)},
points{pointDimensions, pointSize == 0 ? this->weightManager.get()->getParameterCount() : pointSize}{

}

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const CurveT &curve, uint16_t pointDimensions, uint16_t pointLength):
weightManager{curve.getWeightManager()},
points{pointDimensions == 0 ? curve.points.dimensions : pointDimensions, pointLength == 0 ? curve.points.length : pointLength}{

}


template <class T, template <class> class WM>
void CurveT<T,WM>::addPoint(){
    this->points.add();
}
template <class T, template <class> class WM>
void CurveT<T,WM>::addPoint(T *point){
    this->points.add(point);
}
template <class T, template <class> class WM>
void CurveT<T,WM>::removePoint(std::size_t index){
    this->points.remove(index);
}
template <class T, template <class> class WM>
T* CurveT<T,WM>::getPoint(std::size_t index){
    return this->points.getRawPoint(index);
}

template <class T, template <class> class WM>
void CurveT<T,WM>::calculatePoint(T *res, T t, uint16_t derivative, bool endDerivative) const {
    const WM<T> *wm = this->weightManager.get();
    const typename WeightManager<T>::WeightDataLayout &weightDataLayout = wm->weightDataLayout;
    
    std::size_t index = t;
    T tDec = t-index;
    if(endDerivative){
        if(tDec == (T)(0.0)){
            --index;
            tDec = (T)1.0;
        }
    }
    
    T weights[weightDataLayout.weightCount];
    
    wm->calculateWeights(tDec, weights, derivative);

    for(uint16_t i=0;i<this->points.dimensions;++i){
        res[i] = 0;
    }
    for(uint16_t i=0;i<weightDataLayout.weightCount;++i){
        const typename WeightManager<T>::WeightData & weightData = weightDataLayout.weightData[i];
        const T *param = this->points(index + weightData.pointOffset,weightData.index);
        for(uint16_t j=0;j<this->points.dimensions;++j){
            res[j] += weights[i] * param[j];
        }
    }
}
template <class T, template <class> class WM>
void CurveT<T,WM>::calculatePointsLinear(PointsT<T> &points, std::size_t amount, uint16_t paramIndex, uint16_t derivative) const {
    if(paramIndex >= points.length){
        throw std::invalid_argument("[CurveT]: Param index ("+std::to_string(paramIndex)+") must be lower than point's length ("+std::to_string(points.length)+")");
    }
    points.resize(amount*this->points.pointCount());

    const WM<T> *wm = this->weightManager.get();
    const typename WeightManager<T>::WeightDataLayout &weightDataLayout = wm->weightDataLayout;
        
    T weights[amount][weightDataLayout.weightCount];
    
    T step = 1.0/(T)amount;
    for(std::size_t i=0;i<amount;++i){
        wm->calculateWeights(step*(T)i, weights[i], derivative);
    }

    for(std::size_t p=0;p<this->points.pointCount();++p){
        for(std::size_t a=0;a<amount;++a){
            typename PointsT<T>::Point point = points.getPoint(p*amount + a);
            
            for(uint16_t i=0;i<weightDataLayout.weightCount;++i){
                const typename WeightManager<T>::WeightData & weightData = weightDataLayout.weightData[i];
                const T *param = this->points(p + weightData.pointOffset,weightData.index);
                for(uint16_t j=0;j<this->points.dimensions;++j){
                    point[paramIndex][j] += weights[a][i] * param[j];
                }
            }
        }
    }
}

template class CurveT<float,WeightManagerMatT>;
template class CurveT<double,WeightManagerMatT>;