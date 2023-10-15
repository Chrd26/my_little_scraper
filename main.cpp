#include "scraping-handler.h"
#include <wx/wx.h>
#include "csv_handler.h"
// More info and understanding
// https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus
// preallocate the vector with the size of arguments.
// It is not worth preallocating when the vector is reallocated
// Read more here:
// https://stackoverflow.com/questions/11888265/is-it-more-efficient-to-preallocate-a-vector

int main(){
    CSV_Handler csv;
    // std::vector<std::string> startingURLS;

    csv.ReadFile();

    // Create scraper object
    Scraper scraper;
    AnalyzePages pageAnalyzer;

    //urls vector
    std::vector<std::string> urls;

    // Page data data structure
    std::vector<pageData> data;

    // Get info from website
    scraper.baseURL = "ethnos.gr";
    cpr::Response r = scraper.request_info(scraper.baseURL);

    // Parse it
    urls = scraper.ParseContent(r.text, (char *) "href", (char *) "/");

    // Iterate through them
    for (const std::string &item: urls) {
        std::cout << item << std::endl;
        cpr::Response res = pageAnalyzer.request_info(item);
        pageAnalyzer.analyzeEntry(item);

    }

    std::cout << "Done press enter to complete.";
    std::cin.get();
    return 0;
}
