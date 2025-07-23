#include "splinekit/Curve.hpp"

#include <cstring>
#include <cmath>
#include <queue>

using namespace splinekit;

template <class T>
T calculateMagnitudeSq(T *data, std::size_t count){
    T res = 0.0;
    for(std::size_t i=0;i<count;++i){
        res += data[i] * data[i];
    }
    return res;
}
template <class T>
T calculateMagnitude(T *data, std::size_t count){
    T res = 0.0;
    for(std::size_t i=0;i<count;++i){
        res += data[i] * data[i];
    }
    return std::sqrt(res);
}
template <class T>
T calculateDot(T *a, T *b, std::size_t count){
    T res = 0.0;
    for(std::size_t i=0;i<count;++i){
        res += a[i] * b[i];
    }
    return res;
}


template <class T, template <class> class WM>
void CurveT<T,WM>::calculateExtrema(std::vector<Extrema> &res, std::size_t segmentIndex, uint16_t derivative, bool ignoreEnd, bool max, bool min, T maxError, std::size_t maxIterations, std::size_t lengthIterations, T alpha, T minStep, T maxStep) const {
    if(!max && !min){
        return;
    }
    if(maxIterations == 0){
        maxIterations = UINT64_MAX;
    }
    uint16_t order = this->weightManager.get()->getOrder() - derivative;
    std::size_t dimensions = this->points.getDimensions();

    T length = this->calculateLengthDeltaPos(segmentIndex, maxIterations);

    if(order == 0){
        // For a point, take the beginning of the spline as the extrema, since the entire segment is an extrema
        std::unique_ptr<T[]> vec = std::make_unique<T[]>(dimensions);
        this->calculatePoint(vec.get(),0.0,derivative);
        res.push_back({(T)segmentIndex, std::move(vec)});
    }else if(order == 1){
        // For a line (lerp), only check the beginning and end, they are the only possible extrema
        std::unique_ptr<T[]> vecA = std::make_unique<T[]>(dimensions);
        std::unique_ptr<T[]> vecB = std::make_unique<T[]>(dimensions);
        this->calculatePoint(vecA.get(),(T)(segmentIndex),derivative,false);
        this->calculatePoint(vecB.get(),(T)(segmentIndex+1.0),derivative,true);
        if(min && max){
            res.push_back({(T)(segmentIndex), std::move(vecA)});
            res.push_back({(T)(segmentIndex+1.0), std::move(vecB)});
        }else{
            T magAsq = calculateMagnitudeSq(vecA.get(),dimensions);
            T magBsq = calculateMagnitudeSq(vecB.get(),dimensions);
            if(magAsq < magBsq){
                if(min){
                    res.push_back({(T)(segmentIndex), std::move(vecA)});
                }else{
                    if(!ignoreEnd){
                        res.push_back({(T)(segmentIndex+1.0), std::move(vecB)});
                    }
                }
            }else{ // max == true
                if(max){
                    res.push_back({(T)(segmentIndex), std::move(vecA)});
                }else{
                    if(!ignoreEnd){
                        res.push_back({(T)(segmentIndex+1.0), std::move(vecB)});
                    }
                }
            }
        }
    }else{
        alpha = std::abs(alpha) / length;
        // For higher orders, take the beginning and end as extrema and use gradient descent
        auto start = res.end();
        // Structure for shorthand calculations
        struct Guess{
            const std::size_t dimensions;
            T t; T velMagSq; T velMag; T dot;
            T *vel; T *acc;
            Guess(std::size_t dimensions):dimensions{dimensions},vel{nullptr},acc{nullptr}{};
            Guess(T t, std::size_t dimensions, uint16_t derivative, const CurveT<T,WM> *parent, bool end):t{t},dimensions{dimensions},vel{new T[this->dimensions]},acc{new T[this->dimensions]}{
                parent->calculatePoint(this->vel,t,derivative,end);
                parent->calculatePoint(this->acc,t,derivative+1,end);
                this->velMagSq = calculateMagnitudeSq(this->vel,this->dimensions);
                this->velMag = std::sqrt(this->velMagSq);
                this->dot = calculateDot(this->vel,this->acc,this->dimensions);
            }
            Guess(const Guess &g):t{g.t},dimensions{g.dimensions},velMagSq{g.velMagSq},velMag{g.velMag},dot{g.dot},vel{new T[this->dimensions]},acc{new T[this->dimensions]}{
                memcpy(this->vel,g.vel,this->dimensions*sizeof(T));
                memcpy(this->acc,g.acc,this->dimensions*sizeof(T));
            }
            Guess(Guess &&g):t{g.t},dimensions{g.dimensions},velMagSq{g.velMagSq},velMag{g.velMag},dot{g.dot},vel{g.vel},acc{g.acc}{
                g.vel = nullptr;
                g.acc = nullptr;
            }
            ~Guess(){
                if(this->vel != nullptr){delete[] this->vel;};
                if(this->acc != nullptr){delete[] this->acc;};
            }
            void operator()(T t, uint16_t derivative, const CurveT<T,WM> *parent, bool end){
                if(this->vel == nullptr){this->vel = new T[this->dimensions];};
                if(this->acc == nullptr){this->acc = new T[this->dimensions];};
                this->t = t;
                parent->calculatePoint(this->vel,t,derivative,end); parent->calculatePoint(this->acc,t,derivative+1,end);
                this->velMagSq = calculateMagnitudeSq(this->vel,this->dimensions);
                this->velMag = std::sqrt(this->velMagSq);
                this->dot = calculateDot(this->vel,this->acc,this->dimensions);
            };
            void operator=(Guess &&g){
                if(&g == this){
                    return;
                }
                if(this->vel != nullptr){delete[] this->vel;};
                if(this->acc != nullptr){delete[] this->acc;};
                this->t = g.t; this->velMagSq = g.velMagSq; this->velMag = g.velMag; this->dot = g.dot;
                this->vel = g.vel; this->acc = g.acc;
                g.vel = nullptr; g.acc = nullptr;
            }
            void operator=(const Guess &g){
                if(&g == this){
                    return;
                }
                if(this->vel != nullptr){delete[] this->vel;};
                if(this->acc != nullptr){delete[] this->acc;};
                this->t = g.t; this->velMagSq = g.velMagSq; this->velMag = g.velMag; this->dot = g.dot;
                this->vel = new T[this->dimensions];
                this->acc = new T[this->dimensions];
                memcpy(this->vel,g.vel,this->dimensions*sizeof(T));
                memcpy(this->acc,g.acc,this->dimensions*sizeof(T));
            }
            std::unique_ptr<T[]> getUniqueVel(){
                std::unique_ptr<T[]> vec = std::make_unique<T[]>(this->dimensions);
                memcpy(vec.get(),this->vel,sizeof(T)*this->dimensions);
                return std::move(vec);
            }
        };
        // First pass
        {
            Guess prevGuess{dimensions};
            Guess nextGuess{(T)segmentIndex, dimensions, derivative, this, false};
            // First point is an extrema
            if((nextGuess.dot >= 0 && min) || (nextGuess.dot <= 0 && max)){
                res.push_back({(T)(segmentIndex), nextGuess.getUniqueVel()});
            }
            // Find rest of points
            T guesst = (T)segmentIndex;
            std::size_t iterations = 0;
            T step = minStep;
            std::queue<std::pair<Guess, Guess>> dotRoots;

            while(iterations++ < maxIterations){
                prevGuess = std::move(nextGuess);
                guesst += step;

                if(guesst >= (T)(segmentIndex+1.0)){
                    break;
                }

                nextGuess(guesst, derivative, this, false);

                bool lMax = prevGuess.dot > 0 && nextGuess.dot < 0;
                bool lMin = prevGuess.dot < 0 && nextGuess.dot > 0;
                if(lMax || lMin){
                    step = minStep;
                    if((max && lMax) || (min && lMin)){
                        dotRoots.push(std::make_pair(Guess{prevGuess}, Guess{nextGuess}));
                    }
                }else{
                    T dvdt = (nextGuess.velMag - prevGuess.velMag) / step;
                    step = std::min(maxStep, std::max(minStep, std::abs(dvdt * alpha)));
                }
            }
            while(!dotRoots.empty()){
                std::pair<Guess, Guess> brackets = dotRoots.front();
                if(brackets.first.dot == 0){
                    res.push_back({brackets.first.t, brackets.first.getUniqueVel()});
                }else if(brackets.second.dot == 0){
                    res.push_back({brackets.second.t, brackets.second.getUniqueVel()});
                }else{
                    Guess temp{dimensions};
                    while(true){
                        temp((brackets.first.t + brackets.second.t)*0.5, derivative, this, false);
                        T error = std::max(std::abs(temp.velMag - brackets.first.velMag), std::abs(temp.velMag - brackets.second.velMag));
                        if((iterations++ > maxIterations - dotRoots.size()) || (error < maxError)){
                            break;
                        }else{
                            if((temp.dot > 0) == (brackets.first.dot > 0)){
                                brackets.first = std::move(temp);
                            }else{
                                brackets.second = std::move(temp);
                            }
                        }
                    }
                    res.push_back({temp.t, temp.getUniqueVel()});
                }
                dotRoots.pop();
            }
            // Last point is an extrema
            if(!ignoreEnd){
                nextGuess((T)(segmentIndex+1.0),derivative,this,true);
                if((nextGuess.dot >= 0 && min) || (nextGuess.dot <= 0 && max)){
                    res.push_back({(T)(segmentIndex), nextGuess.getUniqueVel()});
                }
            }
        }
    }
}
template <class T, template <class> class WM>
void CurveT<T,WM>::calculateAllExtrema(std::vector<Extrema> &res, uint16_t derivative, bool ignoreEnd, bool max, bool min, T maxError, std::size_t maxIterations, std::size_t lengthIterations, T alpha, T minStep, T maxStep) const {
    for(std::size_t i=0;i<this->points.pointCount();++i){
        this->calculateExtrema(res, i, derivative, ignoreEnd, max, min, maxError, maxIterations, lengthIterations, alpha, minStep, maxStep);
    }
}


template class CurveT<float,WeightManagerMatT>;
template class CurveT<double,WeightManagerMatT>;