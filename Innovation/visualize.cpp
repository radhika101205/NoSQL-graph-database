#include "visualize.hpp"
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>
#include <nlohmann/json.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

enum NodeShape
{
    CIRCLE,
    SQUARE,
    TRIANGLE
};
enum ColorTheme
{
    DEFAULT,
    DARK,
    PASTEL,
    HIGH_CONTRAST
};
enum GraphLayout
{
    CIRCULAR,
    RANDOM,
    FORCE_DIRECTED,
    GRID
};

class GraphVisualizer
{
private:
    sf::RenderWindow window;
    sf::Font font;
    TextGraph &textGraph;
    float radius = 30.0f;
    float centerX, centerY;
    NodeShape currentShape = CIRCLE;
    ColorTheme currentTheme = DEFAULT;
    GraphLayout currentLayout = CIRCULAR;
    sf::View view;
    float zoomLevel = 1.0f;

    struct Node
    {
        float x, y, radius;
        sf::Text text;
    };

    struct Edge
    {
        std::string from;
        std::string to;
        std::string relation;
    };

    std::unordered_map<std::string, Node> nodes;
    std::vector<Edge> edges;

    sf::Color getNodeColor()
    {
        switch (currentTheme)
        {
        case DARK:
            return sf::Color(70, 130, 180);
        case PASTEL:
            return sf::Color(255, 182, 193);
        case HIGH_CONTRAST:
            return sf::Color(255, 69, 0);
        default:
            return sf::Color(100, 149, 237);
        }
    }

    sf::Color getBackgroundColor()
    {
        switch (currentTheme)
        {
        case DARK:
            return sf::Color(30, 30, 40);
        case PASTEL:
            return sf::Color(240, 240, 245);
        case HIGH_CONTRAST:
            return sf::Color::White;
        default:
            return sf::Color(230, 230, 240);
        }
    }

    void applyCircularLayout()
    {
        int n = nodes.size();
        if (n == 0)
            return;

        float angleIncrement = 2 * M_PI / n;
        float layoutRadius = std::min(centerX, centerY) * 0.7f;
        int index = 0;

        for (auto &pair : nodes)
        {
            float angle = index * angleIncrement;
            pair.second.x = centerX + layoutRadius * cos(angle);
            pair.second.y = centerY + layoutRadius * sin(angle);
            pair.second.text.setPosition(pair.second.x, pair.second.y);
            index++;
        }
    }

    void applyRandomLayout()
    {
        for (auto &nodePair : nodes)
        {
            nodePair.second.x = rand() % (window.getSize().x - 200) + 100;
            nodePair.second.y = rand() % (window.getSize().y - 200) + 100;
            nodePair.second.text.setPosition(nodePair.second.x, nodePair.second.y);
        }
    }

    void applyGridLayout()
    {
        int cols = std::ceil(std::sqrt(nodes.size()));
        int i = 0;
        for (auto &nodePair : nodes)
        {
            int row = i / cols;
            int col = i % cols;
            nodePair.second.x = 150 + col * 150;
            nodePair.second.y = 150 + row * 150;
            nodePair.second.text.setPosition(nodePair.second.x, nodePair.second.y);
            ++i;
        }
    }

    void applyForceDirectedLayout()
    {
        const float repulsion = 400.0f;
        const float attraction = 0.02f;
        std::unordered_map<std::string, sf::Vector2f> forces;

        for (auto &n1 : nodes)
        {
            forces[n1.first] = {0, 0};
            for (auto &n2 : nodes)
            {
                if (n1.first == n2.first)
                    continue;
                float dx = n1.second.x - n2.second.x;
                float dy = n1.second.y - n2.second.y;
                float dist = std::sqrt(dx * dx + dy * dy) + 0.1f;
                float force = repulsion / (dist * dist);
                forces[n1.first].x += dx * force;
                forces[n1.first].y += dy * force;
            }
        }

        for (auto &edge : edges)
        {
            auto &a = nodes[edge.from], &b = nodes[edge.to];
            float dx = b.x - a.x;
            float dy = b.y - a.y;
            float dist = std::sqrt(dx * dx + dy * dy) + 0.1f;
            float force = attraction * dist;

            forces[edge.from].x += dx * force;
            forces[edge.from].y += dy * force;
            forces[edge.to].x -= dx * force;
            forces[edge.to].y -= dy * force;
        }

        for (auto &n : nodes)
        {
            n.second.x += forces[n.first].x * 0.01f;
            n.second.y += forces[n.first].y * 0.01f;
            n.second.text.setPosition(n.second.x, n.second.y);
        }
    }

    void drawNode(const std::string &word)
    {
        const Node &node = nodes[word];
        sf::CircleShape shape(node.radius);
        shape.setPosition(node.x - node.radius, node.y - node.radius);
        shape.setFillColor(getNodeColor());
        window.draw(shape);
        window.draw(node.text);
    }

    void drawEdgeWithLabel(const std::string &from, const std::string &to, const std::string &label)
    {
        const Node &n1 = nodes[from];
        const Node &n2 = nodes[to];

        sf::Vertex line[] = {
            sf::Vertex(sf::Vector2f(n1.x, n1.y), sf::Color(150, 150, 150)),
            sf::Vertex(sf::Vector2f(n2.x, n2.y), sf::Color(150, 150, 150))};
        window.draw(line, 2, sf::Lines);

        sf::Text text;
        text.setFont(font);
        text.setString(label);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::Black);
        text.setOrigin(text.getLocalBounds().width / 2, text.getLocalBounds().height / 2);
        text.setPosition((n1.x + n2.x) / 2, (n1.y + n2.y) / 2);
        window.draw(text);
    }

    void setupNodes()
    {
        const auto &graph = textGraph.getGraph();
        const auto &words = textGraph.getWords();

        std::unordered_set<std::string> seen;
        for (const auto &word : words)
        {
            if (seen.count(word))
                continue;
            seen.insert(word);

            Node node;
            node.radius = radius;
            node.text.setFont(font);
            node.text.setString(word);
            node.text.setCharacterSize(14);
            node.text.setFillColor(sf::Color::Black);
            node.text.setOrigin(node.text.getLocalBounds().width / 2.0f, node.text.getLocalBounds().height / 2.0f + 4);
            nodes[word] = node;
        }

        applyCircularLayout();
    }

public:
    GraphVisualizer(TextGraph &tg, std::vector<std::tuple<std::string, std::string, std::string>> edgeList)
        : window(sf::VideoMode(1200, 800), "Text Graph Visualizer"), textGraph(tg)
    {
        centerX = window.getSize().x / 2.0f;
        centerY = window.getSize().y / 2.0f;
        srand(time(0));

        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf"))
        {
            std::cerr << "Font load failed!" << std::endl;
        }

        for (const auto &e : edgeList)
        {
            edges.push_back({std::get<0>(e), std::get<1>(e), std::get<2>(e)});
        }

        setupNodes();
    }

    void run()
    {
        view = window.getDefaultView();
        bool dragging = false;
        std::string draggedNode;
        sf::Vector2f dragOffset;

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                    for (auto &[name, node] : nodes)
                    {
                        float dx = node.x - mousePos.x;
                        float dy = node.y - mousePos.y;
                        if (std::sqrt(dx * dx + dy * dy) < node.radius)
                        {
                            dragging = true;
                            draggedNode = name;
                            dragOffset = sf::Vector2f(dx, dy);
                            break;
                        }
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased)
                {
                    dragging = false;
                }

                if (event.type == sf::Event::MouseMoved && dragging)
                {
                    sf::Vector2f pos = window.mapPixelToCoords({event.mouseMove.x, event.mouseMove.y});
                    nodes[draggedNode].x = pos.x + dragOffset.x;
                    nodes[draggedNode].y = pos.y + dragOffset.y;
                    nodes[draggedNode].text.setPosition(nodes[draggedNode].x, nodes[draggedNode].y);
                }

                if (event.type == sf::Event::KeyPressed)
                {
                    switch (event.key.code)
                    {
                    case sf::Keyboard::Num1:
                        currentLayout = CIRCULAR;
                        applyCircularLayout();
                        break;
                    case sf::Keyboard::Num2:
                        currentLayout = RANDOM;
                        applyRandomLayout();
                        break;
                    case sf::Keyboard::Num3:
                        currentLayout = GRID;
                        applyGridLayout();
                        break;
                    case sf::Keyboard::Num4:
                        currentLayout = FORCE_DIRECTED;
                        break;
                    case sf::Keyboard::Num5:
                        currentShape = CIRCLE;
                        break;
                    case sf::Keyboard::Num6:
                        currentShape = SQUARE;
                        break;
                    case sf::Keyboard::Num7:
                        currentShape = TRIANGLE;
                        break;
                    case sf::Keyboard::Num8:
                        currentTheme = DEFAULT;
                        break;
                    case sf::Keyboard::Num9:
                        currentTheme = DARK;
                        break;
                    case sf::Keyboard::Num0:
                        currentTheme = PASTEL;
                        break;
                    case sf::Keyboard::R:
                        view = window.getDefaultView();
                        break;
                    default:
                        break;
                    }
                }

                if (event.type == sf::Event::MouseWheelScrolled)
                {
                    if (event.mouseWheelScroll.delta > 0)
                        zoomLevel *= 1.1f;
                    else
                        zoomLevel /= 1.1f;

                    view.zoom(1.0f / zoomLevel);
                }
            }

            if (currentLayout == FORCE_DIRECTED)
            {
                applyForceDirectedLayout();
            }

            window.setView(view);
            window.clear(getBackgroundColor());

            for (const auto &edge : edges)
                drawEdgeWithLabel(edge.from, edge.to, edge.relation);

            for (const auto &node : nodes)
                drawNode(node.first);

            window.display();
        }
    }
};

void visualizeFromGraph(TextGraph &graph)
{
    std::ifstream f("db.json");
    if (!f.is_open())
    {
        std::cerr << "Could not open edges.json\n";
        return;
    }

    nlohmann::json j;
    f >> j;

    std::vector<std::tuple<std::string, std::string, std::string>> edgeList;
    for (auto &e : j)
    {
        std::string source = e["source"];
        std::string target = e["target"];
        std::string relation = e.value("relation", "related");

        edgeList.emplace_back(source, target, relation);

        graph.getGraph()[source].insert(target);
        graph.getGraph()[target].insert(source);
        graph.getWords().push_back(source);
        graph.getWords().push_back(target);
    }

    if (edgeList.empty())
    {
        std::cout << "No edges found. Please check your JSON file.\n";
        return;
    }

    GraphVisualizer gv(graph, edgeList);
    gv.run();
}
