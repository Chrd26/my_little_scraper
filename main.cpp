#include "scraping-handler.h"
#include <wx/wx.h>
#include <wx/stattext.h>
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

wxIMPLEMENT_APP(ScraperApp);

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
    ID_Start = 1,
};

//Define logic

bool ScraperApp::OnInit()
{
    MainFrame* frame = new MainFrame();
    // Use Hex to set the background color, read more here:
    // https://docs.wxwidgets.org/3.2/classwx_colour.html#aac96e7922132d672a1f83d59ecf07343
    // Stylizing the frame needs to use wxWindows class members
    // Read: https://docs.wxwidgets.org/stable/classwx_window.html

    // By Chat GPT
    // Read more here:
    // wxSize https://docs.wxwidgets.org/3.2/classwx_size.html#a89bbb1a42ad12573ff42809221e243a7
    // Graphic Device Interface(wxGetDisplaySize() can be found here):
    // https://docs.wxwidgets.org/3.2/group__group__funcmacro__gdi.html
    // SetSize: https://docs.wxwidgets.org/3.2/classwx_window.html
    wxSize screenSize = wxGetDisplaySize();
    wxSize windowSize(screenSize.GetWidth() * 0.9, screenSize.GetHeight() * 0.9);
    frame->SetSize(windowSize);
    frame->SetBackgroundColour(wxColour(0,0,0));
    frame->Center(wxBOTH);
    frame->Show(true);
    return true;
}

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, "Web Scraper")
{
    // Set Font
    // Frame Layout
    wxPanel* top = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
    top->SetBackgroundColour("#4C6E81");
    // This is how I maanged to make font settings to work. Helvetica Neue indeed changes the font
    // Read more here: https://docs.wxwidgets.org/latest/classwx_font_info.html#a7273ff25fbd808e83ee79103d117ecaf
    top->SetFont(wxFontInfo(65).FaceName("Helvetica Neue").Bold());

    wxStaticText* title = new wxStaticText(top, wxID_ANY, "Info Hunter", wxPoint(100,10),
                                           wxDefaultSize, 0, "Info Hunter");
    title->SetForegroundColour("#FFFFFF");

    wxPanel* options = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
    options->SetBackgroundColour("#7AACB3");

    // Options Text
    // Read more on static text:
    // https://docs.wxwidgets.org/3.2/classwx_static_text.html#a9291a72fe2317f4a9e30c6eb7d02e014

    // Set Font size for the options panel
    // Found here: https://stackoverflow.com/questions/67843563/font-size-scaling-problems
    options->SetFont(wxFontInfo(wxSize(30,30)));
    wxSize optionsPanelSize = options->GetSize();


    wxStaticText* searchSettings = new wxStaticText(options, wxID_ANY, "Search Settings" ,
                                                    wxPoint(optionsPanelSize.GetWidth()/2,10),wxDefaultSize,
                                                    0,  "Search Settings");
    searchSettings->SetForegroundColour("#FFFFFF");


    wxStaticText* databaseSettings = new wxStaticText(options, wxID_ANY, "Database Settings" ,
                                                      wxPoint(optionsPanelSize.GetWidth()/2, 110),
                                                      wxDefaultSize, 0,  "Database Settings");
    databaseSettings->SetForegroundColour("#FFFFFF");

    wxStaticText* database = new wxStaticText(options, wxID_ANY, "Database" ,
                                              wxPoint(optionsPanelSize.GetWidth()/2,210),
                                              wxDefaultSize, 0,  "Database");
    database->SetForegroundColour("#FFFFFF");

    wxStaticText* run = new wxStaticText(options, wxID_ANY, "Run" ,
                                         wxPoint(optionsPanelSize.GetWidth()/2,310),
                                         wxDefaultSize, 0,  "Run");

    run->SetForegroundColour("#FFFFFF");

    wxPanel* content = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(200,100));
    content->SetBackgroundColour("#4C6E81");

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(top, 1, wxEXPAND);

    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* optionsSizer = new wxBoxSizer(wxVERTICAL);

    contentSizer->Add(options, 2, wxEXPAND);
    contentSizer->Add(content, 3, wxEXPAND);

    sizer->Add(contentSizer, 9, wxEXPAND);

    this->SetSizerAndFit(sizer);

    // Menu

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

    Bind(wxEVT_MENU, &MainFrame::OnStart, this, ID_Start);
    Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &MainFrame::OnExit, this, wxID_EXIT);
}

AboutWindow::AboutWindow()
:wxFrame(NULL, wxID_ANY, "About")
{
    CreateStatusBar();
    SetStatusText("This is an about window. You will find instructions here.");

    wxSize screen = wxGetDisplaySize();
    wxSize windowSize(screen.GetWidth() * 0.4, screen.GetHeight() * 0.4);
    SetSize(windowSize);
}

void MainFrame::OnExit(wxCommandEvent &event)
{
    close(true);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
    AboutWindow* aboutWindow = new AboutWindow();
    aboutWindow->Center(wxBOTH);
    // Set background color, Source: https://forums.wxwidgets.org/viewtopic.php?t=17706
    aboutWindow->SetBackgroundColour("#AA5377");
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