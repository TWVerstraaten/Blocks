//
// Created by pc on 01-02-21.
//

#ifndef BLOCKS_SERIALIZER_H
#define BLOCKS_SERIALIZER_H

#include <fstream>
#include <ostream>

namespace model {
    class Model;
    class Cluster;
    class Level;
} // namespace model

namespace io {

    std::ostream& operator<<(std::ostream& out, const model::Model& model);
    std::ostream& operator<<(std::ostream& out, const model::Cluster& cluster);
    std::ostream& operator<<(std::ostream& out, const model::Level& level);

    std::istream& operator>>(std::istream& in, model::Level& level);
    std::istream& operator>>(std::istream& in, model::Cluster& cluster);
    std::istream& operator>>(std::istream& in, model::Model& model);

} // namespace io

#endif // BLOCKS_SERIALIZER_H
