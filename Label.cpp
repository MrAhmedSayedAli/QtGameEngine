#include "Label.h"
#include <QFont>
#include <QDebug>

Label::Label():
    font_("Tahoma"),
    fontSize_(12),
    width_(400),
    text_("some COOL text"),
    fontColor_(Qt::blue)
{
    draw_();
}

QGraphicsItem *Label::getGraphicsItem()
{
    return this;
}

/// Set the font size of the Label.
void Label::setFontSize(int fontSize)
{
    fontSize_ = fontSize;
    draw_();
}

/// Sets the font color of the Label.
void Label::setFontColor(const QColor &fontColor)
{
    fontColor_ = fontColor;
    draw_();
}

/// Sets the width of the Label. Text that surpasses the width of the Label will
/// go down one line.
void Label::setWidth(double width)
{
    width_ = width;
    draw_();
}

/// Sets the text of the Label.
void Label::setText(const std::string &text)
{
    text_ = text;
    draw_();
}

void Label::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hovered();
}

void Label::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit unhovered();
}

void Label::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(this);
}

/// Draws the Label in its current state (current font, width, etc...).
void Label::draw_()
{
    // set text
    setPlainText(QString::fromStdString(text_));

    // set the font (font, size, color)
    QFont font(font_.c_str(),fontSize_);
    setFont(font);
    setDefaultTextColor(fontColor_);

    // set the width
    setTextWidth(width_);
}