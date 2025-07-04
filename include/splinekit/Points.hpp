#pragma once

#include <vector>
#include <cstdint>
#include <iostream>

#include "splinekit/version.hpp"

namespace splinekit{
    template <class T>
    class PointsT{
    public:
        // Note: Point objects contain a pointer to a point in a Points array by index. A Point may point to a different or invalid Point when Points::insert or Points::remove are used.
        class Point{
        public:
            T *p;
            PointsT *parent;
            // Access a property from the point (can be multidimensional)
            T *operator[](std::size_t index);

            // 
            inline const uint16_t getDimensions(){return this->parent->dimensions;};
            inline const uint16_t getLength(){return this->parent->length;};

            // Returns pointer to the first float in this Point. This function is meant for adding and inserting into a flat array.
            inline const T* begin() const {return &p[0];};
            // Returns pointer to the last float in this Point. This function is meant for adding and inserting into a flat array.
            inline const T* end() const {return &p[this->parent->stride()];};

            /*void operator+=(std::size_t d);
            void operator-=(std::size_t d);
            Point<T> &operator++();
            Point<T> &operator--();
            Point<T> operator++(int);
            Point<T> operator--(int);*/
        };
    public:
        // Number of dimensions in each point's data
        const uint16_t dimensions;
        // Number of types of data in each point's data
        const uint16_t length;
    protected:
        // Flattened array of points
        std::vector<T> p;
        // Index to use for >> operator
        std::size_t ptr;
        bool ptrReset;
    public:
        // PointsT is used to make it easier to manage a flat vector of points as an array of 2 dimensional data
        inline PointsT(uint16_t dimensions, uint16_t length):dimensions{dimensions},length{length},ptr{0},ptrReset{false}{};
        // Number of T required in each point (all pointer operations assume this many T are available)
        inline uint32_t stride() const {return this->dimensions * this->length;};

        // Adds a zero point to the end of the points array
        void add();
        // Adds p to the end of the points array
        void add(const T *p);
        // Adds p to the end of the points array
        void add(const Point p);
        // Adds point p to the points array at index. Note that if p points to an element of this Points, this method may not work correctly, and insertSelf should be used instead.
        void insert(const T *p, std::size_t index);
        // Adds point p to the points array at index. This method uses a temporay buffer, so a Point may be inserted before itself in the same Points object.
        void insertSelf(const T *p, std::size_t index);
        // Adds point p to the points array at index. This method checks if the Point belongs to this Points object to ensure it inserts correctly.
        void insert(const Point p, std::size_t index);
        // Removes a point from the points array
        void remove(std::size_t index);
        // Retreives a pointer to a point
        T *getRawPoint(std::size_t index);
        // Retreives a Point from the points array
        Point getPoint(std::size_t index);
        // Wrapper for >> operator to get point with counter
        Point getPoint(){Point res;*this >> res;return res;};
        // Resets pointer for >> operator to 0
        inline void resetPtr(){this->ptr=0;this->ptrReset=false;};
        // Sets pointer for >> operator
        inline void setPtr(std::size_t ptr, bool ptrReset){this->ptr = ptr;this->ptrReset=ptrReset;};
        // Get pointer for >> operator
        inline std::size_t getPtr() const {return this->ptr;};
 
        // Number of T in the point vector (note: This is this->stride() * number of actual points. Use this->pointCount for number of points.)
        inline std::size_t size() const {return this->p.size();};
        // Number of points in point vector
        inline std::size_t pointCount() const {return this->p.size() / this->stride();}

        // Operator for wrapping this->add(T*)
        inline void operator+=(const T *p){this->add(p);};
        // Operator for wrapping this->add(Point)
        inline void operator+=(const Point p){this->add(p);};
        // Operator for wrapping this->add(T*) with chaining
        inline PointsT &operator<<(const T *p){(*this+=p);return *this;};
        // Operator for wrapping this->add(Point) with chaining
        inline PointsT &operator<<(const Point p){(*this+=p);return *this;};
        // Operator for wrapping this->getPoint(std::size_t), with built in counter for linear traversal
        inline PointsT &operator>>(Point &p){if(this->ptr>=this->pointCount()){this->ptr=0;this->ptrReset=true;}else{this->ptrReset=false;};p=this->getPoint(this->ptr++);return *this;};
        // Operator for checking if linear traversal through >> is finished (checks if ptr is 0)
        inline operator bool(){if(this->ptrReset){this->ptrReset=false;this->ptr=0;return false;}return true;};
        // Operator for wrapping this->remove(std::size_t)
        inline void operator-=(std::size_t index){this->remove(index);};

        // Operator for wrapping this->getPoint(index) using array access notation
        inline Point operator[](std::size_t index){return this->getPoint(index);};
        // Operator for retreiving a parameter from a point
        T *operator()(std::size_t pointIndex, std::size_t parameterIndex);
        // Const version of operator for retreiving a parameter from a point
        const T *operator()(std::size_t pointIndex, std::size_t parameterIndex) const;
        // Operator for retreiving a dimention from a parameter from a point
        T &operator()(std::size_t pointIndex, std::size_t parameterIndex, std::size_t dimensionIndex);
        // Const version of operator for retreiving a dimention from a parameter from a point
        const T &operator()(std::size_t pointIndex, std::size_t parameterIndex, std::size_t dimensionIndex) const;
    };

    typedef PointsT<float> Points;
}