#include "splinekit/Points.hpp"

#include <stdexcept>

using namespace splinekit;

// Throws an error if the Point p cannot be inserted into Points ps. Returns true if ps == p.parent.
template <class T>
inline bool checkPointValidity(const PointsT<T> *ps, const typename PointsT<T>::Point &p){
    if(p.parent == ps){
        return true;
    }else if(p.parent->dimensions != ps->dimensions || p.parent->length != ps->length){
        throw std::invalid_argument("[Points] Failed to add Point to Points: dimensions and/or length don't match");
    }else{
        return false;
    }
}

template <class T>
T* PointsT<T>::Point::operator[](std::size_t index){
    return &(this->p[this->parent->dimensions*index]);
}


template <class T>
void PointsT<T>::add(){
    this->p.insert(this->p.end(), this->stride(), T(0));
}
template <class T>
void PointsT<T>::add(const T *p){
    this->p.insert(this->p.end(), &p[0], &p[this->stride()]);
}
template <class T>
void PointsT<T>::add(const Point p){
    checkPointValidity(this, p);
    this->p.insert(this->p.end(), p.begin(), p.end());
}

template <class T>
void PointsT<T>::insert(const T *p, std::size_t index){
    this->p.insert(this->p.begin() + index*this->stride(), &p[0], &p[this->stride()]);
}
template <class T>
void PointsT<T>::insertSelf(const T *p, std::size_t index){
    T temp[this->stride()];
    memcpy(temp, p, this->stride() * sizeof(T));
    this->p.insert(this->p.begin() + index*this->stride(), &temp[0], &temp[this->stride()]);
}
template <class T>
void PointsT<T>::insert(const Point p, std::size_t index){
    if(checkPointValidity(this, p)){
        T temp[this->stride()];
        memcpy(temp, p.begin(), this->stride() * sizeof(T));
        this->p.insert(this->p.begin() + index*this->stride(), &temp[0], &temp[this->stride()]);
    }else{
        this->p.insert(this->p.begin() + index*this->stride(), p.begin(), p.end());
    }
}

template <class T>
void PointsT<T>::remove(std::size_t index){
    this->p.erase(this->p.begin() + index*this->stride(), this->p.begin() + (index+1)*this->stride());
}
template <class T>
T *PointsT<T>::getRawPoint(std::size_t index){
    return &this->p.at(index*this->stride());
}
template <class T>
typename PointsT<T>::Point PointsT<T>::getPoint(std::size_t index){
    return {this->getRawPoint(index),this};
}

template <class T>
T *PointsT<T>::operator()(std::size_t pointIndex, std::size_t parameterIndex){
    std::size_t pIndex = (pointIndex * this->stride()) % this->size();
    std::size_t pOff = parameterIndex * this->dimensions;
    return &this->p.at(pIndex + pOff);}
template <class T>
const T *PointsT<T>::operator()(std::size_t pointIndex, std::size_t parameterIndex) const {
    std::size_t pIndex = (pointIndex * this->stride()) % this->size();
    std::size_t pOff = parameterIndex * this->dimensions;
    return &this->p.at(pIndex + pOff);
}

template <class T>
T &PointsT<T>::operator()(std::size_t pointIndex, std::size_t parameterIndex, std::size_t dimensionIndex){
    std::size_t pIndex = (pointIndex * this->stride()) % this->size();
    std::size_t pOff = parameterIndex * this->dimensions;
    return this->p.at(pIndex + pOff + dimensionIndex);}
template <class T>
const T &PointsT<T>::operator()(std::size_t pointIndex, std::size_t parameterIndex, std::size_t dimensionIndex) const {
    std::size_t pIndex = (pointIndex * this->stride()) % this->size();
    std::size_t pOff = parameterIndex * this->dimensions;
    return this->p.at(pIndex + pOff + dimensionIndex);
}

template class PointsT<float>;
template class PointsT<double>;