#pragma once

#include <iostream>
#include <cpr/cpr.h>
#include <mlpack/mlpack.hpp>
#include <lexbor/html/parser.h>
#include <lexbor/dom/interfaces/element.h>
#include "lexbor/html/html.h"
#include <lexbor/dom/dom.h>
#include <lexbor/dom/interfaces/document.h>
#include <string>
#include <vector>
#include <thread>
#include <boost/locale.hpp>
#include <boost/algorithm/string/case_conv.hpp>

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

public:
    static bool isCanceled;
    static bool analysis;
    static std::vector<std::string> keywords;
    static std::string baseURL;
    static cpr::Response request_info(std::string url);
    lxb_inline void serialize_node(lxb_dom_node_t *node);
    static std::vector<std::string> ParseContent(std::string content);
    lxb_inline lxb_html_document_t* Parse(const lxb_char_t* html, size_t html_len);
    static void SetupScraper(std::vector<std::string> inputKeywords, std::string url);
    static bool CheckForConnection();
    static bool hasStarted;
    static bool hasDisconnected;
};

class AnalyzePages
{
public:
    static bool hasStarted;
    static void analyzeEntry(std::string input, std::vector<std::string> grabKeywords, Scraper scraper);
};