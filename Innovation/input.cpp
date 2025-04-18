#include <iostream>
#include "visualize.hpp"
#include "textgraph.hpp"

int main()
{
    TextGraph graph;
    visualizeFromGraph(graph); // all JSON and edge processing happens inside this
    return 0;
}
