# Heaven        {#mainpage}

Heaven is a tool library enabling you to write modularized software with
consistent graphical user interface. It glues all the required match sticks
together for you almost transparently behind your back.

Basically, Heaven consists of 2 utilities:

-   __An action framework__

    An application typically has _one_ menu bar and several tool bars. In an
    monolithic application there's nothing wrong with that. In a modularized
    application, however, this might lead to hard to implement _Lego_.

    Imagine a word processor application. There will probably be _File_, _Edit_,
    _View_ and _Tools_ menus. But now you navigate your edit cursor into a table.
    Suddenly there is a requirement to insert a _Table_ menu, say between the
    _View_- and _Tools_-Menu. Heaven will allow you to do that, with exactly
    one additional line of code.

    Heaven defines a so called "Heaven Interface Definition" (HID) language.
    
    The syntax of a HID file is strongly oriented towards C++ but is actually
    kind of declarative. HID files are read and _compiled_ by the so called
    __Heaven Interface Compiler__ (hic). Hic produces a C++-Header- and a
    C++-Source-File. Inside it, a class is generated which will setup all
    required `QAction`, `QMenuBar`, `QMenu`, `QToolBar` etc. objects for you.
    This is of code. In past development projects I have often seen such code
    scattered around in several places.

    Back in 2008 I wrote a completely XML based solution of this concept. As I
    must sadly admit today, I gave copyright on that to a four letter company
    (for charity). But I shall not regret it: I've got a lot of lessons learnt
    from that implementation, which are now all incorporated into this brand new
    implementation.

-   __An MDI framework__

    Frankly, this part is more experimental than anything else ...

