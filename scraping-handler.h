#pragma once

#include <iostream>
#include <cpr/cpr.h>
#include <mlpack/mlpack.hpp>
#include <lexbor/html/parser.h>
#include <lexbor/dom/interfaces/element.h>
#include "lexbor/html/html.h"
#include <lexbor/dom/dom.h>
#include <string>
#include <strings.h>
#include <vector>

// Get Data
typedef struct pageData{
    std::string url;
    std::string content;
    std::string date;
};

// Scraper
class Scraper
{
private:
    static std::vector<std::string> urls;
    // static function can only call other static functions
    // lxb_inline makes a function static!!!
    // See this-> #define lxb_inline static inline
    // Source: https://stackoverflow.com/questions/15687638/c-cannot-call-member-function-inside-the-class
    static void getUrls(char* data);
    lxb_inline lxb_status_t serializer_callback(const lxb_char_t *data, size_t len, void *ctx);
    lxb_inline void serialize_node(lxb_dom_node_t *node);

public:
    cpr::Response request_info(std::string url);
    std::vector<std::string> ParseContent(std::string content);
};