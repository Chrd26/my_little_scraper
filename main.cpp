#include "scraping-handler.h"

int main(){
    // Create scraper object
    Scraper scraper;

    //urls vector
    std::vector<std::string> urls;

    // Page data data structure
    std::vector<pageData> data;

    // Get info from website
    cpr::Response r = scraper.request_info("https://www.ethnos.gr/tag/141/astynomikhbia");

    // Parse it
    urls = scraper.ParseContent(r.text);

    for (std::string item : urls)
    {
        std::cout << item << std::endl;
    }

    std::cin.get();
    return 0;
}

// More info and understanding
// https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus