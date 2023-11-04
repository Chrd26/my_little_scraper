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

// Class Properties
private:
    wxStaticText* searchSettings;

// State manager and IDs
private:
    enum optionsHoverState
    {
        noHover = -1,
        searchSettingsHover,
        databaseSettingsHover,
        databaseHover,
        runHover
    };
    enum events
    {
        ID_Start = 1,
        ID_SearchSettings,
        ID_DataBaseSettings,
        ID_Database,
        ID_Run
    };

    enum ElementID
    {
        eID_SearchSettings = 0,
        eID_DatabaseSettings,
        eID_Database,
        eID_Run
    };

// State Values
private:
    int currentHoverState, currentButtonState;

// Events Handling
private:
void OnExit(wxCommandEvent& event);
void OnAbout(wxCommandEvent& event);
void OnStart(wxCommandEvent& event);

// Hover Events
private:
    void HoverSearchSettings(wxMouseEvent& event);
    void HoverDatabaseSettings(wxMouseEvent& event);
    void HoverDatabase(wxMouseEvent& event);
    void HoverRun(wxMouseEvent& event);

// Stop Hovering Event
private:
    void StopHoverSearchSettings(wxMouseEvent& event);
    void StopHoverDatabaseSettings(wxMouseEvent& event);
    void StopHoverDatabase(wxMouseEvent& event);
    void StopHoverRun(wxMouseEvent& event);

// Click Button Events
private:
void PressSearchSettings(wxEvent &event);
void PressDatabaseSettings(wxMouseEvent& event);
void PressDatabase(wxMouseEvent& event);
void PressRun(wxMouseEvent& event);



// Bools
private:
    bool isMouseHovering = false;
};

class AboutWindow: public wxFrame
{
public:
    AboutWindow();
private:
    void OnExit(wxCommandEvent& event);
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
    // This is how I managed to make font settings to work. Helvetica Neue indeed changes the font
    // Read more here: https://docs.wxwidgets.org/latest/classwx_font_info.html#a7273ff25fbd808e83ee79103d117ecaf
    top->SetFont(wxFontInfo(65).FaceName("Helvetica Neue").Bold());

    wxStaticText* title = new wxStaticText(top, wxID_ANY, "Info Hunter", wxPoint(100,10),
                                           wxDefaultSize, 0, "Info Hunter");
    title->SetForegroundColour("#FFFFFF");

    wxPanel* options = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    options->SetBackgroundColour("#7AACB3");

    // Options Text
    // Read more on static text:
    // https://docs.wxwidgets.org/3.2/classwx_static_text.html#a9291a72fe2317f4a9e30c6eb7d02e014

    // Set Font size for the options panel
    // Found here: https://stackoverflow.com/questions/67843563/font-size-scaling-problems
    options->SetFont(wxFontInfo(55).FaceName("Helvetica Neue"));
    wxSize optionsPanelSize = options->GetSize();

    searchSettings = new wxStaticText(options, eID_SearchSettings, "Search Settings" ,
                                                    wxPoint(optionsPanelSize.GetWidth()/2,60),
                                                   wxDefaultSize, 0,  "Search Settings");

    searchSettings->SetForegroundColour("#FFFFFF10");

    wxStaticText* databaseSettings = new wxStaticText(options, eID_DatabaseSettings, "Database Settings" ,
                                                      wxPoint(optionsPanelSize.GetWidth()/2, 180),
                                                      wxDefaultSize, 0,  "Database Settings");
    databaseSettings->SetForegroundColour("#FFFFFF");

    wxStaticText* database = new wxStaticText(options, eID_Database, "Database" ,
                                              wxPoint(optionsPanelSize.GetWidth()/2,300),
                                              wxDefaultSize, 0,  "Database");
    database->SetForegroundColour("#FFFFFF");

    wxStaticText* run = new wxStaticText(options, eID_Run, "Run" ,
                                         wxPoint(optionsPanelSize.GetWidth()/2,420),
                                         wxDefaultSize, 0,  "Run");
    run->SetForegroundColour("#FFFFFF");

    wxPanel* content = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    content->SetBackgroundColour("#4C6E81");
    content->SetFont(wxFontInfo(30).FaceName("Roboto"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(top, 1, wxEXPAND);

    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

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

    // Handle events for certain elements in the window
    // Read more: https://docs.wxwidgets.org/3.2.3/overview_events.html

    // On hover events
    searchSettings->Bind(wxEVT_ENTER_WINDOW,&MainFrame::HoverSearchSettings,
                         this, eID_SearchSettings);
    databaseSettings->Bind(wxEVT_ENTER_WINDOW, &MainFrame::HoverDatabaseSettings,
                           this, eID_DatabaseSettings);
    database->Bind(wxEVT_ENTER_WINDOW, &MainFrame::HoverDatabase, this, eID_Database);
    database->Bind(wxEVT_ENTER_WINDOW, &MainFrame::HoverRun, this, eID_Run);

    // On Exit hover events
    searchSettings->Bind(wxEVT_LEAVE_WINDOW,&MainFrame::StopHoverSearchSettings,
                         this, eID_SearchSettings);
    databaseSettings->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::StopHoverDatabaseSettings,
                           this, eID_DatabaseSettings);
    database->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::StopHoverDatabase, this, eID_Database);
    database->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::StopHoverRun, this, eID_Run);


    // On click events
    searchSettings->Bind(wxEVT_LEFT_UP, &MainFrame::PressSearchSettings,
                        this, eID_SearchSettings);
    databaseSettings->Bind(wxEVT_LEFT_UP, &MainFrame::PressDatabaseSettings,
                           this, eID_DatabaseSettings);
    database->Bind(wxEVT_LEFT_UP, &MainFrame::PressDatabase, this,
                            eID_Database);
    run->Bind(wxEVT_LEFT_UP, &MainFrame::PressRun, this, eID_Run);
}

// Hover Events Functions
void MainFrame::HoverSearchSettings(wxMouseEvent &event){
    currentHoverState = searchSettingsHover;
    std::cout << "Hover Search Settings" << std::endl;
    searchSettings->SetForegroundColour("#FFFFFF");
}

void MainFrame::HoverDatabaseSettings(wxMouseEvent &event){
    currentHoverState = databaseSettingsHover;
    std::cout << "Hover Database Settings" << std::endl;
}

void MainFrame::HoverDatabase(wxMouseEvent &event){
    currentHoverState = databaseHover;
    std::cout << "Hover Database" << std::endl;
}

void MainFrame::HoverRun(wxMouseEvent &event){
    currentHoverState = runHover;
    std::cout << "Hover Run" << std::endl;
}

// Stop Hovering Functions
void MainFrame::StopHoverSearchSettings(wxMouseEvent &event){
    currentHoverState = noHover;
    std::cout << "Stop Hover Search Settings" << std::endl;
    searchSettings->SetForegroundColour("#FFFFFF10");
}

void MainFrame::StopHoverDatabaseSettings(wxMouseEvent &event){
    currentHoverState = noHover;
    std::cout << "Stop Hover Database Settings" << std::endl;
}

void MainFrame::StopHoverDatabase(wxMouseEvent &event){
    currentHoverState = noHover;
    std::cout << "Stop Hover Database" << std::endl;
}

void MainFrame::StopHoverRun(wxMouseEvent &event){
    currentHoverState = noHover;
    std::cout << "Stop Hover Run" << std::endl;
}

// Click Events Functions
void MainFrame::PressSearchSettings(wxEvent &event)
{
   std::cout << "Pressed Search Settings" << std::endl;
}

void MainFrame::PressDatabaseSettings(wxMouseEvent &event)
{
    std::cout << "Pressed Database Settings" << std::endl;
}

void MainFrame::PressDatabase(wxMouseEvent &event)
{
    std::cout << "Pressed Database" << std::endl;
}

void MainFrame::PressRun(wxMouseEvent &event)
{
    std::cout << "Pressed Run" << std::endl;
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