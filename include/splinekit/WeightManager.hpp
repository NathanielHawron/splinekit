#pragma once

#include <cstdint>
#include <iostream>

#include "splinekit/version.hpp"

namespace splinekit{
    template<class T>
    class WeightManager{
    public:
        struct WeightData{
            // Index of parameter in point data
            uint16_t index;
            // Relative index of point to use
            int16_t pointOffset = 0;
            inline void print() const {std::cout << "(" << this->index << ", " << this->pointOffset << ")";}
        };
        struct WeightDataLayout{
            uint16_t weightCount;
            WeightData *weightData;
            inline void print() const {for(uint16_t i=0;i<this->weightCount;++i){this->weightData[i].print(); if(i<this->weightCount-1){std::cout << ", ";}}std::cout << std::endl;}
        };
    public:
        const WeightDataLayout weightDataLayout;
    protected:
        inline WeightManager(WeightDataLayout weightDataLayout):weightDataLayout{weightDataLayout}{
            if(this->weightDataLayout.weightCount <= 1){
                delete[] this->weightDataLayout.weightData;
                throw std::invalid_argument("WeightManager must have more than 1 weight");
            }
        };
        inline virtual ~WeightManager(){
            delete[] this->weightDataLayout.weightData;
        }
    public:
        inline uint16_t getWeightCount() const {return this->weightDataLayout.weightCount;};
        inline uint16_t getParameterCount() const {uint16_t res = 0;for(uint16_t i=0;i<this->weightDataLayout.weightCount;++i){res = std::max(this->weightDataLayout.weightData[i].index, res);}return res+1;};
        virtual uint16_t getOrder() const = 0;
        virtual void calculateWeights(T t, T *res) const = 0;
        virtual void calculateWeights(T t, T *res, int32_t d) const = 0;
    public:
        static inline void printWeightDataDescription() {std::cout << "(index into point, point offset)" << std::endl;};
        inline void printWeightData() const {this->weightDataLayout.print();};
    };
}