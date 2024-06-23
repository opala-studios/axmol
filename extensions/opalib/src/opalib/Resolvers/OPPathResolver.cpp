//
// Created by Talles Borges  on 29/07/20.
//

#include "OPPathResolver.hpp"

std::string OPPathResolver::getFetchUrlPath(int64_t timestamp, int apiVersion)
{
    std::string urlParams = "?version=" + std::to_string(apiVersion) + "&timestamp=" + std::to_string(timestamp);
    return "/api/fetch" + urlParams;
}
