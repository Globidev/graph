#pragma once

#include "spatial/types.hpp"
#include "graph/graph.hpp"

namespace web {

    struct Application {

        Application(const graph::Graph &, const spatial::Index &, ushort port);

        void run();

    private:

        const graph::Graph & graph_;
        const spatial::Index & index_;

        ushort port_;
    };

}
