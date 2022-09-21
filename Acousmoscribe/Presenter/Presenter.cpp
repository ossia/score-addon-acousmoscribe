#include <score/command/Dispatchers/CommandDispatcher.hpp>

/* View */
#include <Acousmoscribe/View/View.hpp>
#include <Acousmoscribe/View/SignView.hpp>
#include <Acousmoscribe/View/SpectralKeyView.hpp>
#include <Acousmoscribe/View/MelodicKeyView.hpp>

/* Presenter */
#include <Acousmoscribe/Presenter/Presenter.hpp>

/* Commands */
#include <Acousmoscribe/Commands/AddKey.hpp>
#include <Acousmoscribe/Commands/AddSign.hpp>
#include <Acousmoscribe/Commands/RemoveSigns.hpp>
#include <Acousmoscribe/Commands/ScaleSigns.hpp>


/* Model */
#include <Acousmoscribe/Model/Process.hpp>
#include <Acousmoscribe/Model/Sign.hpp>
#include <Acousmoscribe/Model/GrainProfile.hpp>
#include <Acousmoscribe/Model/MelodicKey.hpp>
#include <Acousmoscribe/Model/SpectralKey.hpp>
#include <Acousmoscribe/Model/RhythmicProfile.hpp>
#include <Acousmoscribe/Model/MelodicProfile.hpp>

#include <Scenario/Document/ScenarioDocument/ScenarioDocumentView.hpp>

#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/math.hpp>
#include <score/tools/IdentifierGeneration.hpp>

#include <score/command/Dispatchers/CommandDispatcher.hpp>
#include <score/command/Dispatchers/MacroCommandDispatcher.hpp>
#include <score/document/DocumentContext.hpp>
#include <score/document/DocumentInterface.hpp>
#include <score/tools/Bind.hpp>
#include <score/graphics/ItemBounder.hpp>

#include <core/document/Document.hpp>
#include <core/document/DocumentModel.hpp>

#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/math.hpp>

#include <boost/range/adaptor/filtered.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>

#include <core/document/DocumentView.hpp>
#include <QAction>
#include <QApplication>
#include <QInputDialog>
#include <QMenu>


namespace Acousmoscribe
{
SoftGradient::SoftGradient(QGraphicsItem* parent)
  : QGraphicsItem{parent}
{
  this->setZValue(2);

}
static const QLinearGradient black_gradient = [] {
  QLinearGradient g;
  g.setStart(0., 0.);
  g.setFinalStop(200., 1.);
  int b = 20;

  g.setColorAt(0, QColor(b, b, b, 255));
  g.setColorAt(0.9, QColor(b, b, b, 235));
  g.setColorAt(1.0, QColor(b, b, b, 0));
  return g;
}();

void SoftGradient::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
  painter->fillRect(m_rect, black_gradient);
}

Presenter::Presenter(
    const Acousmoscribe::Model& layer, View* view,
    const Process::Context& ctx, QObject* parent)
    : Process::LayerPresenter{layer, view, ctx, parent}
    , m_model{layer}
    , m_view{view}
    , m_moveDispatcher{ctx.commandStack}
    , m_disp{ctx.commandStack}
    , m_zr{1.}
{
  putToFront();

  auto* v = static_cast<Scenario::ScenarioDocumentView*>(&ctx.document.view()->viewDelegate());
  {
    con(v->view(), &Scenario::ProcessGraphicsView::scrolled,
        this, [this] { updateKeyPosition(); });
    con(v->view(), &Scenario::ProcessGraphicsView::horizontalZoom,
        this, [this] { updateKeyPosition(); });
    con(v->view(), &Scenario::ProcessGraphicsView::visibleRectChanged,
        this, [this] { updateKeyPosition(); });
  }
  auto& model = layer;

  con(model, &Model::melodicKeysNeedUpdate, this, [&] {
    updateMelodicKey(*m_melodicKeyView);
  });

  con(model, &Model::spectralKeyNeedUpdate, this, [&] {
    updateSpectralKey(*m_spectralKeyView);
  });

  con(model, &Model::signsNeedUpdate, this, [&] {
    for (auto sign : m_signs)
      updateSign(*sign);
  });

  /*con(model, &Model::signsChanged, this, [&] {
    for (auto sign : m_signs)
    {
      delete sign;
    }

    m_signs.clear();

    for (auto& sign : model.signs)
    {
      on_signAdded(sign);
    }
  });*/

  {
    m_melodicKeyView = new MelodicKeyView{*model.melodicKey, *this, m_view};
    updateMelodicKey(*m_melodicKeyView);
  }

  {
    m_spectralKeyView = new SpectralKeyView{*model.spectralKey, *this, m_view};
    updateSpectralKey(*m_spectralKeyView);
  }

  {
    m_keyGradient = new SoftGradient{m_view};
    m_keyGradient->setRect({0, 0, 220, 50});
  }

  model.signs.added.connect<&Presenter::on_signAdded>(this);
  model.signs.removing.connect<&Presenter::on_signRemoving>(this);

  for (auto& sign : model.signs)
    on_signAdded(sign);

  assert(m_melodicKeyView);
  assert(m_spectralKeyView);

  connect(m_view, &View::doubleClicked, this, [&](QPointF pos) {
    m_disp.submit(
        new AddSign{model, m_view->signAtPos(pos)});
  });

  connect(m_view, &View::deleteRequested, this, [&] {
    m_disp.submit(
        new RemoveSigns{this->model(), selectedSigns()});
  });

  connect(m_view, &View::pressed, this, [&]() {
    m_context.context.focusDispatcher.focus(this);
    for (SignView* n : m_signs)
      n->setSelected(false);
  });

  //connect(m_view, &View::dropReceived, this, &Presenter::on_drop);

  /*connect(m_view, &View::deleteRequested, this, [&] {
    m_disp.submit(
        new RemoveSigns{this->model(), selectedSigns()});
  });*/

  connect(m_view, &View::askContextMenu, this, &Presenter::contextMenuRequested);

}

Presenter::~Presenter()
{
}

void Presenter::updateKeyPosition()
{
  if(auto [x, ok] = m_bounder.bound(m_view, 0., 100.); ok)
  {
    m_keyGradient->setPos({x, 0.});
    updateMelodicKey(*m_melodicKeyView);
    updateSpectralKey(*m_spectralKeyView);
  }
}



void Presenter::on_selectionChanged(SignView* v, bool ok)
{
 if(ok)
   m_selectedSigns.push_back(v);
 else
   ossia::remove_erase(m_selectedSigns, v);

 Selection s;
 for(auto n : m_selectedSigns)
   s.append(&n->sign);

 context().context.selectionStack.pushNewSelection(s);
}

void Presenter::setWidth(qreal val, qreal defaultWidth)
{
  m_view->setWidth(val);
  m_view->setDefaultWidth(defaultWidth);
}

void Presenter::setHeight(qreal val)
{
  m_view->setHeight(val);
  m_keyGradient->setRect({0, 0, 220, val});
  updateMelodicKey(*m_melodicKeyView);
  updateSpectralKey(*m_spectralKeyView);
  for (auto sign : m_signs)
    updateSign(*sign);
}

void Presenter::putToFront()
{
  m_view->setEnabled(true);
}

void Presenter::putBehind()
{
  m_view->setEnabled(false);
}

void Presenter::on_zoomRatioChanged(ZoomRatio zr)
{
  m_zr = zr;
  m_view->setDefaultWidth(model().duration().toPixels(m_zr));
  updateMelodicKey(*m_melodicKeyView);
  updateSpectralKey(*m_spectralKeyView);
  for (auto sign : m_signs)
    updateSign(*sign);
}

void Presenter::parentGeometryChanged()
{
}

const Acousmoscribe::Model& Presenter::model() const noexcept
{
  return static_cast<const Acousmoscribe::Model&>(m_process);
}

void Presenter::updateMelodicKey(MelodicKeyView& v)
{
  const auto keyRect = v.computeRect();
  const auto newPos = QPointF{m_bounder.x() + keyRect.topLeft().x(), keyRect.topLeft().y()};
  if (newPos != v.pos())
  {
    v.setPos(newPos);
  }

  v.setWidth(keyRect.width());
  v.setHeight(keyRect.height());
}

void Presenter::updateSpectralKey(SpectralKeyView& s)
{
  const auto keyRect = s.computeRect();
  const auto newPos = QPointF{m_bounder.x() + keyRect.topLeft().x(), keyRect.topLeft().y()};
  if (newPos != s.pos())
  {
    s.setPos(newPos);
  }

  s.setWidth(keyRect.width());
  s.setHeight(keyRect.height());
}

void Presenter::updateSign(SignView& v)
{
  //std::cout << "entré dans update sign\n";

  const auto signRect = v.computeRect();
  const auto newPos = signRect.topLeft();
  if (newPos != v.pos())
  {
    v.setPos(newPos);
  }

  v.setWidth(signRect.width());
  v.setHeight(signRect.height());
}


// Melodic Key Changed

void Presenter::on_melodicKeyPitchChanged(const MelodicKey& mKey, Pitch pitch){
  auto data = mKey.melodicKeyData();
  data.pitch = pitch;
  m_disp.submit(new ChangeMelodicKey{mKey, data});
}

void Presenter::on_melodicKeyRangeChanged(const MelodicKey& mKey, Range range){
  auto data = mKey.melodicKeyData();
  data.range = range;
  m_disp.submit(new ChangeMelodicKey{mKey, data});
}


// Spectral Key Changed

void Presenter::on_spectralKeyNatureChanged(const SpectralKey& sKey, Nature nature){
  auto data = sKey.spectralKeyData();
  data.nature = nature;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyNature2Changed(const SpectralKey& sKey, Nature nature){
  auto data = sKey.spectralKeyData();
  data.nature2 = nature;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyHybridNatureChanged(const SpectralKey& sKey, Nature nature){
  auto data = sKey.spectralKeyData();
  data.hybrid_nature = nature;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyHybridNature2Changed(const SpectralKey& sKey, Nature nature){
  auto data = sKey.spectralKeyData();
  data.hybrid_nature2 = nature;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}


void Presenter::on_spectralKeyIsRichChanged(const SpectralKey& sKey, bool isRich){
  auto data = sKey.spectralKeyData();
  data.isRich = isRich;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyIsRich2Changed(const SpectralKey& sKey, bool isRich){
  auto data = sKey.spectralKeyData();
  data.isRich2 = isRich;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyIsHybridChanged(const SpectralKey& sKey, bool isHybrid){
  auto data = sKey.spectralKeyData();
  data.isHybrid = isHybrid;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyIsHybrid2Changed(const SpectralKey& sKey, bool isHybrid){
  auto data = sKey.spectralKeyData();
  data.isHybrid2 = isHybrid;
  m_disp.submit(new ChangeSpectralKey{sKey, data});
}

void Presenter::on_spectralKeyColorationChanged(const SpectralKey& sKey, Coloration c){
  auto data =sKey.spectralKeyData();
  data.coloration = c;
  m_disp.submit(new ChangeSpectralKey{sKey,data});
}

void Presenter::on_spectralKeyColoration2Changed(const SpectralKey& sKey, Coloration c){
  auto data =sKey.spectralKeyData();
  data.coloration2 = c;
  m_disp.submit(new ChangeSpectralKey{sKey,data});
}



// ===================================================== SIGN =====================================================

void Presenter::on_signMoved(SignView& v)
{
  if(!m_origMoveStart)
  {
    m_origMoveStart = v.sign.start();
  }

  auto newPos = v.pos();
  auto rect = m_view->boundingRect();
  auto height = rect.height();

  auto signs = selectedSigns();
  auto it = ossia::find(signs, v.sign.id());
  if (it == signs.end())
  {
    signs = {v.sign.id()};
  }

  m_moveDispatcher.submit(
      model(),
      signs,
      newPos.x() / m_view->defaultWidth() - *m_origMoveStart);
}

void Presenter::on_signMoveFinished(SignView& v)
{
  on_signMoved(v);
  m_moveDispatcher.commit();

  m_origMoveStart = std::nullopt;
}

void Presenter::on_signScaled(const Sign& sign, double newScale)
{
  auto signs = selectedSigns();
  auto it = ossia::find(signs, sign.id());
  if (it == signs.end())
  {
    signs = {sign.id()};
  }

  auto dt = newScale - sign.duration();
  m_disp.submit(new ScaleSigns{model(), signs, dt});
}


// --------------------------------------------- Sign/Dynamic Profile ---------------------------------------------



void Presenter::on_signVolumeStartChanged(const Sign& sign, float newVolStart)
{
  if(sign.dynamicProfile().volumeStart != newVolStart)
  {
    auto data = sign.signData();

    data.dynamicProfile.volumeStart = newVolStart;

    m_disp.submit(new ChangeSign{sign, data});
  }
};

void Presenter::on_signVolumeEndChanged(const Sign& sign, float newVolEnd)
{
  if(sign.dynamicProfile().volumeEnd != newVolEnd)
  {
    auto data = sign.signData();

    data.dynamicProfile.volumeEnd = newVolEnd;

    m_disp.submit(new ChangeSign{sign, data});
  }
};


// --------------------------------------------- Sign/Melodic Profile ---------------------------------------------

void Presenter::on_signMelodicProfilePitchChanged(const Sign& sign, Pitch newPitch){
  if(sign.melodicProfile().pitch != newPitch)
  {
    auto data = sign.signData();
    data.melodicProfile.pitch = newPitch;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

void Presenter::on_signMelodicProfilePitchEndChanged(const Sign& sign, Pitch newPitchEnd){
  if(sign.melodicProfile().pitchEnd != newPitchEnd)
  {
    auto data = sign.signData();
    data.melodicProfile.pitchEnd = newPitchEnd;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

void Presenter::on_signMelodicProfileVariationChanged(const Sign& sign, Variation newVar){
  if(sign.melodicProfile().var != newVar)
  {
    auto data = sign.signData();
    data.melodicProfile.var = newVar;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

void Presenter::on_signMelodicProfilePaceChanged(const Sign& sign, Pace pace){

  if(sign.melodicProfile().pace != pace)
  {
    auto data = sign.signData();
    data.melodicProfile.pace = pace;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

// --------------------------------------------- Sign/Rhythmic Profile ---------------------------------------------

void Presenter::on_signRhythmicProfileSpeedChanged(const Sign& sign, Speed newSpeed)
{
  if(sign.rhythmicProfile().speed != newSpeed)
  {
    auto data = sign.signData();
    data.rhythmicProfile.speed = newSpeed ;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

void Presenter::on_signRhythmicProfileAccelerationChanged(const Sign& sign, Acceleration newAcc)
{
  if(sign.rhythmicProfile().acceleration != newAcc)
  {
    auto data = sign.signData();
    data.rhythmicProfile.acceleration = newAcc  ;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

void Presenter::on_signRhythmicProfileIsRandomChanged(const Sign& sign, Variation newIsRandom)
{
  if(sign.rhythmicProfile().variation != newIsRandom)
  {
    auto data = sign.signData();
    data.rhythmicProfile.variation = newIsRandom;

    m_disp.submit(new ChangeSign{sign, data});
  }
}

// -------------------------------------------------- Sign/Grain  --------------------------------------------------

void Presenter::on_signGrainChanged(const Sign& sign, Grain g)
{
  if(sign.grainProfile().grain != g)
  {
    auto data = sign.signData();
    data.grainProfile.grain = g;
    m_disp.submit(new ChangeSign{sign, data});
  }
}

// =================================================================================================================


void Presenter::on_signAdded(const Sign& s)
{
  auto v = new SignView{s, *this, m_view};
  updateSign(*v);
  m_signs.push_back(v);
}

void Presenter::on_signRemoving(const Sign& s)
{
  {
    auto it = ossia::find_if(m_selectedSigns, [&](const auto& other) { return &other->sign == &s; });
    if (it != m_selectedSigns.end())
    {
      m_selectedSigns.erase(it);
    }
  }

  {
    auto it = ossia::find_if(m_signs, [&](const auto& other) { return &other->sign == &s; });
    if (it != m_signs.end())
    {
      delete *it;
      m_signs.erase(it);
    }
  }
}

void Presenter::on_signDuplicate()
{

}

void Presenter::on_deselectOtherSigns()
{
  for (SignView* s : m_signs)
    s->setSelected(false);
}


std::vector<Id<Sign>> Presenter::selectedSigns() const
{
  using namespace boost::adaptors;

  std::vector<Id<Sign>> res;
  boost::copy(
      m_signs | boost::adaptors::filtered([](SignView* v) { return v->isSelected(); })
          | transformed([](SignView* v) { return v->sign.id(); }),
      std::back_inserter(res));
  return res;

}

}
