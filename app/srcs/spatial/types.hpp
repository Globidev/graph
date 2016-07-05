#pragma once

namespace spatial {

    using Coordinates = bgeo::model::point<
        double,
        2,
        bgeo::cs::spherical_equatorial<bgeo::degree>
    >;

    using Index = bgi::rtree<
        std::pair<Coordinates, uint>,
        bgi::rstar<16>
    >;
}
