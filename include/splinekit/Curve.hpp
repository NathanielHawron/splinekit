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
        // Construct a curve using a WeightManager's CreateInfo. pointLength defaults to WeightManager's highest parameter index.
        CurveT(const typename WM<T>::CreateInfo &createInfo, uint16_t pointDimensions = 3, uint16_t pointLength = 0);
        // Construct a curve using a WeightManager's common curve. pointLength defaults to WeightManager's highest parameter index.
        CurveT(const typename WM<T>::CommonCurve &curveType, uint16_t pointDimensions = 3, uint16_t pointLength = 0);
        // Construct a curve using another curve's WeightManager. pointDimensions defaults to curve's pointDimensions, pointLength defaults to curve's pointLength.
        CurveT(const CurveT &curve, uint16_t pointDimensions = 0, uint16_t pointLength = 0);

        const inline std::shared_ptr<const WM<T>> &getWeightManager() const {return this->weightManager;};

        inline PointsT<T> &getPoints(){return this->points;};
        void addPoint();
        void addPoint(T *point);
        void removePoint(std::size_t index);
        T *getPoint(std::size_t index);

        // Calculates a single point on the curve
        void calculatePoint(T *res, T t, uint16_t order=0) const;
        // Calculates amount points from each segment, evenly spaced in terms of t, and stores the result in points.
        // This method optimizes point calculation using symmetry between segments' t vectors.
        void calculatePointsLinear(PointsT<T> &points, std::size_t amount, uint16_t paramIndex=0, uint16_t order=0) const;
    };

    typedef CurveT<float,WeightManagerMatT> Curve;
    typedef CurveT<double,WeightManagerMatT> Curved;
}