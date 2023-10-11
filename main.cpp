#include "scraping-handler.h"
// More info and understanding
// https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus

int main(int argc, char* argv[]){
    // Make sure that enough arguments have been provided
    if (argc < 2)
    {
        std::cout << "Not enough arguments, add at least 1" << std::endl;
        return 1;
    }

    std::vector<std::string> startingURLS;

    // preallocate the vector with the size of arguments.
    // It is not worth preallocating when the vector is reallocated
    // Read more here:
    // https://stackoverflow.com/questions/11888265/is-it-more-efficient-to-preallocate-a-vector
    startingURLS.reserve(argc);

for (int i = 0; i < argc; i++)
    {
        startingURLS.emplace_back(argv[i]);
    }

    // Create scraper object
    Scraper scraper;
    AnalyzePages pageAnalyzer;

    //urls vector
    std::vector<std::string> urls;

    // Page data data structure
    std::vector<pageData> data;

    for (int k = 0; k < argc; k++) {
        // Get info from website
        scraper.baseURL = startingURLS[k];
        cpr::Response r = scraper.request_info(startingURLS[k]);

        // Parse it
        urls = scraper.ParseContent(r.text, (char *) "href", (char *) "/");

        // Iterate through them
        for (const std::string &item: urls) {
            std::cout << item << std::endl;
            cpr::Response res = pageAnalyzer.request_info(item);
            pageAnalyzer.analyzeEntry(item);
        }

    }

    std::cout << "Done press enter to complete.";
    std::cin.get();
    return 0;
}
