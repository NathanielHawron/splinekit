#include "splinekit/Curve.hpp"


using namespace splinekit;

std::string splinekit::versionString(){
    return "v" + std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_PATCH) + "-" + VERSION_TAG;
}

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const typename WM<T>::CreateInfo &createInfo, uint16_t pointOrder, uint16_t pointSize):
weightManager{std::make_shared<WM<T>>(createInfo)},
pointSize{pointSize == 0 ? this->weightManager.get()->getWeightCount() : pointSize},
pointOrder{pointOrder}{

}

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const typename WM<T>::CommonCurve &curveType, uint16_t pointOrder, uint16_t pointSize):
weightManager{std::make_shared<WM<T>>(curveType)},
pointSize{pointSize == 0 ? this->weightManager.get()->getWeightCount() : pointSize},
pointOrder{pointOrder}{

}

template <class T, template <class> class WM>
CurveT<T,WM>::CurveT(const CurveT &curve, uint16_t pointOrder, uint16_t pointSize):
weightManager{curve.getWeightManager()},pointSize{pointSize == 0 ? curve.pointSize : pointSize},
pointOrder{pointOrder == 0 ? curve.pointOrder : pointOrder}{

}


template <class T, template <class> class WM>
void CurveT<T,WM>::addPoint(){
    this->points.insert(this->points.end(), this->pointSize*this->pointOrder, T(0));
}
template <class T, template <class> class WM>
void CurveT<T,WM>::addPoint(T *point){
    this->points.insert(this->points.end(), &point[0], &point[this->pointSize*this->pointOrder]);
}
template <class T, template <class> class WM>
void CurveT<T,WM>::removePoint(std::size_t index){
    this->points.erase(this->points.begin() + index*this->pointSize*this->pointOrder, this->points.begin() + (index+1)*this->pointSize*this->pointOrder);
}
template <class T, template <class> class WM>
T *CurveT<T,WM>::getPoint(std::size_t index){
    return &this->points.at(index*this->pointSize*this->pointOrder);
}

template <class T, template <class> class WM>
void CurveT<T,WM>::calculatePoint(T *res, T t) const {
    const WM<T> *wm = this->weightManager.get();
    const typename WeightManager<T>::WeightDataLayout &weightDataLayout = wm->weightDataLayout;
    
    std::size_t index = t;
    T tDec = t-index;
    
    T weights[weightDataLayout.weightCount];
    
    wm->calculateWeights(tDec, weights);

    for(uint16_t i=0;i<this->pointOrder;++i){
        res[i] = 0;
    }
    for(uint16_t i=0;i<weightDataLayout.weightCount;++i){
        const typename WeightManager<T>::WeightData & weightData = weightDataLayout.weightData[i];
        std::size_t pointIndex = this->pointOrder*(this->pointSize*(index+weightData.pointOffset)+weightData.index);
        for(uint16_t j=0;j<this->pointOrder;++j){
            res[j] += weights[i] * this->points[(pointIndex%this->points.size()) + j];
        }
    }
}

template class CurveT<float,WeightManagerMatT>;
template class CurveT<double,WeightManagerMatT>;