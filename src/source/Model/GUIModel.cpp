#include "GUIModel.h"
#include "ApplicationSettings.h"
#include "Common.h"

QList<int> GLOBAL_WIDTHS =  { 640,  800,  960, 1024, 1280 };
QList<int> GLOBAL_HEIGHTS = { 480,  600,  720,  768,  960 };
QList<int> GLOBAL_MARGINS = {  10,   20,   30,   40,   50 };

GUIModel &GUIModel::instance()
{
    static GUIModel _self;
    return _self;
}

int GUIModel::globalWidth() const
{
    return m_globalWidth;
}

void GUIModel::setGlobalWidth(int newGlobalWidth)
{
    if (m_globalWidth == newGlobalWidth)
        return;
    m_globalWidth = newGlobalWidth;
    emit globalWidthChanged();
}

int GUIModel::globalHeight() const
{
    return m_globalHeight;
}

void GUIModel::setGlobalHeight(int newGlobalHeight)
{
    if (m_globalHeight == newGlobalHeight)
        return;
    m_globalHeight = newGlobalHeight;
    emit globalHeightChanged();
}

int GUIModel::globalMargins() const
{
    return m_globalMargins;
}

void GUIModel::setGlobalMargins(int newGlobalMargins)
{
    if (m_globalMargins == newGlobalMargins)
        return;
    m_globalMargins = newGlobalMargins;
    emit globalMarginsChanged();
}

const QString &GUIModel::globalFont() const
{
    return m_globalFont;
}

void GUIModel::setGlobalFont(const QString &newGlobalFont)
{
    if (m_globalFont == newGlobalFont)
        return;
    m_globalFont = newGlobalFont;
    emit globalFontChanged();
}

void GUIModel::applySettings()
{
    INFO << "Apply new settings";
    setGlobalWidth(GLOBAL_WIDTHS.at(SETTINGS.resolutionID()));
    setGlobalHeight(GLOBAL_HEIGHTS.at(SETTINGS.resolutionID()));
    setGlobalMargins(GLOBAL_MARGINS.at(SETTINGS.resolutionID()));
}

GUIModel::GUIModel()
    : m_globalWidth     { 800 } // default value
    , m_globalHeight    { 600 } // default value
    , m_globalMargins   { 20 }  // default value
    , m_globalFont      { "Hack" }
{

}








