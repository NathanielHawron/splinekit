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
        void calculatePoint(T *res, T t, uint16_t derivative=0, bool endDerivative=false) const;
        // Calculates amount points from each segment, evenly spaced in terms of t, and stores the result in points.
        // This method optimizes point calculation using symmetry between segments' t vectors.
        void calculatePointsLinear(PointsT<T> &points, std::size_t amount, uint16_t paramIndex=0, uint16_t derivative=0) const;
        // Calculate length by summing distances between evenly placed sample points. Always <= actual length, but will always converge.
        T calculateLengthDeltaPos(std::size_t segmentIndex, std::size_t samples=1000) const;
        // Calculate length by summing velocity at evenly placed sample points.
        T calculateLengthIntegralVelLinear(std::size_t segmentIndex, std::size_t samples=1000) const;
        // Calculate length by summing velocity at evenly placed sample points, and including the acceleration projected onto the velocity. samples must be even (odd numbers get truncated).
        T calculateLengthIntegralVelQuadratic(std::size_t segmentIndex, std::size_t samples=1000) const;
        // Calculate length using Simpson's 1/3 method, which fits a parabola between evenly spaced points. samples must be even (odd numbers get truncated).
        T calculateLengthSimpsons13(std::size_t segmentIndex, std::size_t samples=1000) const;
        // Calculate length using Simpson's 3/8 method, which fits a cubic function between evenly spaced points. samples must be a multiple of 3 (non-multiples of three get truncated).
        T calculateLengthSimpsons38(std::size_t segmentIndex, std::size_t samples=1000) const;
        // Caclulate length using Boole's method, which fits a quartic function between evenly spaces points. samples must be a multiple of 4 (non-multiples of 4 get truncated).
        T calculateLengthBoole(std::size_t segmentIndex, std::size_t samples=1000) const;
        // Uses calculatePointsLinear to calculate combined length of all spline segments. samples applies to each segment.
        T calculateTotalLength(std::size_t samples=1000) const;
    public:
        struct Extrema{
            T t; std::unique_ptr<T[]> vec;
        };
        // Calculate (local) minimum and maximum magnitudes for a derivative of a curve segment. Use calculateAllExtrema to loop through all segments.
        // This function uses numerical methods.
        // maxError is the maximum absolute value of the derivative dotted with the next derivative (Velocity multiplied by acceleration in the same direction).
        // maxIterations is the maximum number of times a loop will be run before giving up. This prevents it from holding up the program, at the expense of accuracy. Note that this includes all loops, but that it will find approximately where all the extrema are before reducing the error. Set to 0 for practically infinite.
        // lengthIterations is the number of iterations used to calculate the length of the spline (this is to make the velocity uniform). A smaller value should generally be sufficient here.
        // alpha is the factor used for gradient descent. The smaller the number, the more stability there will be when finding extrema, but the more iterations it may require. In general, only decrease if you have very large or very sharp curves.
        void calculateExtrema(std::vector<Extrema> &res, std::size_t segmentIndex, uint16_t derivative=1, bool ignoreEnd=true, bool max=true, bool min=true, T maxError=0.0001, std::size_t maxIterations=10000, std::size_t lengthIterations=100, T alpha=0.0001, T minStep=0.0005, T maxStep=0.01) const;
        // Calculate all (local) minimum and maximum magnitudes for a derivative of every curve segment.
        // Set ignoreEnd to false for discontinuous splines, or else you will get duplicates
        void calculateAllExtrema(std::vector<Extrema> &res, uint16_t derivative=1, bool ignoreEnd=true, bool max=true, bool min=true, T maxError=0.0001, std::size_t maxIterations=10000, std::size_t lengthIterations=100, T alpha=0.0001, T minStep=0.0005, T maxStep=0.01) const;
    };

    typedef CurveT<float,WeightManagerMatT> Curve;
    typedef CurveT<double,WeightManagerMatT> Curved;
}