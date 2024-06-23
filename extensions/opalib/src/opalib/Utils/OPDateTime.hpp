//
//  OPDateTime.hpp
//  opalib-ios
//
//  Created by Opala Studios on 03/09/21.
//  Copyright © 2021 Opala Studios. All rights reserved.
//

#ifndef OPDateTime_hpp
#define OPDateTime_hpp

#include <memory>
#include <ctime>

#define OP_HOURS_IN_DAY 24
#define OP_SECONDS 60
#define OP_SECONDS_IN_DAY OP_HOURS_IN_DAY * OP_SECONDS * OP_SECONDS

class OPDateTime
{
public:
    enum Flag
    {
        DAY,
        MONTH,
        YEAR,
        DAY_WEEK,
        DAY_YEAR,
        HOUR,
        MINUTE,
        SECOND
    };
    
public:
    OPDateTime();
    ~OPDateTime();
    
    static std::shared_ptr<OPDateTime>& shared();
    
    OPDateTime& setTimePoint(const time_t time);
    
    int get(const Flag flag) const;

    time_t getCurrentTimePoint() const;
    
    void update();
    
private:
    tm* getLocalTime(const time_t timePoint = -1);
    
private:
    std::shared_ptr<OPDateTime> _shared;
    tm* _localtime;
};

#endif /* OPDateTime_hpp */
