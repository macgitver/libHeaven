
#ifndef BLUESKY_WINDOW_HPP
#define BLUESKY_WINDOW_HPP

#include <QWidget>

#include "libBlueSky/libBlueSkyAPI.hpp"

#include "libBlueSky/Views.hpp"

namespace Heaven {
    class MenuBar;
}

namespace BlueSky {

    class FooterWidget;

    class HEAVEN_BLUESKY_API Window : public QWidget {
        Q_OBJECT
    protected:
        class Data;
        Data* d;
        Window(Data *d);
    public:
        ~Window();

    public:
        static ViewIdentifier idPrimaryWindow();
        static ViewIdentifier idPrimarySplitter();

    public:
        ContainerWidget* rootContainer();
        ViewIdentifier identifier() const;
    };

    class HEAVEN_BLUESKY_API PrimaryWindow : public Window {
        Q_OBJECT
    private:
        class Data;

    public:
        PrimaryWindow();
        ~PrimaryWindow();

    public:
        void setMenuBar(Heaven::MenuBar* menu);
        Heaven::MenuBar* menuBar() const;

    public:
        FooterWidget* footer();

    private:
        Data* d();
        const Data* d() const;
    };

}

#endif
