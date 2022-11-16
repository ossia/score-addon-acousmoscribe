#include "SignView.hpp"

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QPainter>

#include <Acousmoscribe/Model/Sign.hpp>
#include <Acousmoscribe/Presenter/Presenter.hpp>
#include <Acousmoscribe/View/Utils.hpp>
#include <Acousmoscribe/View/View.hpp>

namespace Acousmoscribe
{

SignView::SignView(const Sign& n, Presenter& presenter, View* parent)
    : QGraphicsItem{parent}
    , sign{n}
    , m_presenter{presenter}
    , m_action{None}
{
  this->setFlag(QGraphicsItem::ItemIsSelectable, true);
  this->setFlag(QGraphicsItem::ItemIsMovable, true);
  this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  this->setAcceptHoverEvents(true);
  this->setZValue(1);
}

QPolygonF SignView::polygon() const noexcept
{
  QPolygonF p;
  p.reserve(4);

  const auto [x0, y0, w, h]{baseItemRect(m_width, m_height)};

  const auto& profile = sign.dynamicProfile();
  const float volumeStart = profile.volumeStart;
  const float volumeEnd = profile.volumeEnd;

  p.push_back({x0, y0 + h});
  p.push_back({x0, y0 + h * (1 - volumeStart)});
  p.push_back({x0 + w, y0 + h * (1 - volumeEnd)});
  p.push_back({x0 + w, y0 + h});

  return p;
}
void SignView::paint(
    QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  QPen p;
  p.setWidth(2);
  painter->setPen(p);

  const auto [x0, y0, w, h]{baseItemRect(m_width, m_height)};
  painter->drawRect(x0, y0, w, h);
  //float x_left = x0;
  float x_right = x0 + w;

  Grain grain = sign.grainProfile().grain;
  Speed speed = sign.rhythmicProfile().speed;
  Acceleration acc = sign.rhythmicProfile().acceleration;
  Pace pace = sign.melodicProfile().pace;
  Variation rhythmVar = sign.rhythmicProfile().variation;
  Variation grainVar = sign.grainProfile().variation;
  Variation melodicVar = sign.melodicProfile().variation;

  //double duration = sign.duration();
  float volumeStart = sign.dynamicProfile().volumeStart;
  float volumeEnd = sign.dynamicProfile().volumeEnd;
  float volumeUsed;

  Pitch pitch = sign.melodicProfile().pitch;
  Pitch pitchEnd = sign.melodicProfile().pitchEnd;

  QPointF a(x0, y0 + h);
  QPointF b(x0, y0 + h * (1 - volumeStart));
  QPointF c(x0 + w, y0 + h * (1 - volumeEnd));
  QPointF d(x0 + w, y0 + h);

  double x_pitch = x0;      //x coordinate of the melodic profile on the sign
  volumeUsed = volumeStart; // std::max(volumeStart, volumeEnd);
  if(volumeUsed == volumeStart)
  {
    x_pitch += x_right * 0.025;
    ;
  }
  else
  {
    x_pitch += x_right * 0.95;
  }

  p.setStyle(Qt::DotLine);
  painter->setBrush(fillColor);

  QColor col = drawColor;
  if(isSelected())
    col = selectColor;
  else if(m_mousePos)
    col = focusColor;

  p.setColor(col);
  p.setCapStyle(Qt::RoundCap);
  p.setWidth(2);
  painter->setPen(p);

  /* GRAIN */
  switch(grain)
  {
    case smooth:
      p.setStyle(Qt::SolidLine);
      break;
    case fine:
      p.setStyle(Qt::DotLine);
      break;
    case sharp:
      p.setStyle(Qt::DashDotLine);
      break;
    case big:
      p.setStyle(Qt::DashLine);
      break;
  }

  painter->setPen(p);
  if(grainVar == random_var)
  {
    drawVariationLine(painter, {b, c});
  }
  else
  {
    painter->drawLine(b, c);
  }

  /* RHYTHMIC */
  switch(speed)
  {
    case slow:
      p.setStyle(Qt::DashLine);
      break;
    case medium:
      p.setStyle(Qt::DashDotLine);
      break;
    case fast:
      p.setStyle(Qt::DotLine);
      break;
    default:
      p.setStyle(Qt::SolidLine);
      break;
  }

  painter->setPen(p);
  if(rhythmVar == random_var)
  {
    drawVariationLine(painter, {a, d}, false);
  }
  else
  {
    painter->drawLine(a, d);
  }

  p.setStyle(Qt::SolidLine);
  painter->setPen(p);

  //Drawing side lines

  painter->drawLine(a, b);
  painter->drawLine(c, d);

  float h_acc = h - std::min(h * volumeStart / 3, h * volumeEnd / 3);

  /* ACCELERATION */
  switch(acc)
  {
    case decelerating:
      painter->drawLine(
          QPointF(x0 + x_right * 0.15, y0 + h),
          QPointF(x0 + x_right * 0.15, y0 + h_acc));
      break;
    case accelerating:
      painter->drawLine(
          QPointF(x0 + x_right * 0.75, y0 + h),
          QPointF(x0 + x_right * 0.75, y0 + h_acc));
      break;
    default:
      break;
  }

  /* MELODIC */

  const float h_pitch = volumeUsed * 0.65;

  float pitchY[7];
  // Draw pitch points
  const bool focusedOrSelected = this->isSelected() || this->m_mousePos;
  for(int i = 1; i <= 7; i++)
  {
    const float y_pitch = y0 + h - volumeUsed * h + i * h_pitch * h / 5;
    pitchY[i - 1] = y_pitch;

    if(focusedOrSelected)
    {
      if(i == 4)
      {
        p.setWidth(5); // the 4th point is bigger
        painter->setPen(p);
        if(b.y() > (y0 + (h * 0.5)))
        {
          painter->drawPoint(QPoint(w - x_pitch, y_pitch));
        }
        else
        {
          painter->drawPoint(QPoint(x_pitch, y_pitch));
        }
        p.setWidth(2);
        painter->setPen(p);
      }
      else
      {
        if(b.y() > (y0 + (h * 0.5)))
        {
          painter->drawPoint(QPoint(w - x_pitch, y_pitch));
        }
        else
        {
          painter->drawPoint(QPoint(x_pitch, y_pitch));
        }
      }
    }
  }

  // Draw pitch line
  {
    p.setWidth(2);
    painter->setPen(p);
    auto firstP = QPoint{};
    auto secondP = QPoint{};
    if(b.y() > (y0 + (h * 0.5)))
    {
      firstP = QPoint(w + x_pitch - 0.015 * w, pitchY[pitch]);
      secondP = QPoint(w + x_pitch - 0.3 * w, pitchY[pitchEnd]);
    }
    else
    {
      firstP = QPoint(x_pitch + 0.015 * w, pitchY[pitch]);
      secondP = QPoint(x_pitch + 0.3 * w, pitchY[pitchEnd]);
    }

    if(pace != Pace::unknow && firstP.y() == secondP.y())
    {
      int half = (secondP.x() - firstP.x()) / 2.0;
      QPoint a{firstP.x(), firstP.y() - half};
      QPoint b{secondP.x(), firstP.y() + half};
      QRectF drawR{a, b};
      switch(pace)
      {
        case lento: {
          painter->drawArc(drawR, 0 * 16, 180 * 16);
          break;
        }
        case moderato: {

          QRectF first{
              drawR.topLeft(),
              QPointF{drawR.bottomRight().x() / 2, drawR.bottomRight().y()}};
          QRectF second{
              QPointF{first.bottomRight().x(), drawR.topLeft().y()},
              drawR.bottomRight()};
          painter->drawArc(first, 0 * 16, 180 * 16);
          painter->drawArc(second, 0 * 16, -180 * 16);
          break;
        }
        case presto: {
          QRectF first{
              drawR.topLeft(),
              QPointF{drawR.bottomRight().x() / 3, drawR.bottomRight().y()}};
          QRectF second{
              QPointF{first.bottomRight().x(), drawR.topLeft().y()},
              QPointF{drawR.bottomRight().x() * 2 / 3, drawR.bottomLeft().y()}};
          QRectF third{
              QPointF{second.bottomRight().x(), drawR.topLeft().y()},
              drawR.bottomRight()};
          painter->drawArc(first, 0 * 16, 180 * 16);
          painter->drawArc(second, 0 * 16, -180 * 16);
          painter->drawArc(third, 0 * 16, 180 * 16);
          break;
        }
      }
    }
    else
    {
      painter->drawLine(firstP, secondP);
    }

    if(melodicVar == random_var)
    {
      int span = 8;
      painter->drawLine(secondP, QPoint{secondP.x() + 8, secondP.y() + 8});
      painter->drawLine(
          QPoint{secondP.x() + 8, secondP.y() + 8},
          QPoint{secondP.x() + 16, secondP.y()});
    }
  }

  // Draw control buttons

  if(m_mousePos && w > 30)
  {
    p.setStyle(Qt::SolidLine);

    painter->setFont(buttonFont(w / 4., h / 10.));
    pitchStartButton = button(0, 0);
    grainButton = button(1, 0);
    paceButton = button(2, 0);
    pitchEndButton = button(3, 0);
    volumeStartButton = button(0, 1);
    speedButton = button(1, 1);
    accelerationButton = button(2, 1);
    volumeEndButton = button(3, 1);
    grainVarButton = button(0, 2);
    speedVarButton = button(1, 2);
    pitchVarButton = button(2, 2);
    const auto& textOpts = buttonTextOptions();
    for(auto [r1, text] :
        {std::make_pair(pitchStartButton, "| P"), std::make_pair(grainButton, "GRN"),
         std::make_pair(paceButton, "PCE"), std::make_pair(pitchEndButton, "P |"),
         std::make_pair(volumeStartButton, "| V"), std::make_pair(speedButton, "SPD"),
         std::make_pair(accelerationButton, "ACC"),
         std::make_pair(volumeEndButton, "V |"), std::make_pair(grainVarButton, "GR V"),
         std::make_pair(speedVarButton, "Sp V"), std::make_pair(pitchVarButton, "P V")})
    {
      if(r1.contains(*m_mousePos))
        p.setColor(focusColor);
      else
        p.setColor(drawColor);

      painter->setPen(p);
      painter->drawRect(r1);
      painter->drawText(r1, text, textOpts);
    }
  }
  painter->setRenderHint(QPainter::Antialiasing, false);
}

QPointF SignView::closestPos(QPointF newPos) const noexcept
{
  auto& view = *(View*)parentItem();
  const QRectF rect = view.boundingRect();
  const auto height = rect.height();

  const auto w = view.defaultWidth();

  newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
  newPos.setY(0);

  return newPos;
}

bool SignView::contains(const QPointF& point) const
{
  if(point.y() > 0.8 * m_height)
    return true;
  if(polygon().containsPoint(point, {}))
    return true;
  return false;
}
QPainterPath SignView::shape() const
{
  QPainterPath p;

  const auto& profile = sign.dynamicProfile();
  const auto [x0, y0, w, h]{baseItemRect(m_width, m_height)};

  p.moveTo(0, (1 - profile.volumeStart) * h);
  p.lineTo(0, m_height);
  p.lineTo(m_width, m_height);
  p.lineTo(m_width, (1 - profile.volumeEnd) * h);
  p.closeSubpath();

  return p;
}
void SignView::drawVariationLine(QPainter* p, QLineF line, bool down) const
{

  int dir = (down ? 1 : -1);
  int span = 8;
  QPointF midL = (line.p2() + line.p1()) / 2;
  midL.rx() -= span;
  QPointF midR = (line.p2() + line.p1()) / 2;
  midR.rx() += span;
  QPointF mid = (line.p2() + line.p1()) / 2;
  mid.ry() += (span * dir);

  p->drawLine(line.p1(), midL);
  p->drawLine(midL, mid);
  p->drawLine(mid, midR);
  p->drawLine(midR, line.p2());
}
QRectF SignView::computeRect() const noexcept
{
  auto& view = *(View*)parentItem();
  const auto h = view.height();
  // const auto w = view.width();
  const auto w = view.defaultWidth(); // changé
  const QRectF rect{sign.start() * w, 0, sign.duration() * w, h};

  return rect;
}

QVariant
SignView::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  switch(change)
  {
    case QGraphicsItem::ItemSelectedChange: {
      const bool b = value.toBool();
      this->m_presenter.on_selectionChanged(this, b);
      this->setZValue(10 * (int)b);
      break;
    }
    default:
      break;
  }

  return QGraphicsItem::itemChange(change, value);
}

void SignView::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  if(!contains(event->pos()))
  {
    hoverLeaveEvent(event);
    return;
  }

  m_mousePos = event->pos();

  if(event->pos().x() >= this->boundingRect().width() - 2)
  {
    auto& skin = score::Skin::instance();
    this->setCursor(skin.CursorScaleH);
  }
  else
  {
    this->unsetCursor();
  }

  update();
}

void SignView::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  if(!contains(event->pos()))
  {
    hoverLeaveEvent(event);
    return;
  }

  m_mousePos = event->pos();
  float h = this->boundingRect().height();
  float w = this->boundingRect().width();

  if(event->pos().x() >= w - 10 && event->pos().y() <= h * 0.8)
  {
    auto& skin = score::Skin::instance(); // Scale
    this->setCursor(skin.CursorScaleH);
  }
  else
  {
    this->unsetCursor();
  }

  update();
}

void SignView::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = std::nullopt;
  this->unsetCursor();
  update();
}

bool SignView::canEdit() const
{
  return boundingRect().height() > 5;
}

static QPointF signview_origpoint;
void SignView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  if(!contains(event->pos()))
  {
    event->ignore();
    return;
  }

  const auto mods = QGuiApplication::keyboardModifiers();
  if(!(mods & Qt::ControlModifier) && !isSelected())
    m_presenter.on_deselectOtherSigns();

  setSelected(true);

  float h = this->boundingRect().height();
  float w = this->boundingRect().width();

  m_action = None;

  if(canEdit())
  {
    const auto pos = event->pos();
    if(event->pos().x() >= w - 10 && event->pos().y() <= h * 0.8)
    {
      m_action = Scale;
    }
    else if(grainButton.contains(pos))
    {
      m_action = ChangeGrainProfileGrain;
    }
    else if(grainVarButton.contains(pos))
    {
      m_action = ChangeGrainProfileVar;
    }
    else if(speedButton.contains(pos))
    {
      m_action = ChangeRhythmicProfileSpeed;
    }
    else if(accelerationButton.contains(pos))
    {
      m_action = ChangeRhythmicProfileAcceleration;
    }
    else if(paceButton.contains(pos))
    {
      m_action = ChangeMelodicProfilePace;
    }
    else if(pitchStartButton.contains(pos))
    {
      m_action = ChangeMelodicProfilePitch;
    }
    else if(pitchEndButton.contains(pos))
    {
      m_action = ChangeMelodicProfilePitchEnd;
    }
    else if(volumeStartButton.contains(pos))
    {
      m_action = ChangeSignVolumeStart;
    }
    else if(volumeEndButton.contains(pos))
    {
      m_action = ChangeSignVolumeEnd;
    }
    else if(pitchVarButton.contains(pos))
    {
      m_action = ChangeMelodicProfileVariation;
    }
    else if(speedVarButton.contains(pos))
    {
      m_action = ChangeRhythmicProfileVariation;
    }
    else
    {
      m_action = Move;
      signview_origpoint = this->pos();
    }
  }
  event->accept();
}

QRectF SignView::button(int i, int j) const noexcept
{
  const auto [x0, y0, w, h]{buttonItemRect(m_width, m_height)};
  const int border = 3;
  return QRectF{x0 + i * (w / 4), y0 + j * (h / 3), w / 4, h / 3}.adjusted(
      border, border, -border, -border);
}

void SignView::mouseMoveEvent(QGraphicsSceneMouseEvent* event)
{
  if(canEdit())
  {
    switch(m_action)
    {
      case Move:
        this->setPos(closestPos(
            signview_origpoint + event->scenePos()
            - event->buttonDownScenePos(Qt::LeftButton)));
        m_presenter.on_signMoved(*this);
        break;
      case Scale:
        this->setWidth(std::max(2., event->pos().x()));
        break;
      case Duplicate:
        m_presenter.on_signDuplicate();
        break;

      case None:
      default:
        break;
    }
  }
  event->accept();
}

void SignView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  if(canEdit())
  {
    switch(m_action)
    {
      case Move: {
        this->setPos(closestPos(
            signview_origpoint + event->scenePos()
            - event->buttonDownScenePos(Qt::LeftButton)));
        m_presenter.on_signMoved(*this);
        m_presenter.on_signMoveFinished(*this);
        break;
      }
      case Scale: {
        this->setWidth(std::max(2., event->pos().x()));
        m_presenter.on_signScaled(sign, m_width / ((View*)parentItem())->defaultWidth());
        break;
      }
      case Duplicate: {
        m_presenter.on_signDuplicate();
        break;
      }
      case ChangeSignVolumeStart: {
        float v = sign.dynamicProfile().volumeStart + 0.2;
        v = v > 1. ? 0.0 : v;
        m_presenter.on_signVolumeStartChanged(sign, v);
        break;
      }
      case ChangeSignVolumeEnd: {
        float ve = sign.dynamicProfile().volumeEnd + 0.2;
        ve = ve > 1. ? 0.0 : ve;
        m_presenter.on_signVolumeEndChanged(sign, ve);
        break;
      }
      case ChangeMelodicProfilePitch: {
        Pitch p = (Pitch)(((int)sign.melodicProfile().pitch - 1 + 7) % 7);
        m_presenter.on_signMelodicProfilePitchChanged(sign, p);
        break;
      }
      case ChangeMelodicProfilePitchEnd: {
        Pitch pe = (Pitch)(((int)sign.melodicProfile().pitchEnd - 1 + 7) % 7);
        m_presenter.on_signMelodicProfilePitchEndChanged(sign, pe);
        break;
      }
      case ChangeMelodicProfileVariation: {
        Variation v = (Variation)(((int)sign.melodicProfile().variation + 1) % 2);
        m_presenter.on_signMelodicProfileVariationChanged(sign, v);
        break;
      }
      case ChangeMelodicProfilePace: {
        Pace p = (Pace)(((int)sign.melodicProfile().pace + 1) % 4);
        m_presenter.on_signMelodicProfilePaceChanged(sign, p);
        break;
      }
      case ChangeRhythmicProfileSpeed: {
        Speed s = (Speed)(((int)sign.rhythmicProfile().speed + 1) % 4);
        m_presenter.on_signRhythmicProfileSpeedChanged(sign, s);
        break;
      }
      case ChangeRhythmicProfileVariation: {
        Variation v = (Variation)(((int)sign.rhythmicProfile().variation + 1) % 2);
        m_presenter.on_signRhythmicProfileVariationChanged(sign, v);
        break;
      }
      case ChangeGrainProfileGrain: {
        Grain g = (Grain)(((int)sign.grainProfile().grain + 1) % 4);
        m_presenter.on_signGrainProfileGrainChanged(sign, g);
        break;
      }
      case ChangeGrainProfileVar: {
        Variation v = (Variation)(((int)sign.grainProfile().variation + 1) % 2);
        m_presenter.on_signGrainProfileVariationChanged(sign, v);
        break;
      }
      case ChangeSpeed: {
        qDebug("TODO");
        break;
      }
      case ChangeRhythmicProfileAcceleration: {
        Acceleration a
            = (Acceleration)(((Acceleration)sign.rhythmicProfile().acceleration + 1) % 3);
        m_presenter.on_signRhythmicProfileAccelerationChanged(sign, a);
        break;
      }
      case None:
        break;
    }
  }
  event->accept();
  m_action = None;
}
}
