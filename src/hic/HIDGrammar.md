
This is a rough specification for `Heaven Interface Definition` files:

```
HADFile             := RootObjectDef

RootObjectDef       := Keyword_Ui <identifer> '{' PropertyList '}' ';'
                    := ObjectFwdDef RootObjectDef;

ObjectDef           := ObjectFwdDef
                    := RealObjectDef

RealObjectDef       := ClassName <identifier> '{' PropertyList '}' ';'

ObjectFwdDef        := ClassName <identifier> ';'

PropertyList        := PropertyListEntry
                    := PropertyListEntry PropertyList

PropertyListEntry   := PropertyDef
                    := ObjectDef
                    := ContentList
                    := ContentRef

PropertyDef         := <identifier> PropertyValue ';'

PropertyValue       := Keyword_True
                    := Keyword_False
                    := <identifier>
                    := TranslatedString

TranslatedString    := '"' AnyChars '"'

/* ContentLists are DEPRECATED - See below. */
ContentList         := Keyword_Content '[' ContentRefs ']'

ContentRefs         := ContentRef
                    := ContentRefs ContentRef

ContentRef          := ObjectDef
                    := Separator

ClassName           := Keyword_Action
                    := Keyword_ActionGroup
                    := Keyword_Menu
                    := Keyword_MenuBar
                    := Keyword_ToolBar
                    := Keyword_MergePoint
                    := Keyword_Container
                    := Keyword_DAM

Separator           := Keyword_Separator ';'

Keyword_True        := "true"
Keyword_False       := "false"
Keyword_Content     := "Content"
Keyword_Ui          := "Ui"
Keyword_Action      := "Action"
Keyword_ActionGroup := "ActionGroup"
Keyword_Menu        := "Menu"
Keyword_MenuBar     := "MenuBar"
Keyword_ToolBar     := "ToolBar"
Keyword_Separator   := "Sep"
                    := "Separator"
Keyword_MergePlace  := "MergePlace"
Keyword_Container   := "Container"
Keyword_DAM         := "DynamicActionMerger"

AnyChars            := >> Every Char other than " and ' <<
```

`ContentLists` are a deprecated feature of initial development. The original syntax was:

    Action action1 { Text "txt"; /*...*/ };
    Action action2 { Text "txt"; /*...*/ };
    Menu menu { Content [ action1 action2 ]; };

This constuct can nowadays simply be written in either of these two forms:

    Action action1 { Text "txt"; /*...*/ };
    Action action2 { Text "txt"; /*...*/ };
    Menu menu { action1; action2; };

or even as:

    Menu menu {
        Action action1 { Text "txt"; /*...*/ };
        Action action2 { Text "txt"; /*...*/ };
    };

Actually, the new context object creation also allows for:

    Menu menu {
        Content [
            Action action {
                Text "txt";
            };
        ];
    };

