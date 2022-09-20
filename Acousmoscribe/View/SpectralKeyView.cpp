// This is an open source non-commercial project. Dear PVS-Studio, please check
// it. PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGuiApplication>
#include <QPainter>
#include <QPoint>
#include <QToolTip>

#include <Acousmoscribe/Presenter/Presenter.hpp>
#include <Acousmoscribe/View/SpectralKeyView.hpp>
#include <Acousmoscribe/View/View.hpp>
#include <Acousmoscribe/View/Utils.hpp>
#include <ossia/detail/math.hpp>


namespace Acousmoscribe
{
SpectralKeyView::SpectralKeyView(const SpectralKey& sk, Presenter& p, View* parent)
    : QGraphicsItem{parent}, spectralKey{sk}, m_presenter{p}, m_action{None}
{
  this->setFlag(QGraphicsItem::ItemIsSelectable, true);
  this->setFlag(QGraphicsItem::ItemIsMovable, false);
  this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
  this->setAcceptHoverEvents(true);
  this->setZValue(3);
}

void SpectralKeyView::paint(
    QPainter* painter,
    const QStyleOptionGraphicsItem* option,
    QWidget* widget)
{
  painter->setRenderHint(QPainter::Antialiasing, true);

  const auto [x0, y0, w, h] { baseItemRect(m_width, m_height) };

  Nature nature = spectralKey.nature();
  Nature h_nature = spectralKey.hybrid_nature();
  Nature h_nature2 = spectralKey.hybrid_nature2();
  Nature nature2 = spectralKey.nature2();
  Coloration color = spectralKey.coloration();
  Coloration color2 = spectralKey.coloration2();
  bool rich = spectralKey.isRich();
  bool rich2 = spectralKey.isRich2();
  bool hybrid = spectralKey.isHybrid();
  bool hybrid2 = spectralKey.isHybrid2();

  QPen p;
  p.setColor(drawColor);
  p.setWidth(2);
  painter->setPen(p);

  /* Background Rect */

  // painter->setBrush(Qt::white);
  // painter->drawRect(x0, y0, w, h);


  if (hybrid)
  {
    switch (h_nature)
    {
      case tonic:
        p.setStyle(Qt::DashLine);
        break;

      case noise:
        p.setStyle(Qt::DotLine);
        break;

      default:
        break;
    }
  }

  painter->setPen(p);
  QPoint lineStart(w / 5, h / 1.05);
  QPoint lineEnd(w / 1.05, h / 5);
  QPoint lineMid(w / 1.5, h / 1.5);
  bool drawLo = false;
  bool drawMed = false;
  bool drawHi = false;
  switch(color){
    case lo:
      drawLo = true;
    break;
    case med:
      drawMed = true;
    break;
    case hi:
     drawHi = true;
    break;
    case lo_med:
     drawLo = true;
     drawMed= true;
    break;
    case med_hi:
    drawMed = true;
    drawHi = true;
    break;
  case lo_hi:
    drawLo=true;
    drawHi = true;
    break;
  default:
    break;
  }

  switch (nature)
  {
    case tonic:
        painter->drawLine(lineStart, lineEnd);
        if(drawLo)
          painter->drawEllipse(lineStart,4,4);
        if(drawMed)
          painter->drawEllipse(QPoint((lineStart.x()+lineEnd.x())/2,(lineStart.y()+lineEnd.y())/2),4,4);
        if(drawHi)
          painter->drawEllipse(lineEnd,4,4);
      break;

    case tonic_warped:
        painter->drawLine(lineStart, lineMid);
        painter->drawLine(lineMid, lineEnd);
        if(drawLo)
          painter->drawEllipse(lineStart,4,4);
        if(drawMed)
          painter->drawEllipse(lineMid,4,4);
        if(drawHi)
          painter->drawEllipse(lineEnd,4,4);
    break;

  case inharmonic:{
    QRectF r(w / 11, h / 30, w / 3, h / 3);
    int startAngle = 225;
    int angleSpan = 180;
    int endAngle = (startAngle+angleSpan)%360;
    int midAngle = (startAngle+endAngle)/2;
    double a = r.width()/2;
    double b = r.height()/2;
    painter->drawArc(r, (startAngle*16), (angleSpan*16));

    double startRad = ossia::radian{ossia::degree{startAngle+5}}.dataspace_value;
    startRad = std::atan((a/b)*std::tan(startRad));
    double loX = a*std::cos(startRad);
    double loY = b*std::sin(startRad);
    QPointF loPoint{-loX+r.center().x(),loY+r.center().y()};
    if(drawLo)
      painter->drawEllipse(loPoint,4,4);

    double endRad = ossia::radian{ossia::degree{endAngle}}.dataspace_value;
    endRad = std::atan((a/b)*std::tan(endRad));
    double hiX = a*std::cos(endRad);
    double hiY = b*std::sin(endRad);
    QPointF hiPoint{hiX+r.center().x(),-hiY+r.center().y()}; //fix having to manually change the signs
    if(drawHi)
      painter->drawEllipse(hiPoint,4,4);

    double midRad = ossia::radian{ossia::degree{midAngle}}.dataspace_value;
    endRad = std::atan((a/b)*std::tan(midRad));
    double medX = a*std::cos(endRad);
    double medY = b*std::sin(endRad);
    QPointF medPoint{medX+r.center().x(),-medY+r.center().y()}; //fix having to manually change the signs
    if(drawMed)
      painter->drawEllipse(medPoint,4,4);

    break;
  }

    case noise:
      p.setWidth(4);
      painter->setPen(p);
      painter->drawPoint(w / 2.5, h / 2.5);
      p.setWidth(2);
      painter->setPen(p);
      break;

    default:
      break;
  }
  p.setStyle(Qt::SolidLine);
  if (hybrid2)
  {
    switch (h_nature2)
    {

    case tonic:
      p.setStyle(Qt::DashLine);
      break;

    case noise:
      p.setStyle(Qt::DotLine);
      break;

    default:
      break;
    }
  }
  painter->setPen(p);

  QPoint line2Start(w / 10, h / 1.2);
  QPoint line2End(w / 1.2, h / 10);
  QPoint line2Mid(w / 1.7, h / 2.1);
  drawLo = false;
  drawMed = false;
  drawHi = false;
  switch(color2){
    case lo:
      drawLo = true;
    break;
    case med:
      drawMed = true;
    break;
    case hi:
     drawHi = true;
    break;
    case lo_med:
     drawLo = true;
     drawMed= true;
    break;
    case med_hi:
    drawMed = true;
    drawHi = true;
    break;
  case lo_hi:
    drawLo=true;
    drawHi = true;
    break;
  default:
    break;
  }
  switch (nature2)
  {
    case tonic:
        painter->drawLine(line2Start, line2End);
        if(drawLo)
          painter->drawEllipse(line2Start,4,4);
        if(drawMed)
          painter->drawEllipse(QPoint((line2Start.x()+line2End.x())/2,(line2Start.y()+line2End.y())/2),4,4);
        if(drawHi)
          painter->drawEllipse(line2End,4,4);
    break;
    case tonic_warped:
        painter->drawLine(line2Start, line2Mid);
        painter->drawLine(line2Mid, line2End);
        if(drawLo)
          painter->drawEllipse(line2Start,4,4);
        if(drawMed)
          painter->drawEllipse(line2Mid,4,4);
        if(drawHi)
          painter->drawEllipse(line2End,4,4);
    break;
  case inharmonic:{
    QRectF r(w / 11, h / 30, w / 2, h / 2);

    int startAngle = 225;
    int angleSpan = 180;
    int endAngle = (startAngle+angleSpan)%360;
    int midAngle = (startAngle+endAngle)/2;

    painter->drawArc(r, startAngle*16, angleSpan*16);
    double a = r.width()/2;
    double b = r.height()/2;

    double startRad = ossia::radian{ossia::degree{startAngle+5}}.dataspace_value;
    startRad = std::atan((a/b)*std::tan(startRad));
    double loX = a*std::cos(startRad);
    double loY = b*std::sin(startRad);
    QPointF loPoint{-loX+r.center().x(),loY+r.center().y()};
    if(drawLo)
      painter->drawEllipse(loPoint,4,4);

    double endRad = ossia::radian{ossia::degree{endAngle}}.dataspace_value;
    endRad = std::atan((a/b)*std::tan(endRad));
    double hiX = a*std::cos(endRad);
    double hiY = b*std::sin(endRad);
    QPointF hiPoint{hiX+r.center().x(),-hiY+r.center().y()}; //fix having to manually change the signs
    if(drawHi)
      painter->drawEllipse(hiPoint,4,4);

    double midRad = ossia::radian{ossia::degree{midAngle}}.dataspace_value;
    endRad = std::atan((a/b)*std::tan(midRad));
    double medX = a*std::cos(endRad);
    double medY = b*std::sin(endRad);
    QPointF medPoint{medX+r.center().x(),-medY+r.center().y()}; //fix having to manually change the signs
    if(drawMed)
      painter->drawEllipse(medPoint,4,4);
    break;
  }

    case noise:
      p.setWidth(4);
      painter->setPen(p);
      painter->drawPoint(w / 1.3, h / 1.3);
      p.setWidth(2);
      painter->setPen(p);
      break;

    default:
      break;
  }

  p.setStyle(Qt::SolidLine);
  painter->setPen(p);

  if(rich)
  {
    if (nature == tonic)
    {
      painter->drawLine(QPoint(w / 1.08, h / 6), QPoint(w / 1.015, h / 4.25));
      painter->drawLine(QPoint(w / 6, h / 1.1), QPoint(w / 4.5, h / 1.02));
    }
    else if (nature == inharmonic)
    {
      painter->drawLine(QPoint(w / 2.3, h / 9.6), QPoint(w / 2.6, h / 6.2));
      painter->drawLine(QPoint(w / 4.15, h / 2.85), QPoint(w / 4.9, h / 2.6));
    }
    else if (nature == noise)
    {
      painter->drawLine(QPoint(w / 2.6, h / 4.2), QPoint(w / 3.7, h / 2.8));
      if (nature2 != noise)
        painter->drawLine(QPoint(w / 2.45, h / 2), QPoint(w / 1.85, h / 2.7));
    }
  }

  if(rich2)
  {
    if (nature2 == tonic)
    {
      painter->drawLine(QPoint(w / 1.15, h / 7.2), QPoint(w / 1.25, h / 14.8));
      painter->drawLine(QPoint(w / 14.8, h / 1.25), QPoint(w / 6.8, h / 1.12));
    }
    else if (nature2 == inharmonic)
    {
      painter->drawLine(QPoint(w / 1.9, h / 4.2), QPoint(w / 1.7, h / 6.4));
      painter->drawLine(QPoint(w / 3.1, h / 2), QPoint(w / 3.9, h / 1.8));
    }
    else if (nature2 == noise)
    {
      painter->drawLine(QPoint(w / 1.2, h / 1.1), QPoint(w / 1.05, h / 1.3));
      if (nature == noise)
      {
        painter->drawLine(QPoint(w / 2, h / 1.65), QPoint(w / 1.5, h / 2.3));
      }
      else
      {
        painter->drawLine(QPoint(w / 1.55, h / 1.3), QPoint(w / 1.3, h / 1.6));
      }
    }
  }

  if(m_mousePos && w > 30)
  {
    p.setStyle(Qt::SolidLine);
    painter->setFont(buttonFont(w/4., h/10.));

    for(auto [r1, text] : {
        std::make_pair(changeNatureButtonRect(), "N"),
        std::make_pair(changeNature2ButtonRect(), "N2"),
        std::make_pair(changeRichButtonRect(), "R"),
        std::make_pair(changeRich2ButtonRect(), "R2"),
        std::make_pair(changeHybridButtonRect(), "H"),
        std::make_pair(changeHybrid2ButtonRect(), "H2"),
        std::make_pair(changeColorationButtonRect(),"C1"),
        std::make_pair(changeColoration2ButtonRect(),"C2"),
      })
    {
      if(r1.contains(*m_mousePos))
        p.setColor(focusColor);
      else
        p.setColor(drawColor);
      painter->setPen(p);
      painter->drawRect(r1);
      painter->drawText(r1, text, QTextOption(Qt::AlignCenter));
    }
  }
  painter->setRenderHint(QPainter::Antialiasing, false);
}

QRectF SpectralKeyView::computeRect() const noexcept
{
  auto& view = *(View*)parentItem();
  const auto h = view.height();
  const QRectF rect{
      0, // 35 à changer (instant de départ)
      0,
      100.,
      h};

  return rect;
}

bool SpectralKeyView::canEdit() const
{
  return boundingRect().height() > 5;
}

QRectF SpectralKeyView::changeNatureButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + 0, y0, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}

QRectF SpectralKeyView::changeNature2ButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + 0, y0 + 0.5 * h, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}


QRectF SpectralKeyView::changeHybridButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 +  w / 4, y0, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}

QRectF SpectralKeyView::changeHybrid2ButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + w / 4, y0 + 0.5 * h, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}

QRectF SpectralKeyView::changeRichButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + 2*w / 4, y0, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}

QRectF SpectralKeyView::changeRich2ButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + 2*w / 4, y0 + 0.5 * h, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}


QRectF SpectralKeyView::changeColorationButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + 3*w / 4, y0, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}

QRectF SpectralKeyView::changeColoration2ButtonRect() const noexcept
{
  const auto [x0, y0, w, h] { buttonItemRect(m_width, m_height) };
  const int border = 3;
  return QRectF{x0 + 3*w / 4, y0 + 0.5 * h, w / 4, 0.5 * h}.adjusted(border, border, -border, -border);
}

void SpectralKeyView::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
  const auto mods = QGuiApplication::keyboardModifiers();
  if (!(mods & Qt::ControlModifier) && !isSelected())
    m_presenter.on_deselectOtherSigns();

  float h = this->boundingRect().height();
  float w = this->boundingRect().width();

  m_action = None;

  if (canEdit())
  {
    const auto pos = event->pos();
    if(changeNatureButtonRect().contains(pos))
      m_action = ChangeNature;
    else if(changeNature2ButtonRect().contains(pos))
      m_action = ChangeNature2;
    else if(changeRichButtonRect().contains(pos))
      m_action = ChangeRich;
    else if(changeRich2ButtonRect().contains(pos))
      m_action = ChangeRich2;
    else if(changeHybridButtonRect().contains(pos) && spectralKey.canBeHybrid(spectralKey.nature()))
      m_action = ChangeHybrid;
    else if(changeHybrid2ButtonRect().contains(pos)&& spectralKey.canBeHybrid(spectralKey.nature2()))
      m_action = ChangeHybrid2;
    else if(changeColorationButtonRect().contains(pos))
      m_action = ChangeColoration;
    else if(changeColoration2ButtonRect().contains(pos))
      m_action = ChangeColoration2;
  }
  event->accept();
}

void SpectralKeyView::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
  if (canEdit())
  {

    switch (m_action)
    {
      case ChangeNature:{
        Nature n = (Nature) (((int) spectralKey.nature()%5 + 1));
        m_presenter.on_spectralKeyNatureChanged(spectralKey, n);
        if(!spectralKey.canBeHybrid(n)){
          m_presenter.on_spectralKeyIsHybridChanged(spectralKey, false);
        }
        break;
      }
      case ChangeNature2:{
        Nature n = (Nature) (((int) spectralKey.nature2() %5 + 1));
        m_presenter.on_spectralKeyNature2Changed(spectralKey, n);
        if(!spectralKey.canBeHybrid(n)){
          m_presenter.on_spectralKeyIsHybrid2Changed(spectralKey, false);
        }
        break;
      }
      case ChangeRich:{
        bool b = !(spectralKey.isRich());
        m_presenter.on_spectralKeyIsRichChanged(spectralKey, b);
        break;
      }
      case ChangeRich2:{
        bool b = !(spectralKey.isRich2());
        m_presenter.on_spectralKeyIsRich2Changed(spectralKey, b);
        break;
      }
      case ChangeHybrid:{
        if(spectralKey.nature()==Nature::tonic){
          bool b = !(spectralKey.isHybrid());
          m_presenter.on_spectralKeyIsHybridChanged(spectralKey, b);
          m_presenter.on_spectralKeyHybridNatureChanged(spectralKey,Nature::noise);
        } else{
          Nature n = (Nature) (((int) spectralKey.hybrid_nature()%3 + 1));
          bool b = ((int)n != 0);
          m_presenter.on_spectralKeyIsHybridChanged(spectralKey, b);
          m_presenter.on_spectralKeyHybridNatureChanged(spectralKey,n);
        }

        break;
      }
      case ChangeHybrid2:{
      if(spectralKey.nature2()==Nature::tonic){
        bool b = !(spectralKey.isHybrid2());
        m_presenter.on_spectralKeyIsHybrid2Changed(spectralKey, b);
        m_presenter.on_spectralKeyHybridNature2Changed(spectralKey,Nature::noise);
      } else{
        Nature n = (Nature) (((int) spectralKey.hybrid_nature2()%3 + 1));
        bool b = ((int)n != 0);
        m_presenter.on_spectralKeyIsHybrid2Changed(spectralKey, b);
        m_presenter.on_spectralKeyHybridNature2Changed(spectralKey,n);
      }
        break;
      }
    case ChangeColoration:{
      Coloration c = (Coloration) ((int)(spectralKey.coloration()%7 +1));
      m_presenter.on_spectralKeyColorationChanged(spectralKey,c);
      break;
    }
    case ChangeColoration2:{
      Coloration c = (Coloration) ((int)(spectralKey.coloration2()%7 +1));
      m_presenter.on_spectralKeyColoration2Changed(spectralKey,c);
      break;
    }
    default:
      break;
    }
  }
  event->accept();
  m_action = None;
}

QVariant
SpectralKeyView::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant& value)
{
  switch (change)
  {
    case QGraphicsItem::ItemSelectedChange:
    {
      this->setZValue(10 * (int)value.toBool());
      break;
    }
    default:
      break;
  }

  return QGraphicsItem::itemChange(change, value);
}

void SpectralKeyView::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = event->pos();
  update();
}

void SpectralKeyView::hoverMoveEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = event->pos();
  update();
}

void SpectralKeyView::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
  m_mousePos = std::nullopt;
  update();
}
}
