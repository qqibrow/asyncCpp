//
// Created by Lu Niu on 8/28/15.
//

#ifndef PROJECTTEMPLATE_EACH_H
#define PROJECTTEMPLATE_EACH_H
#include <vector>
#include <functional>

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
            for(unsigned int i = 0; i < list.size(); ++i) {
                auto cb = [&](Otype output) {
                    olist[i] = output;
                };
                func(list[i], cb);
            }
            // monitor that olist is complete and then call
            collector(olist);
        }
    };

    template<typename T>
    struct Predicate {
        typedef std::function<void (T, std::function<void(bool)>)> f;
    };

    template<typename T>
    struct Filter {
        void operator()(const std::vector<T>& list, typename Predicate<T>::f predicate,
                        std::function<void(const std::vector<T>&)> collector) {
            std::vector<bool> indicators(list.size(), false);
            for(unsigned int i = 0; i < list.size(); ++i) {
                auto setIndicator = [&](bool flag) {
                    indicators[i] = flag;
                };
                predicate(list[i], setIndicator);
            }
            // monitor that check is done then do something.
            std::vector<T> res;
            for(unsigned int i = 0; i < list.size(); ++i) {
                if(indicators[i]) {
                    res.push_back(list[i]);
                }
            }
            collector(res);
        }
    };

    template<typename T>
    struct Compose {
        typedef std::function<void(T accumulate, T now, std::function<void(T)>)>f;
    };

    template<typename T>
    struct Foldl {
        void operator()(const std::vector<T> list, T init, typename Compose<T>::f func,
                        std::function<void(T)> collector) {
            T prev = init;
            for(const auto& now : list) {
                func(prev, now, [&](T result){
                    prev = result;
                });
            }
            collector(prev);
        }
    };



}




#endif //PROJECTTEMPLATE_EACH_H
