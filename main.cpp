#include "scraping-handler.h"
#include <wx/wx.h>
#include "csv_handler.h"

// More info and understanding
// https://brightdata.com/blog/how-tos/web-scraping-in-c-plus-plus
// preallocate the vector with the size of arguments.
// It is not worth preallocating when the vector is reallocated
// Read more here:
// https://stackoverflow.com/questions/11888265/is-it-more-efficient-to-preallocate-a-vector

// Wx Widgets Initialization
// Source: https://docs.wxwidgets.org/3.2/overview_helloworld.html
class ScraperApp : public wxApp
{
public:
    virtual bool OnInit();
};

// the public keyword is needed, otherwise the inherited code will be considered as
// private member
class MainFrame : public wxFrame
{
public:
    MainFrame();

private:
void OnExit(wxCommandEvent& event);
void OnAbout(wxCommandEvent& event);
void OnStart(wxCommandEvent& event);
};

class AboutWindow: public wxFrame
{
public:
    AboutWindow();
private:
    void OnExit(wxCommandEvent& event);
};

enum events
{
    ID_Start = 1
};

wxIMPLEMENT_APP(ScraperApp);

bool ScraperApp::OnInit()
{
    MainFrame* frame = new MainFrame();
    frame->Show(true);
    return true;
}

MainFrame::MainFrame()
    : wxFrame(NULL, wxID_ANY, "Web Scraper")
{
    wxMenu* menuFile = new wxMenu;

    menuFile->Append(ID_Start, "Start", "It starts the application");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT);

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);

    wxMenuBar* menubar = new wxMenuBar;
    menubar->Append(menuFile, "&File");
    menubar->Append(menuHelp, "&Help");

    SetMenuBar( menubar );

    CreateStatusBar();
    SetStatusText("Hello");

    Bind(wxEVT_MENU, &MainFrame::OnStart, this, ID_Start);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

AboutWindow::AboutWindow()
:wxFrame(NULL, wxID_ANY, "About")
{
    CreateStatusBar();
    SetStatusText("This is an about window. You will find instructions here.");
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
    AboutWindow* aboutWindow = new AboutWindow();
    aboutWindow->Show(true);
}

void MainFrame::OnStart(wxCommandEvent &event)
{
    wxMessageBox("The app should start running now!", "", wxOK|wxICON_INFORMATION);
}

void AboutWindow::OnExit(wxCommandEvent &event)
{
    close(true);
}

/*
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

    for (std::string& url : csv.links) {
        // Get info from website
        scraper.baseURL = url;
        cpr::Response r = scraper.request_info(scraper.baseURL);

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
*/