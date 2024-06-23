//
//  Algorithm.h
//  opalib
//
//  Created by Murilo Kinape on 09/07/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef Algorithm_h
#define Algorithm_h

namespace opalib
{

    // Removes elements from Input and places them in Output if the predicate is satisfied.
    template<class InputIt, class OutputIt, class UnaryPredicate>
    InputIt splice_if(InputIt first, InputIt last,
                            OutputIt out, UnaryPredicate p)
    {
        InputIt result = first;
        for (; first != last; ++first) {
            if (p(*first)) {
                *result++ = *first;
            } else {
                *out++ = *first;
            }
        }
        return result;
    }

    template<typename T>
    void removeDuplicates(std::vector<T>& vec) //From: https://stackoverflow.com/a/24477023
    {
        std::unordered_set<T> set;
        for (T i: vec)
            set.insert(i);
        vec.assign(set.begin(), set.end());
    }

};

#endif /* Algorithm_h */
