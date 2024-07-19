#include <atomic>
#include <iostream>
#include "manifold.h"
#include <Eigen/Dense>
#include <boost/optional.hpp>
#include <boost/variant.hpp>

#ifndef TAXONOMY_H
#define TAXONOMY_H

#define TAXONOMY_USE_SHARED_PTR
#ifdef TAXONOMY_USE_SHARED_PTR

#define DECLARE_PTR(item) \
typedef std::shared_ptr<item> ptr; \
typedef std::shared_ptr<const item> const_ptr;
#endif //TAXONOMY_USE_SHARED_PTR

// ------------------ item ------------------
// IfcUtil::IfcBaseInterface not added;
struct item {
private:
    uint32_t identity_;
    static std::atomic_uint32_t counter_;
    mutable size_t computed_hash_;
public:
    DECLARE_PTR(item)

    boost::optional<bool> orientation;

    item() : identity_(counter_++) {} // missing parameter
    uint32_t identity() const { return identity_; }
};

// ------------------ eigen_base ------------------
namespace {
    template <typename T>
    const T& eigen_defaults();

    template <>
    const Eigen::Vector3d& eigen_defaults<Eigen::Vector3d>(){
        // static: Only create once in memory and by this function, 2nd-> times invocation will skip the next line
        static Eigen::Vector3d identity = Eigen::Vector3d::Zero();
        return identity;
    }

    template <>
    const Eigen::Matrix4d& eigen_defaults<Eigen::Matrix4d>() {
        static Eigen::Matrix4d identity = Eigen::Matrix4d::Identity();
        return identity;
    }
}

template <typename T>
struct eigen_base {
    T* components_;
    // Defualt constructor
    eigen_base() {
        components_ = nullptr;
    }

    // Copy constructor
    eigen_base(const eigen_base& other) {
        this->components_ = other.components_ ? new T(*other.components_) : nullptr;
    }
    // Constructor
    eigen_base(const T& other) {
        this->components_ = new T(other);
    }
    // Copy-assignment operator
    eigen_base& operator=(const eigen_base& other) {
        if (this != &other) {
            this->components_ = other.components_ ? new T(*other.components_) : nullptr;
        }
        return *this;
    }
    // Destructor
    virtual ~eigen_base() {
        delete this->components_;
    }

    const T& ccomponents() const {
        if (this->components_) {
            return *this->components_;
        } else {
            return eigen_defaults<T>();
        }
    }

    T& components() {
        if (!this->components_) {
            this->components_ = new T(eigen_defaults<T>());
        }
        return *this->components_;
    }

    // returning true if components_ is not nullptr.
    explicit operator bool() const {
        return components_;
    }
};

// ------------------ cartesian_base ------------------
// Simplify the base: not eigen_base inheritance, and <Eigen/Dense>
// template <size_t N>
// struct cartesian_base : public item{
//
// public:
//     double x, y, z;
//     cartesian_base(double x, double y, double z = 0.) : x(x), y(y), z(z) {}
//     std::vector<double> coordinates() const {
//         std::vector<double> coor= {x, y, z};
//         return coor;
//     }
// };

template <size_t N>
struct cartesian_base : public item, public eigen_base<Eigen::Vector3d>{
    cartesian_base() : eigen_base() {}
    cartesian_base(const Eigen::Vector3d& c) : eigen_base(c) {}
    cartesian_base(double x, double y, double z = 0.) : eigen_base(Eigen::Vector3d(x, y, z)) {}
};

// ------------------ point3 ------------------
struct point3 : public cartesian_base<3> {
    DECLARE_PTR(point3)
    // point3(double x, double y, double z = 0.) : cartesian_base(x, y, z) {}

    point3(): cartesian_base(){}
    point3(const Eigen::Vector3d& c) : cartesian_base(c) {}
    point3(double x, double y, double z = 0.) : cartesian_base(x, y, z) {}

    // // Ken's addition:
//     bool operator==(const point3 &other) const {
//         return x == other.x && y == other.y && z == other.z;
//     }
};

// // Hash function for the point3 struct to use in unordered_set. Replaceb by boost::hash?
// namespace std {
//     template<>
//     struct hash<point3> {
//         size_t operator()(const point3 &p) const {
//             return hash<double>()(p.x) ^ hash<double>()(p.y) ^ hash<double>()(p.z);
//         }
//     };
// }

// ------------------ trimmed curve ------------------
struct trimmed_curve : public item {
    DECLARE_PTR(trimmed_curve)

	boost::variant<boost::blank, point3::ptr, double> start, end;
    item::ptr basis;

    trimmed_curve(const point3::ptr& a, const point3::ptr& b) : start(a), end(b), basis(nullptr) {}
};

// ------------------ edge ------------------
struct edge : public trimmed_curve {
    DECLARE_PTR(edge)

    edge(const point3::ptr& a, const point3::ptr& b) : trimmed_curve(a, b) {}
};

// ------------------ collection_base ------------------
template <typename T = item>
struct collection_base {
    DECLARE_PTR(collection_base)

    std::vector<typename T::ptr> children;
    collection_base() {}
};

// ------------------ loop ------------------
struct loop : public collection_base<edge> {
    DECLARE_PTR(loop)
    bool closed;

    // Default constructor
    loop() : closed(false) {}
};
#endif //TAXONOMY_H
