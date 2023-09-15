#include "scraping-handler.h"

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

lxb_status_t Scraper::serializer_callback(const lxb_char_t *data, size_t len, void *ctx)
{
    printf("%.*s", (int) len, (const char *) data);

    return LXB_STATUS_OK;
}

void Scraper::serialize_node(lxb_dom_node_t *node)
{
    lxb_status_t status;

    status = lxb_html_serialize_pretty_cb(node, LXB_HTML_SERIALIZE_OPT_UNDEF,
                                          0, serializer_callback, NULL);
    if (status != LXB_STATUS_OK) {
        exit(EXIT_FAILURE);
    }
}

// Find the right content
// Source 1: http://lexbor.com/docs/lexbor/
// Source 2: https://github.com/lexbor/lexbor/tree/master/examples
void Scraper::ParseContent(std::string content)
{
    lxb_status_t status;
    lxb_dom_element_t*  elem = nullptr;
    const lxb_char_t* tagName = nullptr;
    lxb_html_document_t* document = nullptr;
    lxb_dom_collection_t* collection = nullptr;

    // Create an empty array of lxb_char_t
    // memset and then iterate through the content and
    // assign to the html variable

    lxb_char_t html[content.length() + 1];
    std::memset(html, 0, content.length() + 1);

    for (int i = 0; i < content.length(); i++)
    {
        html[i] = content[i];
    }

    size_t html_len = sizeof(html) - 1;

    document = lxb_html_document_create();
    if (document == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    status = lxb_html_document_parse(document, html, html_len);
    if (status != LXB_STATUS_OK)
    {
        exit(EXIT_FAILURE);
    }
    tagName = lxb_dom_element_qualified_name(lxb_dom_interface_element(document->body), nullptr);

    collection = lxb_dom_collection_make(&document->dom_document, 128);

    if (collection == nullptr)
    {
        exit(EXIT_FAILURE);
    }

    status = lxb_dom_elements_by_tag_name(lxb_dom_interface_element(document->body),
                                          collection, (const lxb_char_t*) "div", 3);

    if (status != LXB_STATUS_OK) {
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < lxb_dom_collection_length(collection); i++)
    {
        elem = lxb_dom_collection_element(collection, i);
        serialize_node(lxb_dom_interface_node(elem));
    }
    std::cin.get();

    // Make sure to destroy all the objects when finished
    lxb_html_document_destroy(document);
    lxb_dom_collection_destroy(collection, true);
}