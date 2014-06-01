
#include <QDebug>
#include <QWidget>
#include <QLayout>
#include <QXmlStreamWriter>

namespace BlueSky
{

    void dumpWidget(QWidget* w, QXmlStreamWriter& writer);

    QString toStr(const QRect& r)
    {
        return QString(QLatin1String("(%1:%2)-(%3:%4) %5x%6"))
                .arg(r.left())
                .arg(r.top())
                .arg(r.right())
                .arg(r.bottom())
                .arg(r.width())
                .arg(r.height());
    }

    QString toStr(const QSize& s)
    {
        return QString(QLatin1String("%1x%2"))
                .arg(s.width())
                .arg(s.height());
    }

    void dumpLayout(QLayout* l, QXmlStreamWriter& writer, QSet<QWidget*>& widgets)
    {
        writer.writeStartElement(QLatin1String("Layout"));
        writer.writeAttribute(QLatin1String("Class"), QString::fromLatin1(l->metaObject()->className()));
        writer.writeAttribute(QLatin1String("OName"), l->objectName());

        writer.writeStartElement(QLatin1String("Min"));
        writer.writeAttribute(QLatin1String("Size"), toStr(l->minimumSize()));
        writer.writeEndElement();
        writer.writeStartElement(QLatin1String("Max"));
        writer.writeAttribute(QLatin1String("Size"), toStr(l->maximumSize()));
        writer.writeEndElement();
        writer.writeStartElement(QLatin1String("Hint"));
        writer.writeAttribute(QLatin1String("Size"), toStr(l->sizeHint()));
        writer.writeEndElement();


        for (int i = 0; i < l->count(); i++)
        {
            QLayoutItem* it = l->itemAt(i);
            if (it) {
                if (it->widget()) {
                    QWidget* w = it->widget();
                    widgets.insert(w);
                    dumpWidget(w, writer);
                }
                else if (it->layout()) {
                    QLayout* l2 = it->layout();
                    dumpLayout(l2, writer, widgets);
                }
                else {
                    writer.writeEmptyElement(QLatin1String("Spacer"));
                }
            }
        }

        writer.writeEndElement();
    }

    void dumpWidget(QWidget* w, QXmlStreamWriter& writer)
    {
        writer.writeStartElement(QLatin1String("Widget"));
        writer.writeAttribute(QLatin1String("Class"), QString::fromLatin1(w->metaObject()->className()));
        writer.writeAttribute(QLatin1String("OName"), w->objectName());
        writer.writeAttribute(QLatin1String("Visible"), w->isVisible() ? QLatin1String("Yes") : QLatin1String("No"));
        writer.writeAttribute(QLatin1String("Geometry"), toStr(w->geometry()));

        writer.writeStartElement(QLatin1String("Min"));
        writer.writeAttribute(QLatin1String("Size"), toStr(w->minimumSize()));
        writer.writeEndElement();
        writer.writeStartElement(QLatin1String("Max"));
        writer.writeAttribute(QLatin1String("Size"), toStr(w->maximumSize()));
        writer.writeEndElement();
        writer.writeStartElement(QLatin1String("MinHint"));
        writer.writeAttribute(QLatin1String("Size"), toStr(w->minimumSizeHint()));
        writer.writeEndElement();
        writer.writeStartElement(QLatin1String("Hint"));
        writer.writeAttribute(QLatin1String("Size"), toStr(w->sizeHint()));
        writer.writeEndElement();


        QSet<QWidget*> widgets;

        QLayout* l = w->layout();
        if (l) {
            dumpLayout(l, writer, widgets);
        }

        foreach(QObject* sub, w->children()) {
            if (sub && sub->isWidgetType()) {
                QWidget* subW = static_cast<QWidget*>(sub);
                if (!widgets.contains(subW)) {
                    dumpWidget(subW, writer);
                }
            }
        }

        writer.writeEndElement();
    }

    void dumpWidgetHierarchy(QWidget* base)
    {
        QString s;
        QXmlStreamWriter writer(&s);
        writer.setAutoFormattingIndent(4);
        writer.setAutoFormatting(true);

        writer.writeStartDocument();
        writer.writeStartElement(QLatin1String("Dump"));

        dumpWidget(base, writer);

        writer.writeEndElement();
        writer.writeEndDocument();

        qDebug() << s;
    }

}
