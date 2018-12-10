#include "sky.hpp"
#include <algorithm>
#include <cstdio>
#include <iostream>

Star::Star(std::string line) {
    std::sscanf(line.c_str(), "position=<%d, %d> velocity=<%d, %d>", &pos[0],
                &pos[1], &vel[0], &vel[1]);
}

void Star::advance() {
    pos[0] += vel[0];
    pos[1] += vel[1];
}

void Star::print() const {
    std::cout << "X : " << pos[0] << " (" << vel[0] << ")\tY : " << pos[1]
              << " (" << vel[1] << ")\n";
}

long unsigned int Sky::get_width() const {
    auto minmax_pair =
        std::minmax_element(stars.begin(), stars.end(),
                    [](auto s1, auto s2) { return s1.x() < s2.x(); });
    /* std::cout << "Min width : " << minmax_pair.first->x() */
    /*           << "\tMax width : " << minmax_pair.second->x() << "\n"; */
    return static_cast<long unsigned int>(
        std::abs(minmax_pair.second->x() - minmax_pair.first->x()));
}

long unsigned int Sky::get_height() const {
    auto minmax_pair =
        std::minmax_element(stars.begin(), stars.end(),
                    [](auto s1, auto s2) { return s1.y() < s2.y(); });
    /* std::cout << "Min height : " << minmax_pair.first->x() */
    /*           << "\tMax height : " << minmax_pair.second->x() << "\n"; */
    return static_cast<long unsigned int>(
        std::abs(minmax_pair.second->y() - minmax_pair.first->y()));
}

long unsigned int Sky::get_fut_width() const {
    auto minmax_pair =
        std::minmax_element(stars.begin(), stars.end(),
                    [](auto s1, auto s2) { return s1.fut_x() < s2.fut_x(); });
    /* std::cout << "Min future width : " << minmax_pair.first->x() */
    /*           << "\tMax future width : " << minmax_pair.second->x() << "\n"; */
    return static_cast<long unsigned int>(
        std::abs(minmax_pair.second->fut_x() - minmax_pair.first->fut_x()));
}

long unsigned int Sky::get_fut_height() const {
    auto minmax_pair =
        std::minmax_element(stars.begin(), stars.end(),
                    [](auto s1, auto s2) { return s1.fut_y() < s2.fut_y(); });
    /* std::cout << "Min future height : " << minmax_pair.first->x() */
    /*           << "\tMax future height : " << minmax_pair.second->x() << "\n"; */
    return static_cast<long unsigned int>(
        std::abs(minmax_pair.second->fut_y() - minmax_pair.first->fut_y()));
}

void Sky::advance() {
    std::for_each(stars.begin(), stars.end(), [](Star& s) { s.advance(); });
}

void Sky::print() const {
    std::cout << "Star list :\n";
    for (auto star : stars) {
        star.print();
    }
    std::cout << "\n";
    auto minmax_height =
        std::minmax_element(stars.begin(), stars.end(),
                    [](auto s1, auto s2) { return s1.y() < s2.y(); });
    auto minmax_width =
        std::minmax_element(stars.begin(), stars.end(),
                    [](auto s1, auto s2) { return s1.x() < s2.x(); });

    for (int j = minmax_height.first->y(); j <= minmax_height.second->y();
         j++) {
        for (int i = minmax_width.first->x(); i <= minmax_width.second->x();
             i++) {
            if (std::any_of(stars.begin(), stars.end(),
                            [&](auto s) { return s.x() == i && s.y() == j; })) {
                std::cout << "#";
            } else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}
