#pragma once

namespace spatial {

    using Coordinates = bgeo::model::point<
        double,
        2,
        bgeo::cs::geographic<bgeo::degree>
    >;

}
