#include "splinekit/version.hpp"


constexpr splinekit::Version splinekit::LIB_VERSION{0,0,1,'a'};

std::string splinekit::Version::toString() const{
    return "v" + std::to_string(this->MAJOR) + "." + std::to_string(this->MINOR) + "." + std::to_string(this->PATCH) + "-" + this->TAG;
}
bool splinekit::Version::operator==(const splinekit::Version &v) const {
    return this->MAJOR == v.MAJOR && this->MINOR == v.MINOR && this->PATCH == v.PATCH && this->TAG == v.TAG;
}