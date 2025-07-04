#include "splinekit/Curve.hpp"


using namespace splinekit;

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const typename WM<T>::CreateInfo &createInfo, uint16_t pointDimensions, uint16_t pointLength):
weightManager{std::make_shared<WM<T>>(createInfo)},
points{pointDimensions, pointLength == 0 ? this->weightManager.get()->getWeightCount() : pointLength}{

}

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const typename WM<T>::CommonCurve &curveType, uint16_t pointDimensions, uint16_t pointSize):
weightManager{std::make_shared<WM<T>>(curveType)},
points{pointDimensions, pointSize == 0 ? this->weightManager.get()->getWeightCount() : pointSize}{

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
void CurveT<T,WM>::calculatePoint(T *res, T t) const {
    const WM<T> *wm = this->weightManager.get();
    const typename WeightManager<T>::WeightDataLayout &weightDataLayout = wm->weightDataLayout;
    
    std::size_t index = t;
    T tDec = t-index;
    
    T weights[weightDataLayout.weightCount];
    
    wm->calculateWeights(tDec, weights);

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

template class CurveT<float,WeightManagerMatT>;
template class CurveT<double,WeightManagerMatT>;