#pragma once
#include <Acousmoscribe/Process.hpp>
#include <Process/LayerView.hpp>

#include <QPainter>
#include <QPainterPath>

#include <Process/LayerView.hpp>

namespace Acousmoscribe
{
class SpectralKeyView;
class MelodicKeyView;
class View final : public Process::LayerView
{
    W_OBJECT(View)
public:
  explicit View(QGraphicsItem* parent);
  ~View() override;
  double defaultWidth() const noexcept { return m_defaultW; }
  void setDefaultWidth(double w);
  
  int visibleCount() const;

  std::pair<int, int> range() const { return {m_min, m_max}; }

private:
  int width_fdp = 200;
  QBrush draw;

  struct SK spectralKey();
  bool canEdit() const;
  void paint_impl(QPainter*) const override;

  void mousePressEvent(QGraphicsSceneMouseEvent*) override;
  void mouseReleaseEvent(QGraphicsSceneMouseEvent*) override;

  SpectralKeyView *m_spectralKey;
  MelodicKeyView *m_melodickey;

  QPainterPath m_selectArea;
  double m_defaultW; // Covers the [ 0; 1 ] area
  int m_min{0}, m_max{127};

  QPixmap m_textCache;
  QPixmap m_bgCache;

  mutable std::vector<QPainter::PixmapFragment> m_fragmentCache;
};
}
