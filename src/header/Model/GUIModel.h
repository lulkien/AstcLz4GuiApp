#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include <QString>
#include <QList>

#define GUI_MODEL GUIModel::instance()

extern QList<int> GLOBAL_WIDTHS;
extern QList<int> GLOBAL_HEIGHTS;
extern QList<int> GLOBAL_MARGINS;

class GUIModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int      globalWidth     READ globalWidth    WRITE setGlobalWidth    NOTIFY globalWidthChanged)
    Q_PROPERTY(int      globalHeight    READ globalHeight   WRITE setGlobalHeight   NOTIFY globalHeightChanged)
    Q_PROPERTY(int      globalMargins   READ globalMargins  WRITE setGlobalMargins  NOTIFY globalMarginsChanged)
    Q_PROPERTY(QString  globalFont      READ globalFont     WRITE setGlobalFont     NOTIFY globalFontChanged)

public:
    static GUIModel &instance();

    int globalWidth() const;
    void setGlobalWidth(int newGlobalWidth);

    int globalHeight() const;
    void setGlobalHeight(int newGlobalHeight);

    int globalMargins() const;
    void setGlobalMargins(int newGlobalMargins);

    const QString &globalFont() const;
    void setGlobalFont(const QString &newGlobalFont);

    void applySettings();

private:
    GUIModel();

signals:
    void globalWidthChanged();
    void globalHeightChanged();
    void globalMarginsChanged();
    void globalFontChanged();

private:
    int m_globalWidth;
    int m_globalHeight;
    int m_globalMargins;
    QString m_globalFont;
};

#endif // GUIMODEL_H
