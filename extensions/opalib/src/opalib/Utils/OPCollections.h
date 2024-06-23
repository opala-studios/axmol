
#ifndef OPALIBTEMPLATE_OPCOLLECTIONS_H
#define OPALIBTEMPLATE_OPCOLLECTIONS_H
#include <functional>
#include <set>

class OPCollections {
public:
    template<typename C, typename E>
    static bool contains(const C& container, const E& element) {
        return container.find(element) != container.end();
    }
    template<typename E>
    static void erase_if(std::set<E>& c, std::function<bool(E&)> predicate) {
        auto last = c.end();
        for (auto i = c.begin(); i != last;) {
            if (predicate(*i)) {
                i = c.erase(i);
            } else {
                ++i;
            }
        }
    }
    template<typename E>
    static void erase_if(std::vector<E>& c, std::function<bool(E&)> predicate) {
        c.erase(std::remove_if(c.begin(), c.end(), predicate), c.end());
    }
    
    template<typename T, typename C>
    static const T& selectRandom(const C& container) {
        size_t index = rand() % container.size();
        return container[index];
    }
};
#endif
