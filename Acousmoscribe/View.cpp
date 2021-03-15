#include "View.hpp"

#include <Process/Style/ScenarioStyle.hpp>

#include <score/graphics/GraphicsItem.hpp>

#include "Inspector/Inspector.hpp"

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPainter>
#include <QPushButton>

QPushButton *r_hybrid;

namespace Acousmoscribe
{
View::View(QGraphicsItem* parent) : Process::LayerView{parent}
{
  this->setAcceptHoverEvents(true);
  this->setAcceptDrops(true);
  this->setFlag(QGraphicsItem::ItemIsFocusable, true);
  this->setFlag(QGraphicsItem::ItemClipsToShape, true);

  m_fragmentCache.reserve(20);
}

View::~View()
{
}

bool View::canEdit() const
{
  const auto rect = boundingRect();
  const auto note_height = rect.height() / (visibleCount());
  return note_height > 5;
}

void View::paint_impl(QPainter* p) const
{
  p->drawRect(30, 50, 30, 50);

  /* rectangle/triangle */
  static const QPointF points[4] = {
     QPointF(10.0, 80.0), //bas gauche
     QPointF(10.0, 10.0), //haut gauche
     QPointF(50.0, 10.0), //haut droite
     QPointF(90.0, 80.0),  //bas droite
 };

 static const QPointF points_signe[4] = {
     QPointF(25.0, 25.0), //bas gauche
     QPointF(35.0, 35.0), //haut gauche
     QPointF(45.0, 25.0), //haut droite
     QPointF(55.0, 35.0),  //bas droite
 };
 
 p->drawPolygon(points, 4);
 p->drawPolygon(points_signe, 4);
 //connect(r_hybrid, SIGNAL(toggled(bool)), p, SLOT(setVisible(bool)));
}

int View::visibleCount() const
{
  return m_max - m_min + 1;
}
}
