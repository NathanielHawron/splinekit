#include "splinekit/Points.hpp"

#include <stdexcept>
#include <fstream>

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
const T* PointsT<T>::PointConst::operator[](std::size_t index) const {
    return &(this->p[this->parent->dimensions*index]);
}

template <class T>
void PointsT<T>::add(){
    this->p.insert(this->p.end(), this->stride(), T(0));
}
template <class T>
void PointsT<T>::add(std::size_t n){
    this->p.insert(this->p.end(), this->stride()*n, T(0));
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
void PointsT<T>::resize(std::size_t n){
    uint32_t deltaPointCount = n-this->pointCount();
    if(deltaPointCount < 0){
        this->p.erase(this->p.begin() + n*this->stride(), this->p.begin() + this->pointCount()*this->stride());
    }else if(deltaPointCount > 0){
        this->p.insert(this->p.end(), this->stride()*deltaPointCount, T(0));
    }
}
template <class T>
void PointsT<T>::reserve(std::size_t n){
    this->p.reserve(this->stride()*n);
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
void PointsT<T>::clear(){
    this->p.clear();
}
template <class T>
T *PointsT<T>::getRawPoint(std::size_t index){
    return &this->p.at((index*this->stride())%this->p.size());
}
template <class T>
const T *PointsT<T>::getRawPointConst(std::size_t index) const{
    return &this->p.at((index*this->stride())%this->p.size());
}
template <class T>
typename PointsT<T>::Point PointsT<T>::getPoint(std::size_t index){
    return {this->getRawPoint(index),this};
}
template <class T>
const typename PointsT<T>::PointConst PointsT<T>::getPointConst(std::size_t index) const{
    return {this->getRawPointConst(index),this};
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

template <class T>
std::string PointsT<T>::uintToAlpha(uint32_t i){
    if(i == 0){
        return "A";
    }
    std::string res;
    while(i > 0){
        res = std::string{(char)('A' + (i%26))} + res;
        i /= 26;
    }
    return res;
}
template <class T>
uint32_t PointsT<T>::alphaToUint(std::string s){
    uint32_t res=0;
    uint32_t mult=1;
    for(auto i=1;i<=s.length();++i){
        res += (s[s.length()-i]-'A') * mult;
        mult *= 26;
    }
    return res;
}

template <class T>
void PointsT<T>::saveAs(
    std::filesystem::path filePath, FILE_FORMAT format,
    DimensionMap x, DimensionMap y, DimensionMap z,
    DimensionMap r, DimensionMap g, DimensionMap b, DimensionMap a
) const {
}
template <class T>
void PointsT<T>::saveAsCSV(std::filesystem::path filePath, bool header, std::string *headers, uint16_t headerCount) const {
    std::ofstream file(filePath);
    
    const auto count = this->pointCount();
    const auto stride = this->stride();
    
    if(header){
        if(headers == nullptr){
            headerCount = 0;
        }
        uint16_t index;
        for(index = 0;index<headerCount;++index){
            file << headers[index] << (index+1 == headerCount ? "" : ", ");
        }
        for(index;index<stride;++index){
            file << this->uintToAlpha(index/this->dimensions) << (index%this->dimensions) << (index+1 == stride ? "" : ", ");
        }
        file << '\n';
    }
    
    for(std::size_t i=0;i<count;++i){
        PointConst p = (*this)[i];
        for(uint16_t l=0;l<p.getLength();++l){
            const T *prop = p[l];
            for(uint16_t d=0;d<p.getDimensions();++d){
                file << prop[d] << (d+1 == p.getDimensions() ? "" : ", ");
            }
            file << (l+1 == p.getLength() ? "" : ", ");
        }
        file << (i+1 == count ? "\0" : "\n");
    }

    file.close();
}
template <class T>
void PointsT<T>::saveAsTXT(std::filesystem::path filePath, std::string delim, bool header, std::string *headers, uint16_t headerCount) const {
    std::ofstream file(filePath);
    
    const auto count = this->pointCount();
    const auto stride = this->stride();
    
    if(header){
        if(headers == nullptr){
            headerCount = 0;
        }
        uint16_t index;
        for(index = 0;index<headerCount;++index){
            file << headers[index] << (index+1 == headerCount ? "" : ", ");
        }
        for(index;index<stride;++index){
            auto p = index/this->dimensions;
            auto d = index%this->dimensions;
            file << (d==0?"(":"") << this->uintToAlpha(p) << d << (d+1 == this->dimensions ? ")" : "") << (index+1 == stride ? "" : ", ");
        }
        file << '\n';
    }
    
    for(std::size_t i=0;i<count;++i){
        PointConst p = (*this)[i];
        for(uint16_t l=0;l<p.getLength();++l){
            file << "(";
            const T *prop = p[l];
            for(uint16_t d=0;d<p.getDimensions();++d){
                file << prop[d] << (d+1 == p.getDimensions() ? "" : ", ");
            }
            file << ")" << (l+1 == p.getLength() ? "" : ", ");
        }
        file << (i+1 == count ? "\0" : delim);
    }

    file.close();
}
template <class T>
void PointsT<T>::saveAsRAW(std::filesystem::path filePath) const {
    std::ofstream file(filePath);
    const auto pointCount = this->pointCount();
    for(uint16_t p=0;p<pointCount;++p){

    }
    file.close();
}

template class PointsT<float>;
template class PointsT<double>;