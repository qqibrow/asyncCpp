//
// Created by Lu Niu on 8/28/15.
//

#ifndef PROJECTTEMPLATE_EACH_H
#define PROJECTTEMPLATE_EACH_H
#include <vector>

namespace Async {
    template<typename T>
    struct Each {
        // here func should be a side-effect async func
        void operator()(const std::vector<T>& list, std::function<void(T)> func) {
            for(const auto elem: list) {
                func(elem);
            }
        }
    };

    template <typename Map>
    struct ForEachOf {
        void operator()(const Map& map, std::function<void (typename Map::key_type, typename Map::mapped_type)> func) {
            for(const auto& pair : map) {
                func(pair.first, pair.second);
            }
        }
    };

    template<typename IType, typename Otype>
    struct Map {
        void operator()(const std::vector<IType>& list, std::function<void (IType, std::function<void(Otype)>)> func,
        std::function<void(const std::vector<Otype>& )> collector) {
            std::vector<Otype> olist(list.size());
            for(int i = 0; i < list.size(); ++i) {
                auto cb = [&](Otype output) {
                    olist[i] = output;
                };
                func(list[i], cb);
            }
            // monitor that olist is complete and then call
            collector(olist);
        }
    };


}




#endif //PROJECTTEMPLATE_EACH_H
