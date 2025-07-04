#pragma once

#include <memory>
#include <vector>

#include "splinekit/WeightManager.hpp"
#include "splinekit/WeightManagerMat.hpp"
#include "splinekit/Points.hpp"

namespace splinekit{
    template <class T, template <class> class WM>
    class CurveT{
    protected:
        std::shared_ptr<const WM<T>> weightManager;
    public:
        PointsT<T> points;
    public:
        // Construct a curve using a WeightManager's CreateInfo. pointLength defaults to WeightManager's weight count.
        CurveT(const typename WM<T>::CreateInfo &createInfo, uint16_t pointDimensions = 3, uint16_t pointLength = 0);
        // Construct a curve using a WeightManager's common curve. pointLength defaults to WeightManager's weight count.
        CurveT(const typename WM<T>::CommonCurve &curveType, uint16_t pointDimensions = 3, uint16_t pointLength = 0);
        // Construct a curve using another curve's WeightManager. pointDimensions defaults to curve's pointDimensions, pointLength defaults to curve's pointLength.
        CurveT(const CurveT &curve, uint16_t pointDimensions = 0, uint16_t pointLength = 0);

        const inline std::shared_ptr<const WM<T>> &getWeightManager() const {return this->weightManager;};

        inline PointsT<T> &getPoints(){return this->points;};
        void addPoint();
        void addPoint(T *point);
        void removePoint(std::size_t index);
        T *getPoint(std::size_t index);

        void calculatePoint(T *res, T t) const;
    };

    typedef CurveT<float,WeightManagerMatT> Curve;
    typedef CurveT<double,WeightManagerMatT> Curved;
}