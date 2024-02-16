#include "scraping-handler.h"
std::string Scraper::baseURL;
std::vector<std::string> Scraper::keywords = {"Hello"};

bool Scraper::CheckForConnection()
{
    std::string createCommand = "ping www.google.com -c1";
    int getVal = std::system(createCommand.c_str());

    if (getVal != 0)
    {
        return false;
    }

    return true;
}

void Scraper::SetupScraper(std::vector<std::string> inputKeywords, std::string url)
{
    keywords.clear();
    baseURL = url;
    keywords = inputKeywords;
}

// You need to define static variables to use them, a declaration is not enough
std::vector<std::string> Scraper::urls;
bool Scraper::analysis = false;

// Get Info
cpr::Response Scraper::request_info(std::string url)
{
    // Use a user agent to mask the scraper
    // Source: https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus
    cpr:: Header headers = {{"User-Agent",
                            "Mozilla/5.0 (Macintosh; Intel Mac OS X 10.15; rv:109.0) Gecko/20100101 Firefox/117.0"}};

    // Get Data
    // Source: https://docs.libcpr.org/introduction.html
    cpr::Response r = cpr::Get(cpr::Url{url}, headers);

    return r;
}

// Parse content
lxb_html_document_t* Scraper::Parse(const lxb_char_t* html, size_t html_len)
{
   lxb_status_t status;
   lxb_html_parser_t* parser;
   lxb_html_document_t* document;

   // Initialize

   parser = lxb_html_parser_create();
   status = lxb_html_parser_init(parser);

   if (status != LXB_STATUS_OK)
   {
       exit(EXIT_FAILURE);
   }

   // Parse
   document = lxb_html_parse(parser, html,html_len);

   if (document == nullptr)
   {
       exit(EXIT_FAILURE);
   }

   // Destroy parser
   lxb_html_parser_destroy(parser);

   return document;
}

// Get urls from the serializer_callback
void Scraper::getUrls(char* data)
{
   std::string getData = (std::string) data;

   // characters use single quotes!!!
   char symbol = '/';
   char dataFirstChar = getData.at(0);
   if (dataFirstChar == symbol)
   {
       urls.push_back(baseURL + getData);
   }
}

// serializer
lxb_status_t Scraper::serializer_callback(const lxb_char_t *data, size_t len, void *ctx)
{

    //printf("%.*s", (int) len, (const char *) data);
    if (analysis)
    {
        std::cin.get();

    }

   if (!analysis)
   {
       getUrls((char *) data);
   }
    return LXB_STATUS_OK;
}

// Serialize node
void Scraper::serialize_node(lxb_dom_node_t *node)
{
    lxb_status_t status;

    status = lxb_html_serialize_pretty_cb(node, LXB_HTML_SERIALIZE_OPT_UNDEF,
                                          0, serializer_callback, nullptr);
    if (status != LXB_STATUS_OK) {
        exit(EXIT_FAILURE);
    }
}

// Find the right content
// Source 1: http://lexbor.com/docs/lexbor/
// Source 2: https://github.com/lexbor/lexbor/tree/master/examples
// Get elements by attribute: https://github.com/lexbor/lexbor/blob/master/examples/lexbor/html/elements_by_attr.c
std::vector<std::string> Scraper::ParseContent(std::string content, char* attributeName, char* value) {
    lxb_status_t status;
    lxb_dom_element_t* body = nullptr;
    lxb_dom_element_t* gather_collection = nullptr;
    lxb_html_document_t* document = nullptr;
    lxb_dom_collection_t* collection = nullptr;
    lxb_html_parser_t* parser = nullptr;
    std::vector<std::string> output;
    // Initialize parser
    parser = lxb_html_parser_create();
    status = lxb_html_parser_init(parser);

    if (status != LXB_STATUS_OK)
    {
        exit(EXIT_FAILURE);
    }

    // Create an empty array of lxb_char_t
    // memset and then iterate through the content and
    // assign to the html variable
    lxb_char_t *html = new lxb_char_t[content.size() + 1];
//    std::memset(html, 0, content.size() + 1);

    for (int i = 0; i < content.length(); i++) {
        html[i] = content[i];
    }

    size_t html_len = content.length() + 1;
    document = lxb_html_parse(parser, html, html_len);

    if (document == nullptr) {
        exit(EXIT_FAILURE);
    }

    // Destroy parser now that the process has finished
    lxb_html_parser_destroy(parser);

    body = lxb_dom_interface_element(document->body);
    collection = lxb_dom_collection_make(&document->dom_document, 128);

    if (collection == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    // Find elements that contain the href attribute and start with /politics.
    status = lxb_dom_elements_by_attr_begin(body,
                                            collection,
                                            (const lxb_char_t*) attributeName, 4,
                                            (const lxb_char_t*) value, 1,
                                            true
                                            );

    if (status != LXB_STATUS_OK)
    {
        exit(EXIT_FAILURE);
    }

    size_t get_collection_length = lxb_dom_collection_length(collection);

    // Print out results
    for (size_t i = 0; i < get_collection_length; i++)
    {
        //std::cout << i << std::endl;
        gather_collection = lxb_dom_collection_element(collection, i);
        serialize_node(lxb_dom_interface_node(gather_collection));
    }

    output.reserve(urls.size());
    for (std::string& item : urls)
    {
        output.push_back(item);
    }

    // Destroy objects to avoid memory leaks
    lxb_dom_collection_destroy(collection, true);
    lxb_html_document_destroy(document);
    delete [] html;

    return output;
}

void AnalyzePages::analyzeEntry(std::string input, std::vector<std::string> grabKeywords, Scraper scraper)
{
//    Check for connection first
    if (!scraper.CheckForConnection())
    {
        return;
    }

    scraper.analysis = true;
    bool keywordsFound = false;
    cpr::Response getRes = scraper.request_info(input);
    lxb_status_t status;
    lxb_dom_element_t* elem = nullptr;
    lxb_html_document_t* document = nullptr;
    lxb_dom_collection_t* collection = nullptr;
    lxb_dom_node_t* node = nullptr;
    lxb_dom_character_data_t *ch_data = nullptr;

    lxb_char_t *html = new lxb_char_t[getRes.text.size() + 1];
    // Check for string, span and a tags, if a tags are found, then replace them with paragraph tags
    for (int i = 0; i < getRes.text.length();) {
        // This might need a refactor but I am unable to find a way to do so
        std::string keyword1 = "<strong>";
        std::string keyword2 = "</strong>";
        std::string keyword3 = "<span>";
        std::string keyword4 = "</span>";
        std::string keyword5 = "<a ";
        std::string keyword6 = "</a>";
        bool isTheSame = true;

        for (int j = 0; j < keyword1.length(); j++) {
            if (getRes.text[i + j] != keyword1[j]) {
                isTheSame = false;
                break;
            }
        }

        if (isTheSame) {
            i += int(keyword1.length()); // Move past
            continue;
        }else{
            isTheSame = true;
        }

        for (int k = 0; k < keyword2.length(); k++) {
            if (getRes.text[i + k] != keyword2[k]) {
                isTheSame = false;
                break;
            }
        }

        if (isTheSame) {
            i += int(keyword2.length()); // Move past
            continue;
        }else{
            isTheSame = true;
        }

        for (int l = 0; l < keyword3.length(); l++) {
            if (getRes.text[i + l] != keyword3[l]) {
                isTheSame = false;
                break;
            }
        }

        if (isTheSame) {
            i += int(keyword3.length()); // Move past
            continue;
        }else{
            isTheSame = true;
        }

        for (int m = 0; m < keyword4.length(); m++) {
            if (getRes.text[i + m] != keyword4[m]) {
                isTheSame = false;
                break;
            }
        }

        if (isTheSame) {
            i += int(keyword4.length()); // Move past
            continue;
        }else{
            isTheSame = true;
        }

        for (int n = 0; n < keyword5.length(); n++) {
            if (getRes.text[i + n] != keyword5[n]) {
                isTheSame = false;
                break;
            }
        }

        if (isTheSame) {
            html[i] = '<';
            html[i+1] = 'p';
            html[i+2] = ' ';
            i += int(keyword5.length()); // Move past
            continue;
        }else{
            isTheSame = true;
        }


        for (int o = 0; o < keyword6.length(); o++) {
            if (getRes.text[i + o] != keyword6[o]) {
                isTheSame = false;
                break;
            }
        }

        if (isTheSame) {
            html[i] = '<';
            html[i+1] = '/';
            html[i+2] = 'p';
            html[i+3] = '>';
            i += int(keyword6.length()); // Move past
            continue;
        }


        html[i] = getRes.text[i];
        i++;
    }

//    std::cout << html << std::endl;
//     std::cin.get();
    std::string getHTML;

    for (int i = 0; html[i] != '\0'; i++)
    {
        getHTML.push_back(html[i]);
    }

    size_t html_len = getHTML.length() - 1;

    document = scraper.Parse(html, html_len);
    collection = lxb_dom_collection_make(&document->dom_document, 128);

    if (collection == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    // Get status of creating a collection creation for paragraph tags
    status = lxb_dom_elements_by_tag_name(lxb_dom_interface_element(document->body),
                                          collection, (const lxb_char_t*) "p", 1);

    if (status != LXB_STATUS_OK)
    {
        exit(EXIT_FAILURE);
    }

//    this is 0
    std::cout << lxb_dom_collection_length(collection) << std::endl;

    for (size_t i = 0; i < lxb_dom_collection_length(collection); i++)
    {
        // Get the text from any paragraphs
        // Special thanks to Alexander Lexborisov for providing me with the solution
        // to get the text. Source: https://github.com/lexbor/lexbor/issues/196
        elem = lxb_dom_collection_element(collection, i);
        node = lxb_dom_interface_node(elem)->first_child;

        if (node != nullptr && node->local_name == LXB_TAG__TEXT)
        {
            ch_data = lxb_dom_interface_character_data(node);
            const auto getData = ch_data->data.data;

            // To cast an unsigned char* to string, we need to use reinterpret_cast
            // This is not very safe but it works Source:
            // https://stackoverflow.com/questions/17746688/convert-unsigned-char-to-stdstring
            std::string toString(reinterpret_cast<char*>(getData));

            for (std::string& keyword : grabKeywords)
            {
                if (toString.find(keyword) != std::string::npos)
                {
                    keywordsFound = true;
                    break;
                }
            }

            // Output
            if (keywordsFound)
            {
                std::cout << "Found: " << toString << std::endl;
                keywordsFound = false;
            }

        }
    }

    lxb_dom_collection_destroy(collection, true);
    lxb_html_document_destroy(document);
    delete [] html;

    scraper.analysis = false;
}