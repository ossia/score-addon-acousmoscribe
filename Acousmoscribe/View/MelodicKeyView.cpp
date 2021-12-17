#include "MelodicKeyView.hpp"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>

#include <Acousmoscribe/Model/MelodicKey.hpp>
#include <Acousmoscribe/Presenter/Presenter.hpp>
#include <Acousmoscribe/View/Utils.hpp>

namespace Acousmoscribe
{

MelodicKeyView::MelodicKeyView(const MelodicKey& mk, Presenter& p, View* parent)
    : QGraphicsItem{parent}
    , melodicKey{mk}
    , m_presenter{p}
    , m_action{None}
{
  this->setFlag(QGraphicsItem::ItemIsSelectable, true);
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
  this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  this->setAcceptHoverEvents(true);
}

void MelodicKeyView::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  const auto [x0, y0, w, h] { baseItemRect(m_width, m_height) };

  Pitch pitch = melodicKey.pitch();
  Range range = melodicKey.range();

  QPen pen;
  pen.setWidth(2);
  pen.setColor(drawColor);
  pen.setStyle(Qt::SolidLine);
  painter->setPen(pen);

  /* Background Rect */

  painter->setBrush(fillColor);
  //painter->drawRect(x0, y0, w, h);

  float h_pitch = h*4/5;
  float x_pitch = w * 1/4;
  float y_pitch;
  for (int i = 1; i <= 7; i++)
  {
    y_pitch = i * h_pitch / 7;
    if (i == 4)
    {
      pen.setWidth(3); // the 4th point is bigger
      painter->setPen(pen);
      painter->drawPoint(QPoint(x_pitch, y_pitch));
      pen.setWidth(2);
      painter->setPen(pen);
    }
    else{
     painter->drawPoint(QPoint(x_pitch, y_pitch));
    }
  }

  switch(range)
  {
    case weak:
      pen.setStyle(Qt::DotLine);
      break;

    case normal:
      pen.setStyle(Qt::DashDotLine);
      break;

    case strong:
      pen.setStyle(Qt::SolidLine);
      break;

    default:
      pen.setStyle(Qt::SolidLine);
      break;
  }

  painter->setPen(pen);

  float y_range = (pitch+1)*h_pitch/7;;
  painter->drawLine(QPoint(x_pitch, y_range), QPoint(x_pitch + w/2, y_range));

  if(m_mousePos)
  {
    pen.setStyle(Qt::SolidLine);
    painter->setFont(QFont("Sans", 7));

    QRectF r1 = pitchButtonRect();
    if(r1.contains(*m_mousePos))
      pen.setColor(focusColor);
    else
      pen.setColor(drawColor);
    painter->setPen(pen);
    painter->drawRect(r1);
    painter->drawText(r1, "PITCH", QTextOption(Qt::AlignCenter));


    QRectF r2 = rangeButtonRect();
    if(r2.contains(*m_mousePos))
      pen.setColor(focusColor);
    else
      pen.setColor(drawColor);
    painter->setPen(pen);
    painter->drawRect(r2);
    painter->drawText(r2, "RANGE", QTextOption(Qt::AlignCenter));
  }
  painter->setRenderHint(QPainter::Antialiasing, false);

}


QRectF MelodicKeyView::pitchButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0, y0, w / 2, h}.adjusted(border, border, -border, -border);
}

QRectF MelodicKeyView::rangeButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + w / 2, y0, w / 2, h}.adjusted(border, border, -border, -border);
}

QRectF MelodicKeyView::computeRect() const noexcept
{
  auto& view = *(View*)parentItem();
  const auto h = view.height();
  const auto w = view.defaultWidth();
  const QRectF rect{
      0.1*w,
      0,
      0.1*w,
      h};

  return rect;
}


bool MelodicKeyView::canEdit() const
{
  return boundingRect().height() > 5;
}

void MelodicKeyView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  setSelected(true);

  m_action = None;

  if (canEdit())
  {
    if(pitchButtonRect().contains(event->pos()))
      m_action = ChangePitch;
    else if (rangeButtonRect().contains(event->pos()))
      m_action = ChangeRange;
  }
  event->accept();
}

void MelodicKeyView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  if (canEdit())
  {
    switch (m_action)
    {
      case ChangePitch:
      {
        Pitch p = (Pitch) (((int) melodicKey.pitch() + 1)%7);
        m_presenter.on_melodicKeyPitchChanged(melodicKey, p);
        break;
      }
      case ChangeRange:
      {
        Range r = (Range) (((int) melodicKey.range() + 1)%3);
        m_presenter.on_melodicKeyRangeChanged(melodicKey, r);
        break;
      }
      default:
        break;
    }
  }
  event->accept();
  m_action = None;
}

void MelodicKeyView::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = event->pos();
  update();
}
void MelodicKeyView::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = event->pos();
  update();
}
void MelodicKeyView::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = std::nullopt;
  update();
}
}
