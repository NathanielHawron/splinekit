#pragma once

#include "splinekit/WeightManager.hpp"

namespace splinekit{
    template <class T>
    class WeightManagerMatT : public WeightManager<T>{
    public:
        struct DerivativeReplacement{
            uint16_t parameterIndex;
            T t;
            int32_t derivativeCount;
        };
        struct Replacement{
            uint16_t parameterIndex;
            T t;
            T *parameterWeights;
        };
        struct RowReplacement{
            uint16_t parameterIndex0;
            uint16_t parameterIndex1;
        };
        struct Remap{
            uint16_t parameterIndex;
            typename WeightManager<T>::WeightData remap;
        };
        struct CreateInfo{
            uint16_t order;
            uint16_t derivativeReplacement_c = 0;
            DerivativeReplacement *derivativeReplacement_v = nullptr;
            uint16_t replacement_c = 0;
            Replacement *replacement_v = nullptr;
            uint16_t rowReplacement_c = 0;
            RowReplacement *rowReplacement_v = nullptr;
            uint16_t remap_c = 0;
            Remap *remap_v = nullptr;
        };
        enum class CommonCurve : uint8_t{
            BEZIER_O1,
            BEZIER_O2,
            BEZIER_O3,
            BEZIER_O4,
            BEZIER_O5,
            HERMITE_D1,
            HERMITE_D2,
            CATMULL_ROM
        };
    private:
        static typename WeightManager<T>::WeightDataLayout calculateWeightDataLayout(const CreateInfo &createInfo);
        static CreateInfo generateCommonCreateInfo(CommonCurve curveType);
    protected:
        void replacePoint(Replacement replacement);
        T *matrix;
    public:
        // Create a WeightManager that generates weights for a polynomial basis matrix based spline
        WeightManagerMatT(const CreateInfo &createInfo);
        // Create a WeightManager that generates weights for a common polynomial basis matrix based spline
        WeightManagerMatT(CommonCurve curveType);
        ~WeightManagerMatT();
        uint16_t getOrder() const;
        void calculateWeights(T t, T *res) const;
        void calculateWeights(T t, T *res, int32_t d) const;
        void printMatrix() const;
    };

    typedef WeightManagerMatT<float> WeightManagerMat;
}