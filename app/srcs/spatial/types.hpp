#pragma once

namespace spatial {

    using Coordinates = bgeo::model::point<
        double,
        2,
        bgeo::cs::spherical_equatorial<bgeo::degree>
    >;

    struct RTreeValue {
        Coordinates coordinates;
        uint id;

        struct IndexableGetter {
            using result_type = const Coordinates &;

            result_type operator()(const RTreeValue & value) const {
                return value.coordinates;
            }
        };

        struct EqualTo {
            bool operator()(const RTreeValue & v1, const RTreeValue & v2) const {
                return (v1.id == v2.id);
            }
        };
    };

    using Index = bgi::rtree<
        RTreeValue,
        bgi::rstar<16>,
        RTreeValue::IndexableGetter,
        RTreeValue::EqualTo
    >;
}
