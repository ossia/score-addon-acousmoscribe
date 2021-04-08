#pragma once

#include <Acousmoscribe/Process.hpp>
#include <Process/Focus/FocusDispatcher.hpp>
#include <Process/LayerPresenter.hpp>

/* Commands */
#include <Acousmoscribe/Commands/ChangeMelodicKey.hpp>
#include <Acousmoscribe/Commands/ChangeSpectralKey.hpp>
#include <Acousmoscribe/Commands/MoveSigns.hpp>
#include <Acousmoscribe/Commands/ChangeSign.hpp>


#include <score/command/Dispatchers/SingleOngoingCommandDispatcher.hpp>

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

class Presenter final : public Process::LayerPresenter
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
  void on_spectralKeyIsRichChanged(const SpectralKey&, bool isRich);
  void on_spectralKeyIsRich2Changed(const SpectralKey&, bool isRich);
  void on_spectralKeyWarpedChanged(const SpectralKey&, bool warped);
  void on_spectralKeyWarped2Changed(const SpectralKey&, bool warped);
  

  
  void on_deselectOtherSigns();                         //done
  void on_signDuplicate();
  void on_signScaled(const Sign&, double newScale);     //done
  void on_signMoved(SignView& s);                       //done
  void on_signMoveFinished(SignView& s);                //done

  // Dynamic Profile :
  void on_signAttackChanged(const Sign&, double newAttack);               //done
  void on_signReleaseChanged(const Sign&, double newRelease);             //done
  void on_signVolumeInChanged(const Sign&, double newVolIn);              //done
  void on_signVolumeOutChanged(const Sign&, double newVolOut);            //done
  //void on_signVolumeChanged(const Sign&, double newVol);

  // Other profiles
  //void on_signMelodicProfileChanged(Sign& v, MelodicProfile mp);
  void on_signMelodicProfilePitchChanged(const Sign&, Pitch newPitch);                //done
  void on_signMelodicProfilePitchEndChanged(const Sign&, Pitch newPitchEnd);                //done
  void on_signMelodicProfileVariationChanged(const Sign&, Variation newVar);          //done

  //void on_signRhythmicProfileChanged(Sign& v, RhythmicProfile rp);
  void on_signRhythmicProfileSpeedChanged(const Sign&, Speed newSpeed);               //done
  void on_signRhythmicProfileAccelerationChanged(const Sign&, Acceleration newAcc);   //done
  void on_signRhythmicProfileIsRandomChanged(const Sign&, bool newIsRandom);          //done

  void on_signGrainChanged(const Sign&, Grain g);                                     //done


private:  

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
  MelodicKeyView* m_melodicKeyView;
  SpectralKeyView* m_spectralKeyView;
  std::vector<SignView*> m_signs;

  /* COMMAND DISPATCHERS */
  SingleOngoingCommandDispatcher<ChangeMelodicKeyPitch> m_changeMelodicKeyPitch;
  SingleOngoingCommandDispatcher<ChangeMelodicKeyRange> m_changeMelodicKeyRange;

  SingleOngoingCommandDispatcher<ChangeSpectralKeyNature> m_changeSpectralKeyNature;
  SingleOngoingCommandDispatcher<ChangeSpectralKeyNature2> m_changeSpectralKeyNature2;
  SingleOngoingCommandDispatcher<ChangeSpectralKeyIsRich> m_changeSpectralKeyIsRich;
  SingleOngoingCommandDispatcher<ChangeSpectralKeyIsRich2> m_changeSpectralKeyIsRich2;
  SingleOngoingCommandDispatcher<ChangeSpectralKeyWarped> m_changeSpectralKeyWarped;
  SingleOngoingCommandDispatcher<ChangeSpectralKeyWarped2> m_changeSpectralKeyWarped2;

  SingleOngoingCommandDispatcher<MoveSigns> m_moveDispatcher;

  std::optional<double> m_origMoveStart{};

  ZoomRatio m_zr{};
};
}
