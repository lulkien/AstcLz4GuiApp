#include "GUIModel.h"

GUIModel &GUIModel::instance()
{
    static GUIModel _self;
    return _self;
}

GUIModel::GUIModel()
    : m_defaultWidth        { 1280 }
    , m_defaultHeight       { 720 }
    , m_globalMargin        { 20 }
    , m_globalButtonSize    { 100 }
    , m_globalBarHeight     { 35 }
    , m_globalFont          { QLatin1String("Ubuntu") }
    , m_invisible           { QLatin1String("Transparent") }
    , m_gray                { QLatin1String("gray") }
    , m_lightgray           { QLatin1String("lightgray") }
    , m_black               { QLatin1String("black") }
    , m_white               { QLatin1String("white") }
    , m_whitesmoke          { QLatin1String("whitesmoke") }
    , m_tomato              { QLatin1String("tomato") }
    , m_snow                { QLatin1String("snow") }
{

}










