#ifndef GUIMODEL_H
#define GUIMODEL_H

#include <QObject>
#include <QString>

#define GUI_MODEL GUIModel::instance()

class GUIModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int      windowWidth     READ windowWidth    WRITE setWindowWidth    NOTIFY windowWidthChanged)
    Q_PROPERTY(int      windowHeight    READ windowHeight   WRITE setWindowHeight   NOTIFY windowHeightChanged)
    Q_PROPERTY(int      globalMargins   READ globalMargins  WRITE setGlobalMargins  NOTIFY globalMarginsChanged)
    Q_PROPERTY(QString  globalFont      READ globalFont     WRITE setGlobalFont     NOTIFY globalFontChanged)

public:
    static GUIModel &instance();
    int windowWidth() const;
    void setWindowWidth(int newWindowWidth);
    int windowHeight() const;
    void setWindowHeight(int newWindowHeight);

    int globalMargins() const;
    void setGlobalMargins(int newGlobalMargins);

    const QString &globalFont() const;
    void setGlobalFont(const QString &newGlobalFont);

private:
    GUIModel();

signals:
    void windowWidthChanged();
    void windowHeightChanged();

    void globalMarginsChanged();

    void globalFontChanged();

private:
    int m_windowWidth;
    int m_windowHeight;
    int m_globalMargins;
    QString m_globalFont;
};

#endif // GUIMODEL_H
