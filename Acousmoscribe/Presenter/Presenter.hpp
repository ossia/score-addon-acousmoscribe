#pragma once

#include <Acousmoscribe/Model/Process.hpp>
#include <Process/Focus/FocusDispatcher.hpp>
#include <Process/LayerPresenter.hpp>

/* Commands */
#include <Acousmoscribe/Commands/ChangeMelodicKey.hpp>
#include <Acousmoscribe/Commands/ChangeSpectralKey.hpp>
#include <Acousmoscribe/Commands/MoveSigns.hpp>
#include <Acousmoscribe/Commands/ChangeSign.hpp>

#include <score/graphics/ItemBounder.hpp>
#include <score/command/Dispatchers/SingleOngoingCommandDispatcher.hpp>
#include <score/command/Dispatchers/CommandDispatcher.hpp>

#include <nano_observer.hpp>

namespace Acousmoscribe
{
class Model;
class View;
class Sign;
class SignView;
class MelodicKey;
class MelodicKeyView;
class SpectralKey;
class SpectralKeyView;

class SoftGradient : public QGraphicsItem {
public:
  explicit SoftGradient(QGraphicsItem* parent);
  QRectF boundingRect() const override { return m_rect; }
  void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

  void setRect(QRectF r)
  {
    prepareGeometryChange();
    m_rect = r;
    update();
  }
private:
  QRectF m_rect;
};

class Presenter final
    : public Process::LayerPresenter
    , public Nano::Observer
{
public:
  explicit Presenter(
      const Model& model,
      View* view,
      const Process::Context& ctx,
      QObject* parent);
  ~Presenter() override;

  void setWidth(qreal width, qreal defaultWidth) override;
  void setHeight(qreal height) override;

  void putToFront() override;
  void putBehind() override;

  void on_zoomRatioChanged(ZoomRatio) override;

  void parentGeometryChanged() override;

  const Acousmoscribe::Model& model() const noexcept;



  //void on_melodicKeyChanged(MelodicKeyView& mKey);

  void on_melodicKeyPitchChanged(const MelodicKey&, Pitch pitch);
  void on_melodicKeyRangeChanged(const MelodicKey&, Range range);


  //void on_spectralKeyChanged(SpectralKeyView& sKey);

  void on_spectralKeyNatureChanged(const SpectralKey&, Nature nature);
  void on_spectralKeyNature2Changed(const SpectralKey&, Nature nature);
  void on_spectralKeyHybridNatureChanged(const SpectralKey&, Nature nature);
  void on_spectralKeyHybridNature2Changed(const SpectralKey&, Nature nature);
  void on_spectralKeyIsHybridChanged(const SpectralKey&, bool isHybrid);
  void on_spectralKeyIsHybrid2Changed(const SpectralKey&, bool isHybrid);
  void on_spectralKeyIsRichChanged(const SpectralKey&, bool isRich);
  void on_spectralKeyIsRich2Changed(const SpectralKey&, bool isRich);
  void on_spectralKeyColorationChanged(const SpectralKey&,Coloration c);
  void on_spectralKeyColoration2Changed(const SpectralKey&,Coloration c);





  void on_deselectOtherSigns();
  void on_signDuplicate();
  void on_signScaled(const Sign&, double newScale);
  void on_signMoved(SignView& s);
  void on_signMoveFinished(SignView& s);

  // Dynamic Profile :
  void on_signVolumeStartChanged(const Sign&, float newVolStart);
  void on_signVolumeEndChanged(const Sign&, float newVolEnd);
  //void on_signVolumeChanged(const Sign&, double newVol);

  // Other profiles
  //void on_signMelodicProfileChanged(Sign& v, MelodicProfile mp);
  void on_signMelodicProfilePitchChanged(const Sign&, Pitch newPitch);
  void on_signMelodicProfilePitchEndChanged(const Sign&, Pitch newPitchEnd);
  void on_signMelodicProfileVariationChanged(const Sign&, Variation newVar);
  void on_signMelodicProfilePaceChanged(const Sign&, Pace pace);

  //void on_signRhythmicProfileChanged(Sign& v, RhythmicProfile rp);
  void on_signRhythmicProfileSpeedChanged(const Sign&, Speed newSpeed);
  void on_signRhythmicProfileAccelerationChanged(const Sign&, Acceleration newAcc);
  void on_signRhythmicProfileVariationChanged(const Sign&, Variation newIsRandom);
  void on_signGrainProfileGrainChanged(const Sign&, Grain g);
  void on_signGrainProfileVariationChanged(const Sign&, Variation var);

  void on_selectionChanged(SignView*, bool ok);


private:

  void updateKeyPosition();
  void updateSpectralKey(SpectralKeyView&);
  void on_spectralKeyAdded(const SpectralKey&);
//  void on_spectralKeyRemoving(const SpectralKey&);

  void updateMelodicKey(MelodicKeyView&);
  void on_melodicKeyAdded(const MelodicKey&);
  void on_melodicKeyRemoving(MelodicKey&);

  void updateSign(SignView&);         //done
  void on_signAdded(const Sign&);     //done
  void on_signRemoving(const Sign&);  //done

  std::vector<Id<Sign>> selectedSigns() const;            //done

  const Model& m_model;
  View* m_view{};
  MelodicKeyView* m_melodicKeyView{};
  SpectralKeyView* m_spectralKeyView{};
  SoftGradient* m_keyGradient{};
  std::vector<SignView*> m_signs;
  std::vector<SignView*> m_selectedSigns;

  /* COMMAND DISPATCHERS */
  SingleOngoingCommandDispatcher<MoveSigns> m_moveDispatcher;
  CommandDispatcher<> m_disp;


  std::optional<double> m_origMoveStart{};
  score::ItemBounder m_bounder;

  ZoomRatio m_zr{};
};
}
