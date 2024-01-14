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
    bool OnInit() override;
};

wxIMPLEMENT_APP(ScraperApp);

// the public keyword is needed, otherwise the inherited code will be considered as
// private member
class MainFrame : public wxFrame
{

public:
    MainFrame();

// Class Properties
// Inserting properties to event functors is not possible
// Create class properties for easy manipulation
private:
    wxStaticText *searchSettings, *databaseSettings,  *database, *run, *title;
    wxPanel *top, *options, *content;
    wxStaticBitmap *optionsImage;
    wxSize contentPanelSize;
    int currentState;

// Starting Content
private:
    wxStaticText *instructions;

// Search Settings
private:
    // element arrays
    std::array<wxTextCtrl*, 5> urlInput;
    std::array<wxTextCtrl*, 5> keywords1;
    std::array<wxTextCtrl*, 5> keywords2;
    std::array<wxTextCtrl*, 5> keywords3;
    std::array<wxTextCtrl*, 5> keywords4;

    // Buttons
    wxButton *confirmButton;

    // Sizers
    wxSizer *mainSizer;
    wxSizer* firstRow;
    wxSizer* secondRow;
    wxSizer* thirdRow;
    wxSizer* fourthRow;
    wxSizer* fifthRow;
    wxSizer* buttonSizer;

// Database Settings
private:
    wxTextCtrl *username, *password;

// States and IDs
private:
    enum events
    {
        ID_Start = 1,
    };

    enum ElementID
    {
        eID_SearchSettings = 0,
        eID_DatabaseSettings,
        eID_Title,
        eID_Database,
        eID_Run,
        eID_OptionsImage,
        eID_TopPanel,
        eID_OptionsPanel,
        eID_ContentPanel,
        eID_ConfirmButton,
        eID_Instructions,
        eID_Username,
        eID_Password
    };

    enum TextInput
    {
        itID_SearchSettingsURL = 0,
        itID_Keyword1,
        itID_Keyword2,
        itID_Keyword3,
        itID_Keyword4,
    };

    enum states
    {
        ST_Instructions = 0,
        ST_SearchSettings,
        ST_DatabaseSettings,
        ST_Database,
        ST_Run,
        ST_Running
    };

// Window Events
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

//Define
bool ScraperApp::OnInit()
{
    auto* frame = new MainFrame();
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
    wxSize windowSize(screenSize.GetWidth() * 0.98, screenSize.GetHeight() * 0.95);
    frame->SetSize(windowSize);
    frame->SetBackgroundColour(wxColour(0,0,0));
    frame->Center(wxBOTH);
    frame->Show(true);
    return true;
}

MainFrame::MainFrame()
        : wxFrame(nullptr, wxID_ANY, "Info Hunter")
{
    // Starting stae
    currentState = ST_Instructions;

    // Set Font
    // Frame Layout
    top = new wxPanel(this, eID_TopPanel, wxDefaultPosition, wxSize(200,100));
    top->SetBackgroundColour("#4C6E81");

    // This is how I managed to make font settings to work. Helvetica Neue indeed changes the font
    // Read more here: https://docs.wxwidgets.org/latest/classwx_font_info.html#a7273ff25fbd808e83ee79103d117ecaf
    top->SetFont(wxFontInfo(65).FaceName("Helvetica Neue").Bold());
    title = new wxStaticText(top, eID_Title, "Info Hunter", wxPoint(5,10),
                             wxDefaultSize, 0, "Info Hunter");
    title->SetForegroundColour("#FFFFFFBB");
    options = new wxPanel(this, eID_OptionsPanel, wxDefaultPosition, wxDefaultSize);
    options->SetBackgroundColour("#4C6E81");

    // Options Text
    // Read more on static text:
    // https://docs.wxwidgets.org/3.2/classwx_static_text.html#a9291a72fe2317f4a9e30c6eb7d02e014

    // Set Font size for the options panel
    // Found here: https://stackoverflow.com/questions/67843563/font-size-scaling-problems
    options->SetFont(wxFontInfo(55).FaceName("Helvetica Neue"));
    wxSize optionsPanelSize = options->GetSize();

    // This is important to make sure that every supported
    // file format works.
    // Read more here: https://forums.wxwidgets.org/viewtopic.php?t=47660
    wxInitAllImageHandlers();
    optionsImage = new wxStaticBitmap(options, eID_OptionsImage,
                                      wxBitmap("../graphics/Infohunter_Options_Panel_2.png",
                                               wxBITMAP_TYPE_PNG),
                                      wxDefaultPosition, wxDefaultSize);
    searchSettings = new wxStaticText(options, eID_SearchSettings, "Search Settings" ,
                                      wxPoint(optionsPanelSize.GetWidth()/2,40),
                                      wxDefaultSize, 0,  "Search Settings");
    searchSettings->SetForegroundColour("#FFFFFFAA");
    databaseSettings = new wxStaticText(options, eID_DatabaseSettings, "Database Settings" ,
                                        wxPoint(optionsPanelSize.GetWidth()/2, 140),
                                        wxDefaultSize, 0,  "Database Settings");
    databaseSettings->SetForegroundColour("#FFFFFFAA");
    database = new wxStaticText(options, eID_Database, "Database" ,
                                wxPoint(optionsPanelSize.GetWidth()/2,240),
                                wxDefaultSize, 0,  "Database");
    database->SetForegroundColour("#FFFFFFAA");
    run = new wxStaticText(options, eID_Run, "Run" ,
                           wxPoint(optionsPanelSize.GetWidth()/2,340),
                           wxDefaultSize, 0,  "Run");
    run->SetForegroundColour("#FFFFFFAA");

    content = new wxPanel(this, eID_ContentPanel, wxDefaultPosition, wxDefaultSize);
    content->SetBackgroundColour("#4C6E81");
    content->SetFont(wxFontInfo(30).FaceName("Roboto"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(top, 1, wxEXPAND);

    wxBoxSizer* contentSizer = new wxBoxSizer(wxHORIZONTAL);

    contentSizer->Add(options, 1, wxEXPAND);
    contentSizer->Add(content, 5, wxEXPAND);

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

    content->SetFont(wxFontInfo(35).FaceName("Helvetica"));
    content->SetForegroundColour("#FFFFFF");

    instructions = new wxStaticText(content, eID_Instructions,
                                    std::string("How to use the application:\n1. Add urls, keywords and")
                                    + std::string("set up a timer in the search settings\n")
                                    + std::string("2. Connect to a database in in the database settings\n")
                                    + std::string("3. Press run to start scanning\n")
                                    + std::string("4. Press the database option to see the results"),
                                    wxPoint(20, 50), wxDefaultSize, 0, "Instructions");

    // Handle events for certain elements in the window
    // The argument after the handles has to be the id of the element
    // Read more: https://docs.wxwidgets.org/3.2.3/overview_events.html

    // On hover events
    searchSettings->Bind(wxEVT_ENTER_WINDOW,&MainFrame::HoverSearchSettings,
                         this, eID_SearchSettings);
    databaseSettings->Bind(wxEVT_ENTER_WINDOW, &MainFrame::HoverDatabaseSettings,
                           this, eID_DatabaseSettings);
    database->Bind(wxEVT_ENTER_WINDOW, &MainFrame::HoverDatabase, this, eID_Database);
    run->Bind(wxEVT_ENTER_WINDOW, &MainFrame::HoverRun, this, eID_Run);
    // On Exit hover events
    searchSettings->Bind(wxEVT_LEAVE_WINDOW,&MainFrame::StopHoverSearchSettings,
                         this, eID_SearchSettings);
    databaseSettings->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::StopHoverDatabaseSettings,
                           this, eID_DatabaseSettings);
    database->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::StopHoverDatabase, this, eID_Database);
    run->Bind(wxEVT_LEAVE_WINDOW, &MainFrame::StopHoverRun, this, eID_Run);
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
    searchSettings->SetForegroundColour("#FFFFFFFF");
}

void MainFrame::HoverDatabaseSettings(wxMouseEvent &event){
    databaseSettings->SetForegroundColour("#FFFFFFFF");
}

void MainFrame::HoverDatabase(wxMouseEvent &event){
    database->SetForegroundColour("#FFFFFFFF");
}

void MainFrame::HoverRun(wxMouseEvent &event){
    run->SetForegroundColour("#FFFFFFFF");
}

// Stop Hovering Functions
void MainFrame::StopHoverSearchSettings(wxMouseEvent &event){
    searchSettings->SetForegroundColour("#FFFFFFAA");
}

void MainFrame::StopHoverDatabaseSettings(wxMouseEvent &event){
    databaseSettings->SetForegroundColour("#FFFFFFAA");
}

void MainFrame::StopHoverDatabase(wxMouseEvent &event){
    database->SetForegroundColour("#FFFFFFAA");
}

void MainFrame::StopHoverRun(wxMouseEvent &event){
    run->SetForegroundColour("#FFFFFFAA");
}

// Click Events Functions
void MainFrame::PressSearchSettings(wxEvent &event) {
    if (currentState == ST_Instructions) {
        instructions->Destroy();
    }

    // Destroying vector elements:
    /*The error message you are seeing indicates that the elem variable is not a structure or union,
     * but a pointer to a wxTextCtrl object. Therefore, you cannot use the -> operator to access its members.
     * Instead, you should use the * operator to dereference the pointer and access the object’s members/*/

    if (currentState == ST_SearchSettings) {

        for (int i = 0; i < 5; i++) {
            delete (urlInput[i]);
            delete (keywords1[i]);
            delete (keywords2[i]);
            delete (keywords3[i]);
        }

        confirmButton->Destroy();
    }

    if (currentState == ST_DatabaseSettings) {
        username->Destroy();
        password->Destroy();
        confirmButton->Destroy();
    }

    content->SetFont(wxFontInfo(wxDefaultSize).FaceName("Helvetica"));

    this->SetSizeHints(wxDefaultSize, wxDefaultSize);

    mainSizer = new wxBoxSizer(wxVERTICAL);
    firstRow = new wxBoxSizer(wxHORIZONTAL);
    secondRow = new wxBoxSizer(wxHORIZONTAL);
    thirdRow = new wxBoxSizer(wxHORIZONTAL);
    fourthRow = new wxBoxSizer(wxHORIZONTAL);
    fifthRow = new wxBoxSizer(wxHORIZONTAL);

//    Add Elements to the panel
    for (int i = 0; i < 5; i++)
    {
        urlInput[i] = new wxTextCtrl( content, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        keywords1[i] = new wxTextCtrl( content, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        keywords2[i] = new wxTextCtrl( content, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        keywords3[i] = new wxTextCtrl( content, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
        keywords4[i] = new wxTextCtrl( content, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );

        switch (i)
        {
            case 0:
                firstRow->Add( urlInput[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                firstRow->Add( keywords1[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                firstRow->Add( keywords2[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                firstRow->Add( keywords3[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                firstRow->Add( keywords4[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                mainSizer->Add( firstRow, 0, wxEXPAND|wxTOP, 50);
                break;

            case 1:
                secondRow->Add( urlInput[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                secondRow->Add( keywords1[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                secondRow->Add( keywords2[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                secondRow->Add( keywords3[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                secondRow->Add( keywords4[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                mainSizer->Add( secondRow, 0, wxEXPAND);
                break;

            case 2:
                thirdRow->Add( urlInput[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                thirdRow->Add( keywords1[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                thirdRow->Add( keywords2[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                thirdRow->Add( keywords3[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                thirdRow->Add( keywords4[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                mainSizer->Add( thirdRow, 0, wxEXPAND);
                break;

            case 3:
                fourthRow->Add( urlInput[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fourthRow->Add( keywords1[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fourthRow->Add( keywords2[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fourthRow->Add( keywords3[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fourthRow->Add( keywords4[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                mainSizer->Add( fourthRow, 0, wxEXPAND);
                break;

            case 4:
                fifthRow->Add( urlInput[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fifthRow->Add( keywords1[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fifthRow->Add( keywords2[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fifthRow->Add( keywords3[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                fifthRow->Add( keywords4[i], 1, wxEXPAND|wxRIGHT|wxLEFT, 20 );
                mainSizer->Add( fifthRow, 0, wxEXPAND);
                break;
        }
    }

    buttonSizer = new wxBoxSizer( wxVERTICAL );
    confirmButton = new wxButton( content, wxID_ANY, wxT("Confirm"), wxDefaultPosition, wxDefaultSize, 0 );
    buttonSizer->Add( confirmButton, 0, wxALIGN_CENTER|wxALL|wxTOP, 20 );
    mainSizer->Add( buttonSizer, 1, wxEXPAND, 5 );

    content->SetSizer( mainSizer );
    content->Layout();

    currentState = ST_SearchSettings;
}

void MainFrame::PressDatabaseSettings(wxMouseEvent &event)
{
    wxSize panelSize = content->GetSize();
    content->SetFont(wxFontInfo(40).FaceName("Helvetica"));

    if (currentState == ST_Instructions)
    {
        instructions->Destroy();
    }

    if (currentState == ST_DatabaseSettings)
    {
        username->Destroy();
        password->Destroy();
        confirmButton->Destroy();
    }

    if (currentState == ST_SearchSettings)
    {
        for (int i = 0; i < 5; i++)
        {
            delete(urlInput[i]);
            delete(keywords1[i]);
            delete(keywords2[i]);
            delete(keywords3[i]);
        }

        confirmButton->Destroy();
    }

    wxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *contentHolder = new wxBoxSizer(wxVERTICAL);
    wxSizer *usernameSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *passwordSizer = new wxBoxSizer(wxHORIZONTAL);
    wxSizer *confirmButtonSizer = new wxBoxSizer(wxHORIZONTAL);

    username = new wxTextCtrl(content, eID_Username, "username",
                              wxDefaultPosition, wxDefaultSize, 0);

    password = new wxTextCtrl(content, eID_Password, "password",
                              wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);

    confirmButton = new wxButton(content, eID_ConfirmButton, "Confirm",
                                 wxDefaultPosition, wxDefaultSize, 0,
                                 wxDefaultValidator, "Confirm");

    usernameSizer->Add(username, 1, wxEXPAND|wxLEFT|wxRIGHT,
                       panelSize.GetWidth() * 0.2);
    passwordSizer->Add(password, 1, wxEXPAND|wxLEFT|wxRIGHT,
                       panelSize.GetWidth() * 0.2);
    confirmButtonSizer->Add(confirmButton, 1, wxEXPAND);


    contentHolder->Add(usernameSizer, 1, wxEXPAND);
    contentHolder->Add(passwordSizer, 1, wxEXPAND);
    contentHolder->Add(confirmButtonSizer, 2, wxEXPAND|wxSHAPED);

    topSizer->Add(contentHolder, 1, wxEXPAND|wxTOP,
                  panelSize.GetHeight() * 0.08);

    content->SetSizer(topSizer);
    content->Layout();

    currentState = ST_DatabaseSettings;
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