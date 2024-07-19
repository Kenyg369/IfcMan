#include <iostream>
#include "manifold.h"
#include "taxonomy.h"
// #include "util.h"
int main() {
    // // ------------------------------
    // item it1;
    // std::cout << it1.identity() << std::endl;
    // // ------------------------------
    // cartesian_base<3> cb1(1,1);
    // std::cout << cb1.x << std::endl;
    // // ------------------------------
    // point3 pt1(1,1, 2);
    // std::cout << pt1.z << std::endl;
    // // ------------------------------
    // point3 pt2(2,2, 3);
    // trimmed_curve tc1(pt1, pt2);
    // std::cout << tc1.end.z << std::endl;
    // // ------------------------------
    // ---------build a loop in taxomoy
    // create points

    // Create points
    point3::ptr lbP3 = std::make_shared<point3>(1.0, 0.0);
    point3::ptr ltP3 = std::make_shared<point3>(1.0, 1.0);
    point3::ptr rtP3 = std::make_shared<point3>(2.0, 1.0);
    point3::ptr rbP3 = std::make_shared<point3>(2.0, 0.0);

    // Create edges
    edge::ptr left = std::make_shared<edge>(ltP3, lbP3);
    edge::ptr bot = std::make_shared<edge>(lbP3, rbP3);
    edge::ptr right = std::make_shared<edge>(rbP3, rtP3);
    edge::ptr top = std::make_shared<edge>(rtP3, ltP3);

    // Create loop
    loop::ptr loop0 = std::make_shared<loop>();
    loop0->children.push_back(left);
    loop0->children.push_back(bot);
    loop0->children.push_back(right);
    loop0->children.push_back(top);
    // ----------- Experiment Extrusion C++-------------------
    std::cout << "对照组:" << std::endl; // Expecting: 12?
    manifold::Polygons polygons;
    // simple square coorcinates:
    // not a nested vector!!!
    glm::vec2 lb = {1.f , 0.f};
    glm::vec2 lt = glm::vec2(1.f, 1.f);
    glm::vec2 rt = {2.f , 1.f};
    glm::vec2 rb = glm::vec2(2.f, 0.f);
    // // Approach 1
    // //  square sides
    // manifold::SimplePolygon left;
    // left.push_back(lt);
    // left.push_back(lb);
    //
    // manifold::SimplePolygon top;
    // top.push_back(rt);
    // top.push_back(lt);
    //
    // manifold::SimplePolygon right;
    // right.push_back(rb);
    // right.push_back(rt);
    //
    // manifold::SimplePolygon bot;
    // bot.push_back(lb);
    // bot.push_back(rb);
    //
    // polygons.push_back(bot);
    // polygons.push_back(right);
    // polygons.push_back(top);
    // polygons.push_back(left);

    // Approach2  - intersection mess up the logic
    manifold::SimplePolygon loop1;
    loop1.push_back(lb);
    loop1.push_back(rb);
    loop1.push_back(rt);
    loop1.push_back(lt);

    // // 2.a - manifold::SimplePolygon loop2;
    // manifold::SimplePolygon loop2;
    // glm::vec2 lb2 = {2.f , 0.f};
    // glm::vec2 lt2 = glm::vec2(2.f, 1.f);
    // glm::vec2 rt2 = {3.f , 1.f};
    // glm::vec2 rb2 = glm::vec2(3.f, 0.f);
    // loop2.push_back(lb2);
    // loop2.push_back(rb2);
    // loop2.push_back(rt2);
    // loop2.push_back(lt2);
    //
    // polygons.push_back(loop);
    // polygons.push_back(loop2);

    // 2.b - intersection
    manifold::SimplePolygon loop2;
    glm::vec2 lb2 = {1.5f , 0.f};
    glm::vec2 lt2 = glm::vec2(1.5f, 1.f);
    glm::vec2 rt2 = {2.5f , 1.f};
    glm::vec2 rb2 = glm::vec2(2.5f, 0.f);
    loop2.push_back(lb2);
    loop2.push_back(rb2);
    loop2.push_back(rt2);
    loop2.push_back(lt2);

    // polygons.push_back(loop1);
    polygons.push_back(loop2);
    manifold::Manifold cube =  manifold::Manifold::Extrude(polygons, 1, 0, 0);

    std::cout << "# of Tranigle: " << cube.NumTri()<< std::endl; // Expecting: 12?
    std::cout << "# of Edge: " << cube.NumEdge()<< std::endl; // Expecting: 8?
    std::cout << "# of Vertices: " << cube.NumVert()<< std::endl; // Expecting: 8
    std::cout << "Volume: " << cube.GetProperties().volume<< std::endl; // Expecting: 1 (volume)
    std::cout << "Surface: " << cube.GetProperties().surfaceArea<< std::endl; // Expecting: 8 (surface)
    std::cout << "--------------------" << std::endl; // Expecting: 8 (surface)

    // // Random vertices (Not necessary from Thomas, since all the direction)
    // double coordinates[][2] = {
    //     {2.0, 0.0}, {1.0, 0.0}, {2.0, 1.0}, {1.0, 1.0}, {2.0, 1.0}, {1.0, 1.0}, {2.0, 0.0}
    // };
    // // Create point3 objects and add them to the vector
    // std::vector<point3> point3s;
    // for (const auto& coord : coordinates) {
    //     point3s.emplace_back(coord[0], coord[1]);
    // }
    // std::vector<point3> result = process_points(point3s);
    // print_points(result);

    manifold::Polygons polygons2;
    manifold::SimplePolygon loop3;

    // double coordinates[][2] = {{1.0, 0.0},{2.0, 0.0},{2.0,1.0}, {1.0, 1.0}};
    // for (int i = 0; i < std::size(coordinates); i++) {
    //     loop3.push_back(glm::vec2 (static_cast<float>(coordinates[i][0]), static_cast<float>(coordinates[i][1])));
    // }

    std::vector<edge::ptr> children = loop0->children;
    for (int i = 0; i < std::size(children); i++) {
        std::cout << "x, y: " << children[i]->start->ccomponents()[0] << " , "<< children[i]->start->ccomponents()[1]<< std::endl; // Expecting: 12?
        loop3.push_back(glm::vec2 (static_cast<float>(children[i]->start->ccomponents()[0]), static_cast<float>(children[i]->start->ccomponents()[1])));
    }

    polygons2.push_back(loop3);
    manifold::Manifold cube2 =  manifold::Manifold::Extrude(loop3, 1, 0, 0);

    std::cout << "# of Tranigle: " << cube2.NumTri()<< std::endl; // Expecting: 12?
    std::cout << "# of Edge: " << cube2.NumEdge()<< std::endl; // Expecting: 8?
    std::cout << "# of Vertices: " << cube2.NumVert()<< std::endl; // Expecting: 8
    std::cout << "Volume: " << cube2.GetProperties().volume<< std::endl; // Expecting: 1 (volume)
    std::cout << "Surface: " << cube2.GetProperties().surfaceArea<< std::endl; // Expecting: 8 (surface)


    // Eigen::Vector3d v1 {1,2,3};
    // std::cout << "vec3-x: " << v1[0]<< std::endl;
    // std::cout << "vec3-y: " << v1[1]<< std::endl;
    // std::cout << "vec3-z: " << v1[2]<< std::endl;

    return 0;
}
