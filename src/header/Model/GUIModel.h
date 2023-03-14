#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include <QString>
#include <QList>

#define GUI_MODEL GUIModel::instance()

class GUIModel : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(GUIModel)
    Q_PROPERTY(int      defaultWidth        READ defaultWidth       CONSTANT)
    Q_PROPERTY(int      defaultHeight       READ defaultHeight      CONSTANT)
    Q_PROPERTY(int      globalMargins       READ globalMargins      CONSTANT)
    Q_PROPERTY(int      globalButtonSize    READ globalButtonSize   CONSTANT)
    Q_PROPERTY(int      globalBarHeight     READ globalBarHeight    CONSTANT)
    Q_PROPERTY(QString  globalFont          READ globalFont         CONSTANT)

    // color
    Q_PROPERTY(QString  invisible           READ invisible          CONSTANT)
    Q_PROPERTY(QString  gray                READ gray               CONSTANT)
    Q_PROPERTY(QString  lightgray           READ lightgray          CONSTANT)
    Q_PROPERTY(QString  black               READ black              CONSTANT)
    Q_PROPERTY(QString  white               READ white              CONSTANT)
    Q_PROPERTY(QString  whitesmoke          READ whitesmoke         CONSTANT)
//    Q_PROPERTY(QString tomato READ tomato)

public:
    static GUIModel &instance();

    const int &defaultWidth() const         { return m_defaultWidth; }
    const int &defaultHeight() const        { return m_defaultHeight; }
    const int &globalMargins() const        { return m_globalMargin; }
    const int &globalButtonSize() const     { return m_globalButtonSize; }
    const int &globalBarHeight() const      { return m_globalBarHeight; }
    const QString &globalFont() const       { return m_globalFont; }

    // color
    const QString &invisible() const        { return m_invisible; }
    const QString &gray() const             { return m_gray; }
    const QString &lightgray() const        { return m_lightgray; }
    const QString &black() const            { return m_black; }
    const QString &white() const            { return m_white; }
    const QString &whitesmoke() const       { return m_whitesmoke; }

private:
    GUIModel();

signals:
    void globalFontChanged();

private:
    const int       m_defaultWidth;
    const int       m_defaultHeight;
    const int       m_globalMargin;
    const int       m_globalButtonSize;
    const int       m_globalBarHeight;
    const QString   m_globalFont;
    const QString   m_invisible;
    const QString   m_gray;
    const QString   m_lightgray;
    const QString   m_black;
    const QString   m_white;
    const QString   m_whitesmoke;
};

#endif // GUIMODEL_H
