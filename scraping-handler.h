#pragma once

#include <iostream>
#include <cpr/cpr.h>
#include <mlpack/mlpack.hpp>
#include <lexbor/html/parser.h>
#include <lexbor/dom/interfaces/element.h>
#include "lexbor/html/html.h"
#include <string>

// Get Data
struct pageData{
    std::string url;
    std::string content;
    std::string date;
};

// Scraper
class Scraper
{
private:
    lxb_inline lxb_status_t serializer_callback(const lxb_char_t *data, size_t len, void *ctx);
    lxb_inline void serialize_node(lxb_dom_node_t *node);

public:
    cpr::Response request_info(std::string url);
    void ParseContent(std::string content);
};