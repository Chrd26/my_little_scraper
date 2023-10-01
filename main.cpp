#include "scraping-handler.h"
// More info and understanding
// https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus

int main(){
    // Create scraper object
    Scraper scraper;
    AnalyzePages pageAnalyzer;

    //urls vector
    std::vector<std::string> urls;

    // Page data data structure
    std::vector<pageData> data;

    // Get info from website
    cpr::Response r = scraper.request_info("https://www.ethnos.gr/tag/141/astynomikhbia");

    // Parse it
    urls = scraper.ParseContent(r.text);

    // Iterate through them
    for (const std::string& item : urls)
    {
        cpr::Response res = pageAnalyzer.request_info(item);
        pageAnalyzer.analyzeEntry(item);

    }

    std::cin.get();
    return 0;
}
