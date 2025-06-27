#pragma once

#include <memory>
#include <vector>

#include "splinekit/WeightManager.hpp"
#include "splinekit/WeightManagerMat.hpp"

namespace splinekit{
    constexpr int VERSION_MAJOR = 0;
    constexpr int VERSION_MINOR = 0;
    constexpr int VERSION_PATCH = 0;
    constexpr char VERSION_TAG  = 'a';
    std::string versionString();
    template <class T, template <class> class WM>
    class CurveT{
    protected:
        std::shared_ptr<const WM<T>> weightManager;
    public:
        // Number of dimensions in point
        const uint16_t pointOrder;
        // Number of parameters in a point (including intermediate parameters)
        const uint16_t pointSize;
        std::vector<T> points;
    public:
        // Construct a curve using a WeightManager's CreateInfo. pointSize defaults to WeightManager's weight count.
        CurveT(const typename WM<T>::CreateInfo &createInfo, uint16_t pointOrder = 3, uint16_t pointSize = 0);
        // Construct a curve using a WeightManager's common curve. pointSize defaults to WeightManager's weight count.
        CurveT(const typename WM<T>::CommonCurve &curveType, uint16_t pointOrder = 3, uint16_t pointSize = 0);
        // Construct a curve using another curve's WeightManager. pointOrder defaults to curve's pointOrder, pointSize defaults to curve's pointSize.
        CurveT(const CurveT &curve, uint16_t pointOrder = 0, uint16_t pointSize = 0);

        const inline std::shared_ptr<const WM<T>> &getWeightManager() const {return this->weightManager;};

        void addPoint();
        void addPoint(T *point);
        void removePoint(std::size_t index);
        T *getPoint(std::size_t index);

        void calculatePoint(T *res, T t) const;
    };

    typedef CurveT<float,WeightManagerMatT> Curve;
    typedef CurveT<double,WeightManagerMatT> Curved;
}