#include "splinekit/Curve.hpp"

#include <cmath>

using namespace splinekit;

template <class T>
T calculateDot(T *a, T *b, std::size_t count){
    T res = 0.0;
    for(std::size_t i=0;i<count;++i){
        res += a[i] * b[i];
    }
    return res;
}

template <class T, template <class> class WM>
T CurveT<T,WM>::calculateLengthDeltaPos(std::size_t segmentIndex, std::size_t samples) const {
    T dt = 1.0/((T)samples);
    T t = (T)segmentIndex;
    T res = 0.0;
    T magSq;
    T *a = new T[this->points.getDimensions()];
    T *b = new T[this->points.getDimensions()];
    this->calculatePoint(a, t, 0, false);
    for(std::size_t i=0;i<samples;++i){
        t += dt;
        T *temp = b;
        b = a;
        a = temp;
        this->calculatePoint(a, t, 0, true);
        magSq = 0.0;
        for(int i=0;i<this->points.getDimensions();++i){
            T d = a[i] - b[i];
            magSq += d*d;
        }
        res += std::sqrt(magSq);
    }
    delete[] a;
    delete[] b;
    return res;
}
template <class T, template <class> class WM>
T CurveT<T,WM>::calculateLengthIntegralVelLinear(std::size_t segmentIndex, std::size_t samples) const {
    T dt = 1.0/((T)samples);
    T t = (T)segmentIndex + 0.5*dt;
    T res = 0.0;
    T *a = new T[this->points.getDimensions()];
    this->calculatePoint(a, (T)segmentIndex, 1, false);
    res += std::sqrt(calculateDot(a, a, this->points.getDimensions())) * 0.5;
    for(std::size_t i=0;i<samples-1;++i){
        this->calculatePoint(a, t, 1, false);
        res += std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
    }
    this->calculatePoint(a, (T)(segmentIndex+1), 1, true);
    res += std::sqrt(calculateDot(a, a, this->points.getDimensions())) * 0.5;

    delete[] a;
    return res * dt;
}
template <class T, template <class> class WM>
T CurveT<T,WM>::calculateLengthIntegralVelQuadratic(std::size_t segmentIndex, std::size_t samples) const {
    samples /= 2;
    T dt = 1.0/((T)samples);
    T t = (T)segmentIndex + 0.5*dt;
    T res = 0.0;
    T vel;
    T acc;
    T *a = new T[this->points.getDimensions()];
    T *b = new T[this->points.getDimensions()];
    this->calculatePoint(a, (T)segmentIndex, 1, false);
    this->calculatePoint(b, (T)segmentIndex, 2, false);
    vel = std::sqrt(calculateDot(a, a, this->points.getDimensions()));
    acc = calculateDot(a, b, this->points.getDimensions()) / vel;
    res += 0.5 * (vel + 0.5 * acc * dt);
    for(std::size_t i=0;i<samples-1;++i){
        this->calculatePoint(a, t, 1, false);
        this->calculatePoint(b, t, 2, false);
        vel = std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        acc = calculateDot(a, b, this->points.getDimensions()) / vel;
        res += vel + 0.5 * acc * dt;
        t += dt;
    }
    this->calculatePoint(a, (T)(segmentIndex+1), 1, true);
    this->calculatePoint(b, (T)(segmentIndex+1), 2, true);
    vel = std::sqrt(calculateDot(a, a, this->points.getDimensions()));
    acc = calculateDot(a, b, this->points.getDimensions()) / vel;
    res += 0.5 * (vel + 0.5 * acc * dt);
    delete[] a;
    delete[] b;
    return res * dt;
}
template <class T, template <class> class WM>
T CurveT<T,WM>::calculateLengthSimpsons13(std::size_t segmentIndex, std::size_t samples) const {
    samples /= 2;
    T dt = 0.5/((T)samples);
    T t = (T)segmentIndex;
    T res = 0.0;
    T *a = new T[this->points.getDimensions()];

    this->calculatePoint(a, (T)segmentIndex, 1, false);
    res -= std::sqrt(calculateDot(a, a, this->points.getDimensions()));
    for(std::size_t i=0;i<samples;++i){
        this->calculatePoint(a, t, 1, false);
        res += 2*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
        this->calculatePoint(a, t, 1, false);
        res += 4*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
    }
    this->calculatePoint(a, (T)(segmentIndex+1), 1, true);
    res += std::sqrt(calculateDot(a, a, this->points.getDimensions()));

    delete[] a;
    return res * dt / 3.0;
}
template <class T, template <class> class WM>
T CurveT<T,WM>::calculateLengthSimpsons38(std::size_t segmentIndex, std::size_t samples) const {
    samples /= 3;
    T dt = 1.0/(3.0*(T)samples);
    T t = (T)segmentIndex;
    T res = 0.0;
    T *a = new T[this->points.getDimensions()];

    this->calculatePoint(a, (T)segmentIndex, 1, false);
    res -= std::sqrt(calculateDot(a, a, this->points.getDimensions()));
    for(std::size_t i=0;i<samples;++i){
        this->calculatePoint(a, t, 1, false);
        res += 2*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
        this->calculatePoint(a, t, 1, false);
        res += 3*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
        this->calculatePoint(a, t, 1, false);
        res += 3*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
    }
    this->calculatePoint(a, (T)(segmentIndex+1), 1, true);
    res += std::sqrt(calculateDot(a, a, this->points.getDimensions()));

    delete[] a;
    return res * dt * 3.0 / 8.0;
}
template <class T, template <class> class WM>
T CurveT<T,WM>::calculateLengthBoole(std::size_t segmentIndex, std::size_t samples) const {
    samples /= 4;
    T dt = 0.25/(T)samples;
    T t = (T)segmentIndex;
    T res = 0.0;
    T *a = new T[this->points.getDimensions()];

    this->calculatePoint(a, (T)segmentIndex, 1, false);
    res -= 7*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
    for(std::size_t i=0;i<samples;++i){
        this->calculatePoint(a, t, 1, false);
        res += 14*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
        this->calculatePoint(a, t, 1, false);
        res += 32*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
        this->calculatePoint(a, t, 1, false);
        res += 12*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
        this->calculatePoint(a, t, 1, false);
        res += 32*std::sqrt(calculateDot(a, a, this->points.getDimensions()));
        t += dt;
    }
    this->calculatePoint(a, (T)(segmentIndex+1), 1, true);
    res += 7*std::sqrt(calculateDot(a, a, this->points.getDimensions()));

    delete[] a;
    return res * dt * 2.0 / 45.0;
}
template <class T, template <class> class WM>
T CurveT<T,WM>::calculateTotalLength(std::size_t samples) const {
    T res = 0.0;
    for(std::size_t i=0;i<this->points.pointCount();++i){
        res += this->calculateLengthDeltaPos(i,samples);
    }
    return res;
}

template class CurveT<float,WeightManagerMatT>;
template class CurveT<double,WeightManagerMatT>;