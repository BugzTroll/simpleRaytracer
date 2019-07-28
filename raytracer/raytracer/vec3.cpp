#include "vec3.h"

inline vec3& vec3::operator+=(const vec3 &v2) {

    e[0] += v2[0];
    e[1] += v2[1];
    e[2] += v2[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3 &v2){
    e[0] -= v2[0];
    e[1] -= v2[1];
    e[2] -= v2[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3 &v2){
    e[0] *= v2[0];
    e[1] *= v2[1];
    e[2] *= v2[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3 &v2){
    
    e[0] /= v2[0];
    e[1] /= v2[1];
    e[2] /= v2[2];
    return *this;
}

inline vec3& vec3::operator*=(const float t){
    
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(const float t) {

    float k = 1 / t;
    e[0] *= k;
    e[1] *= k;
    e[2] *= k;
    return *this;
}

inline void vec3::make_unit_vector(){
    float norm = this->length();
    *this /= norm;
}

inline std::istream& operator>>(std::istream &is, vec3& v){
    is >> v.e[0] >> v.e[1] >> v.e[2];
    return is;
}
inline std::ostream& operator<<(std::ostream &os, const vec3& v){
    os << v.e[0] << " " << v.e[1] << " " << v.e[2];
    return os;
}

inline vec3 operator+(const vec3 &v1, const vec3 &v2) {
    return vec3(v1.e[0] + v2.e[0], v1.e[1] + v2.e[1], v1.e[2]+ v2.e[2]);
}

inline vec3 operator-(const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0] - v2.e[0], v1.e[1] - v2.e[1], v1.e[2] - v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0] * v2.e[0], v1.e[1] * v2.e[1], v1.e[2] * v2.e[2]);
}

inline vec3 operator/(const vec3 &v1, const vec3 &v2){
    return vec3(v1.e[0] / v2.e[0], v1.e[1] / v2.e[1], v1.e[2] / v2.e[2]);
}

inline vec3 operator*(const vec3 &v1, float t){
    return vec3(v1.e[0] * t, v1.e[1] * t, v1.e[2] * t);
}

inline vec3 operator/(const vec3 &v1, float t){
    return vec3(v1.e[0] / t, v1.e[1] / t, v1.e[2] / t);
}

inline float dot(const vec3 &v1, const vec3 &v2){
    return v1.e[0] * v2.e[0] + v1.e[1] * v2.e[1] + v1.e[2] * v2.e[2];
}
inline vec3 cross(const vec3 &v1, const vec3 &v2){
    return vec3((v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1]),
                (-v1.e[0] * v2.e[2] - v1.e[2] * v2.e[0]),
                (v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]));
}

inline vec3 unit_vector(const vec3 &v) {
    return v / v.length();
}