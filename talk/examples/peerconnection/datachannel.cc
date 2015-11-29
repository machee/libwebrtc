#include <iostream>
#include <string>

#include "talk/app/webrtc/jsep.h"
#include "talk/app/webrtc/test/fakeconstraints.h"
#include "talk/app/webrtc/peerconnectioninterface.h"
#include "webrtc/base/ssladapter.h"

using webrtc::PeerConnectionInterface;

class PCObserver : public webrtc::PeerConnectionObserver
{
  rtc::scoped_refptr<PeerConnectionInterface> peerConnection;
  bool _waitingForOffer = true;

  virtual void OnDataChannel(webrtc::DataChannelInterface*) {};
  virtual void OnRenegotiationNeeded() {};
  virtual void OnIceCandidate(const webrtc::IceCandidateInterface*) {};
  virtual void OnIceGatheringChange(PeerConnectionInterface::IceGatheringState);

  public:
    void setPeerConnection(rtc::scoped_refptr<PeerConnectionInterface>);
    bool waitingForOffer();
};

void PCObserver::OnIceGatheringChange(PeerConnectionInterface::IceGatheringState new_state) {
  if (new_state == PeerConnectionInterface::kIceGatheringComplete) {
    auto strDesc = std::string();
    peerConnection->local_description()->ToString(&strDesc);

    std::cout << std::endl << "=== OFFER BEGIN ===" << std::endl << strDesc.c_str();
    std::cout << "=== OFFER END ===" << std::endl << std::endl;

    _waitingForOffer = false;
  }
}

void PCObserver::setPeerConnection(rtc::scoped_refptr<PeerConnectionInterface> pc) {
  peerConnection = pc;
}

bool PCObserver::waitingForOffer() {
  return _waitingForOffer;
}


class SetSDObserver : public webrtc::SetSessionDescriptionObserver
{
  virtual void OnSuccess() {};
  virtual void OnFailure(const std::string&);
};

void SetSDObserver::OnFailure(const std::string& error) {
  std::cout << "SetSessionDescription ERROR " << error << std::endl;
}


class CreateSDObserver : public webrtc::CreateSessionDescriptionObserver
{
  rtc::scoped_refptr<PeerConnectionInterface> peerConnection;
  virtual void OnSuccess(webrtc::SessionDescriptionInterface*);
  virtual void OnFailure(const std::string&);

  public:
    CreateSDObserver(rtc::scoped_refptr<PeerConnectionInterface> connection) :
      peerConnection(connection) {};
};

void CreateSDObserver::OnSuccess(webrtc::SessionDescriptionInterface* desc) {
  auto setSDObserver = new rtc::RefCountedObject<SetSDObserver>();
  peerConnection->SetLocalDescription(setSDObserver, desc);
}

void CreateSDObserver::OnFailure(const std::string &error) {
  std::cout << "CreateSessionDescription ERROR " << error << std::endl;
}


int main(int argc, char **argv) {
  PeerConnectionInterface::IceServer iceServer;
  iceServer.uri = "stun:stun.l.google.com:19302";

  PeerConnectionInterface::IceServers iceServers;
  iceServers.push_back(iceServer);

  webrtc::FakeConstraints constraints;
  constraints.AddOptional(
    webrtc::MediaConstraintsInterface::kEnableDtlsSrtp,
    webrtc::MediaConstraintsInterface::kValueTrue
  );
  /*
  // may crash without these constraints
  constraints.AddMandatory(
    webrtc::MediaConstraintsInterface::kOfferToReceiveAudio,
    webrtc::MediaConstraintsInterface::kValueFalse
  );
  constraints.AddMandatory(
    webrtc::MediaConstraintsInterface::kOfferToReceiveVideo,
    webrtc::MediaConstraintsInterface::kValueFalse
  );
  */

  auto peerConnectionFactory = webrtc::CreatePeerConnectionFactory();

  auto peerConnectionObserver = PCObserver();

  auto peerConnection = peerConnectionFactory->CreatePeerConnection(
    iceServers, &constraints, NULL, NULL, &peerConnectionObserver
  );

  peerConnectionObserver.setPeerConnection(peerConnection);

  auto createSDObserver = new rtc::RefCountedObject<CreateSDObserver>(peerConnection);

  // This throws the below error, but must be called before CreateOffer
  //   Error(webrtcsession.cc:852): ConnectDataChannel called when data_channel_ is NULL.
  auto dataChannelInit = webrtc::DataChannelInit();
  dataChannelInit.ordered = true;
  auto dataChannelInterface = peerConnection->CreateDataChannel("test", &dataChannelInit);

  peerConnection->CreateOffer(createSDObserver, NULL);

  while(peerConnectionObserver.waitingForOffer()) {}

  peerConnection->Close();

  return 0;
}
