#pragma once

namespace osm {

    const std::unordered_set<std::string> ROAD_FILTERS = {
        // Roads
        "motorway",
        "trunk",
        "primary",
        "secondary",
        "tertiary",
        "unclassified",
        "residential",
        "service",
        // Link roads
        "motorway_link",
        "trunk_link",
        "primary_link",
        "secondary_link",
        "tertiary_link",
        // Special road types
        "living_street",
        "pedestrian",
        "track",
        "bus_guideway",
        "road",
    };

}
