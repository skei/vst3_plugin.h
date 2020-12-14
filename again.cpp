
#include "vst3_plugin.h"

//----------------------------------------------------------------------

class myDescriptor : public VST3_Descriptor {

public:

  myDescriptor() {
    name = "vst3_plugin";
    appendParameter( new VST3_Parameter("Gain",1.0f) );
  }

};

//----------------------------------------------------------------------

class myInstance : public VST3_Instance {

private:

  float MGain = 0.0f;

public:

  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {
    switch (AIndex) {
      case 0: MGain = AValue; break;
    }
  }

  void on_process(VST3_ProcessContext* AContext) final {
    float*    in0   = AContext->inputs[0];
    float*    in1   = AContext->inputs[1];
    float*    out0  = AContext->outputs[0];
    float*    out1  = AContext->outputs[1];
    uint32_t  len   = AContext->num_samples;
    for (uint32_t i=0; i<len; i++) {
      *out0++ = *in0++ * MGain;
      *out1++ = *in1++ * MGain;
    }
  }

};

//----------------------------------------------------------------------

VST3_ENTRYPOINT(myDescriptor,myInstance);

//----------------------------------------------------------------------
