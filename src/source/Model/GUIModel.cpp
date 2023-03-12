#include "GUIModel.h"



GUIModel &GUIModel::instance()
{
    static GUIModel _self;
    return _self;
}

int GUIModel::windowWidth() const
{
    return m_windowWidth;
}

void GUIModel::setWindowWidth(int newWindowWidth)
{
    if (m_windowWidth == newWindowWidth)
        return;
    m_windowWidth = newWindowWidth;
    emit windowWidthChanged();
}

int GUIModel::windowHeight() const
{
    return m_windowHeight;
}

void GUIModel::setWindowHeight(int newWindowHeight)
{
    if (m_windowHeight == newWindowHeight)
        return;
    m_windowHeight = newWindowHeight;
    emit windowHeightChanged();
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

GUIModel::GUIModel()
    : m_globalFont { "Hack" }
{

}
