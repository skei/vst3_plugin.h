
class template_descriptor
: public VST3_Descriptor {

public:

  template_descriptor() {
    name              = "template";
    author            = "skei.audio";
    url               = "https://torhelgeskei.com";
    email             = "tor.helge.skei_at_gmail_dot_com";
    version_text      = "0.0.0";
    version           = 0;
    plugin_id[16]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    editor_id[16]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
    is_synth          = false;
    has_editor        = false;
    can_send_midi     = false;
    can_receive_midi  = false;
    editor_width      = 0;
    editor_height     = 0;
    num_inputs        = 2;
    num_outputs       = 2;
    appendParameter( new VST3_Parameter("gain",1.0f) );
  }

};

//----------

class template_instance
: public VST3_Instance {

private:

  float gain = 0.0f;

public:

  void on_create() final {}
  void on_destroy() final {}
  void on_initialize() final {}
  void on_terminate() final {}
  void on_activate() final {}
  void on_deactivate() final {}
  void on_prepare(float ASampleRate) final {}
  void on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) final {}
  void on_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) final {}
  void on_process(VST3_ProcessContext* AContext) final {}
  VST3_Editor* on_openEditor(VST3_EditorListener* AListener, VST3_Descriptor* ADescriptor, void* AParent) { return nullptr; }
  void on_closeEditor(VST3_Editor* AEditor) final {}
  void on_updateEditor(VST3_Editor* AEditor) final {}
  uint32_t on_saveState(void** ABuffer, uint32_t AMode) final { *ABuffer = nullptr; return 0; }
  void on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) final {}

};
