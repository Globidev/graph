#pragma once

namespace spatial {

    using Coordinates = bgeo::model::point<
        double,
        2,
        bgeo::cs::spherical_equatorial<bgeo::degree>
    >;

}
