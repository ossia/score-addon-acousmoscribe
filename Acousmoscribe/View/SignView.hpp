#pragma once
#include <Acousmoscribe/View/View.hpp>
#include "Acousmoscribe/Model/Sign.hpp"
#include <Acousmoscribe/Presenter/Presenter.hpp>
#include <QGraphicsItem>

namespace Acousmoscribe
{
class View;
class Presenter;
class Sign;
class SignView final
    : public QGraphicsItem
{
  Q_INTERFACES(QGraphicsItem)

public:
  const Sign& sign;
  SignView(const Sign& n, Presenter& presenter, View* parent);

  void setWidth(qreal w) noexcept
  {
    if (m_width != w)
    {
      prepareGeometryChange();
      m_width = w;
    }
  }

  void setHeight(qreal h) noexcept
  {
    if (m_height != h)
    {
      prepareGeometryChange();
      m_height = h;
    }
  }

  QRectF boundingRect() const override { return {0, 0, m_width, m_height}; }
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  QRectF computeRect() const noexcept;
  QPointF closestPos(QPointF sign) const noexcept;

private:
  bool canEdit() const;
  QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;
  void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
  void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
  void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

  bool contains(const QPointF& point) const override;
  QPainterPath shape() const override;
  void drawVariationLine(QPainter* painter,QLineF line, bool down = true) const;

  QRectF changeGrainProfileGrainRect() const noexcept;
  QRectF changeGrainProfileVariationRect() const noexcept;
  QRectF changeRhythmicProfileSpeedRect() const noexcept;
  QRectF changeRhythmicProfileAccelerationRect() const noexcept;
  QRectF changeRhythmicProfileVariationRect() const noexcept;
  QRectF changeMelodicProfilePitchRect() const noexcept;
  QRectF changeMelodicProfilePitchEndRect() const noexcept;
  QRectF changeMelodicProfileVolumeStartRect() const noexcept;
  QRectF changeMelodicProfileVolumeEndRect() const noexcept;
  QRectF changeMelodicProfilePaceRect() const noexcept;
  QRectF changeMelodicProfileVariationRect() const noexcept;


  QPolygonF polygon() const noexcept;

  Presenter& m_presenter;

  float m_width{};
  float m_height{};
  std::optional<QPointF> m_mousePos;

  enum Action {
    None,
    Move,
    Scale,
    ChangeSignVolumeStart,
    ChangeSignVolumeEnd,
    ChangeMelodicProfilePitch,
    ChangeMelodicProfilePitchEnd,
    ChangeMelodicProfileVariation,
    ChangeMelodicProfilePace,
    ChangeRhythmicProfileSpeed,
    ChangeRhythmicProfileAcceleration,
    ChangeRhythmicProfileVariation,
    ChangeGrainProfileGrain,
    ChangeGrainProfileVar,
    ChangeSpeed,
    Duplicate
  } m_action{};
};
}
