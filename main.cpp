#include "scraping-handler.h"

int main(){

    Scraper scraper;

    cpr::Response r = scraper.request_info("https://www.ethnos.gr/tag/141/astynomikhbia");

    std::cout << "START OUTPUT!!!" << std::endl;

    std::cout << r.url << std::endl;
    //std::cout << r.text << std::endl;

    scraper.ParseContent(r.text);


    return 0;
}

// More info and understanding
// https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus