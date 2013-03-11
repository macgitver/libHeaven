# Heaven Interface Definitions                        {#HIDFile}

A Heaven Interface Definition is declarative approach for creating Qt Widget
based menu and action classes.

A simple HID might look like:

    Ui MainWindowActions {
        Action Quit {
            Text "&Quit";
            ConnectTo onQuit();
        };
        Menu MainFile {
            Text "&File";
            Action Quit;
        };
        MenuBar MainBar {
            Menu MainFile;
        };
    };

It will:
-   Create a user interface named `MainWindowActions`.

-   Inside this user interface it will create:
    -   the action `Quit` 
    -   the menu `MainFile` containing the action `Quit`
    -   the menu bar `MainBar` containing the menu `MainFile`

-   The `Quit` action's `triggered()` signal will be connected to the owner's
    `onQuit()` slot.

## Let's put some focus on the syntax
In the above example, `Ui`, `Action`, `Menu` and `MenuBar` are keywords. Blocks
are enclosed in curly braces and terminated with a semicolon.

Each keyword denotes either the creation of a new object or a reference to an
existing object, depending soley on the existance of that object. So, what the above
example does, is:

-   Create a Ui object named _MainWindowActions_.

-   Create an Action object named _Quit_ owned by the _MainWindowActions_ Ui object.
    -   Set the property _Text_ to "&Quit" => Puting a string value inside quotes means
        that we're dealing with a _translatable text_.
    -   Set the property _ConnectTo to onQuit(). => Omitting the quotes means that the
        text won't be translated.

-   Create a Menu object named _MainFile_ owned by the _MainWindowActions_ Ui object.
    -   Set the property _Text_ to "&File"
    -   Add a reference to the _Quit_ object.

-   Create a MenuBar object named _MainBar_ owned by the _MainWindowActions_ Ui object.
    -   Add a reference to the _MainFile_ object.

Any created object is owned by the Ui object surrounding it. Therefore, the above
example could as well be rewritten as:

    Ui MainWindowActions {
        MenuBar MainBar {
            Menu MainFile {
                Text "&File";
                Action Quit {
                    Text "&Quit";
                    ConnectTo onQuit();
                };
            };
       };
    };

Which is literally the same. However, it is a good practice _NOT_ to nest object
creation.

## Compiling a HID

As mentioned initially, a HID file has to be compiled into (further compilable)
C++ code. This mechanism is very similar to Qt's form generation.

Passing the above snippet through the interface compiler (hic) will produce
a class looking like this:

    class MainWindowActions {
    public:
        Heaven::Action* actQuit;
        Heaven::Menu* menuMainFile;
        Heaven::MenuBar* mbMainBar;

        void setupActions( QObject* parent );
    };


## Using the compiled Heaven Interface Definition
Invoking `setupActions()` will fill all the members of that class with valid
object pointers. The created objects all inherit `QObject` and are reparented
to the given `parent`. That means they will be destroyed automatically with
their parent.

This class can be used in the same ways as a `uic` generated form is used.
The most simple solution is to use multiple inheritance, second best would
be aggregation. Here we will use multiple inheritance.

    class MainWindow : public QMainWindow, private MainWindowActions {
    public:
        MainWindow();
    };

    MainWindow::MainWindow() {
        setupActions( this );
        setMenuBar( mbMainBar->menuBarFor( this ) );
    }

In most desktop environemnts, the menu bar usually belongs to the main window
of application. Note that none of the objects stored inside the generated class
are plain Qt types. They are rather Heaven specific types.

In Heaven any of these _interaction objects_ can create as many GUI objects as is
necessary. Heaven will synchronize them as is required.

If you wanted to create a second window using the same menu bar, you could simply
write:

    void MainWindow::createSecondaryWindow()
    {
        QWidget* w = new MySecondaryWindow();
        w->setMenuBar( mbMainBar->menuBarFor( w ) );
    }

Here, Heaven will create a `QMenuBar` object that is suitable to be used with the
newly created `QWidget`.

While this might not be very intersting for menu bars, it becomes intersting as
soon as you think for: submenus which might appear as part of a menu in the menu
bar and at the same time might appear as part of a context menu. Heaven will handle
this behind the scenes for you.

## The art of merging

Most of the above is straight forward - Just another way to express the creation of
interaction objects. Now, let's see what Heaven can do for us to allow for more
modularity of our code.

Let's create a _file_ menu:

    Ui MainWindowActions {
        /* Define our actions */
        Action FileNew   { Text "&New";     _ConnectTo onFileNew();  };
        Action FileOpen  { Text "&Open";    _ConnectTo onFileOpen(); };
        Action FileQuit  { Text "&Quit";    _ConnectTo onFileQuit(); };

        /* Define the File->Export menu */
        Menu FileExport {
            Text "&Export;
            MergePlace FileExportPlace;
        };

        /* Define the File menu */
        Menu MainFile {
            Text "&File";
            Action FileNew;
            Action FileOpen;
            Separator;
            Menu FileExport;
            Separator;
            Action FileQuit;
        };

        // Define our menu bar
        MenuBar MainBar {
            Menu MainFile;
        };
    };

Compared to the previous example, this one contains 4 new elements:

-   HID files can be amended by using C++ style comments. Either `/* multi line */` or
    single line comments: `// like this`.

-   The _Separator_ keyword inserts a horizontal line into the menu. Strictly speaking, this
    is just another (unnamed) object, which is implicitly generated and referenced here.

-   In the menu object `MainFile` we refer to another menu object. This will simply
    create a submenu.

-   The menu object `FileExport` uses the keyword MergePlace with an object name assigned.
    This actually creates a so called _merge place object_. We don't need to set any
    properties on that object, because merge place objects don't have any. Thus, we don't
    need to open up a block for it (Like we used for i.e. action objects).

Usually, Heaven tries to be smart and most of the time succeeds in trying to be. If we were
to show the menu bar from the above example, Heaven will determine that the _FileExport_ menu
has no content and will implicitly remove it from the MainFile menu object where we
reference it. It will further see that there are 2 Separtors in a row, meaning: Of of them
will implicitly removed.

Now, let's assume we want to create a PDF Export plugin to that application. The usual
approach would be

-   to determine the correct position in the file menu.

-   see if another plugin has already created an "export menu" - and if not, create one.

-   now that we can be sure there is a export menu, append our "PDF" action to that menu

We would repeat that for whatever other places like context-menus we wanted to show the
action... This is much dumb code.

The Heaven approach is a bit different:
We simply create another HID:

    Ui FileExportPdfPluginActions {
        Action FileExportPDF {
            Text "&PDF...";
        };

        Container MergeToMainMenu {
            Action FileExportPDF;
        };
    };

Here, we have yet another new keyword _Container_, which creates objects of type "Container".

A container object is simply an aggregation of object references without any special meaning
or appearance. But we can use those containers from our C++ code:

    class FileExportPdfPlugin : public Plugin, private FileExportPdfPluginActions {
    public:
        FileExportPdfPlugin();
    };

    FileExportPdfPlugin::FileExportPdfPlugin() {
        setupActions( this );
        acMergeToMainMenu->mergeInto( "FileExportPlace" );
    };

Most of this code is the same as we wrote for the MainWindow. The only difference here is that we
use the container and tell it where it belongs.

Heaven will take the contents of the _MergeToMainMenu_ container and substitute all occurences of
merge place objects named `FileExportPlace` with the container's content.

Now, our menu "FileExport" can no longer be considered empty. It will be added where it is
referenced. In our example that would be the "MainFile" menu object. Following that, the two
separator object references are no longer side by side. Heaven will now place one of that
separators before and one after the "FileExport" menu.

