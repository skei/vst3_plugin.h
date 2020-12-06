#ifndef vst3_plugin_included
#define vst3_plugin_included
//----------------------------------------------------------------------

/*
  Copyright 2020 Tor-Helge Skei

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/

//----------------------------------------------------------------------

/*
  vst3_plugin
  v0.0.0
  license: MIT
  tor.helge.skei@gmail.com
  https://torhelgeskei.com
*/

//----------------------------------------------------------------------

#define VST3_NO_GUI

//----------------------------------------------------------------------
//
//
//
//----------------------------------------------------------------------

//#ifdef __gnu_linux__
//  #define VST3_LINUX
//#endif
//
//#ifdef _WIN32
//  #define VST3_WIN32
//#endif

#include <memory.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;

#include "vst3.h"

//----------------------------------------------------------------------

#define MIDI_NOTE_OFF               0x80
#define MIDI_NOTE_ON                0x90
#define MIDI_POLY_AFTERTOUCH        0xA0
#define MIDI_CONTROL_CHANGE         0xB0
#define MIDI_PROGRAM_CHANGE         0xC0
#define MIDI_CHANNEL_AFTERTOUCH     0xD0
#define MIDI_PITCHBEND              0xE0
#define MIDI_SYS                    0xF0

#define PLUGIN_PLAYSTATE_NONE       0
#define PLUGIN_PLAYSTATE_PLAYING    1
#define PLUGIN_PLAYSTATE_RECORDING  2
#define PLUGIN_PLAYSTATE_LOOPING    4

#define VST3_MAX_INPUTS             8
#define VST3_MAX_OUTPUTS            8
#define VST3_PARAM_AFTERTOUCH       0x10000 // kAfterTouch (128)
#define VST3_PARAM_PITCHBEND        0x20000 // kPitchBend (129)
#define VST3_PARAM_BRIGHTNESS       0x30000 // kCtrlFilterResonance (74)
#define VST3_QUEUE_SIZE             1024
#define VST3_TIMER_MS               30

//----------------------------------------------------------------------
//
// queue
//
//----------------------------------------------------------------------

template<class TYPE, int SIZE>
class VST3_Queue {

private:

  TYPE*     MBuffer     = nullptr;
  uint32_t  MBufferSize = 0;
  uint32_t  MBufferMask = 0;
  uint32_t  MReadPos    = 0;
  uint32_t  MWritePos   = 0;

public:

  VST3_Queue() {
    MBuffer     = (TYPE*)malloc(SIZE*sizeof(TYPE));
    MBufferSize = SIZE;
    MBufferMask = SIZE - 1;
    MReadPos    = 0;
    MWritePos   = 0;
  }

  ~VST3_Queue() {
    free(MBuffer);
  }

  bool read(TYPE* AItem) {
    uint32_t wp = MWritePos;
    uint32_t rp = MReadPos;
    if (( (wp-rp) & MBufferMask ) > 0) {
      *AItem = MBuffer[rp];
      uint32_t next = (rp+1) & MBufferMask;
      MReadPos = next;
      return true;
    }
    return false;
  }

  bool write(TYPE AItem) {
    uint32_t rp = MReadPos;
    uint32_t wp = MWritePos;
    if (((wp-rp) & MBufferMask) < (MBufferSize-1)) {
      MBuffer[wp] = AItem;
      uint32_t next = (wp+1) & MBufferMask;
      MWritePos = next;
      return true;
    }
    return false;
  }

};

//----------

typedef VST3_Queue<uint32_t,VST3_QUEUE_SIZE> VST3_UpdateQueue;

//----------------------------------------------------------------------
//
// parameter
//
//----------------------------------------------------------------------

class VST3_Parameter {

public:

  const char* name          = "parameter";
  const char* short_name    = "param";
  const char* label         = "";
  uint32_t    num_steps     = 0;
  float       def_value     = 0.0f;
  bool        can_automate  = true;

public:

  VST3_Parameter(const char* AName, float AValue) {
    name = AName;
    def_value = AValue;
  }

  virtual ~VST3_Parameter() {
  }

public:

  virtual float from01(float AValue) {
    return AValue;
  }

  virtual float to01(float AValue) {
    return AValue;
  }

  virtual const char* displayText(float AValue, char* ABuffer) {
    sprintf(ABuffer,"%.3f",AValue);
    return ABuffer;
  }

};

//----------

typedef vector<VST3_Parameter*> VST3_Parameters;

//----------------------------------------------------------------------
//
// descriptor
//
//----------------------------------------------------------------------

class VST3_Descriptor {

public:

  const char*     name              = "VST3_Plugin_test";
  const char*     author            = "skei.audio";
  const char*     url               = "https://website.com";
  const char*     email             = "mail@email.com";
  const char*     version_text      = "0.0.0";
  uint32_t        version           = 0;
  const uint8_t   plugin_id[16]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  const uint8_t   editor_id[16]     = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
  bool            is_synth          = false;
  bool            has_editor        = false;
  bool            can_send_midi     = false;
  bool            can_receive_midi  = false;
  uint32_t        editor_width      = 0;
  uint32_t        editor_height     = 0;
  uint32_t        num_inputs        = 2;
  uint32_t        num_outputs       = 2;
  VST3_Parameters  parameters;

public:

  ~VST3_Descriptor() {
    deleteParameters();
  }

  void appendParameter(VST3_Parameter* AParameter) {
    parameters.push_back(AParameter);
  }

  void deleteParameters() {
    for (uint32_t i=0; i<parameters.size(); i++) {
      delete parameters[i];
    }
  }

};

//----------------------------------------------------------------------
//
// editor
//
//----------------------------------------------------------------------

class VST3_EditorListener {
public:
  virtual void updateParameterFromEditor(uint32_t AIndex, float AValue) {}
};

//----------

class VST3_Editor {

protected:

  VST3_Descriptor*      MDescriptor = nullptr;
  VST3_EditorListener*  MListener   = nullptr;

public:

  VST3_Editor(VST3_EditorListener* AListener, VST3_Descriptor* ADescriptor, void* AParent) {
    MDescriptor = ADescriptor;
    MListener = AListener;
  }

  virtual ~VST3_Editor() {
  }

public:

  virtual void updateParameterFromHost(uint32_t AIndex, float AValue) {
    // 1. parameter -> widget
    // 2. widget.update
    // 3. redraw
  }

  virtual void updateParameterFromGui(uint32_t AIndex, float AValue) {
    if (MListener) {
      MListener->updateParameterFromEditor(AIndex,AValue);
    }
  }

};

//----------------------------------------------------------------------
//
// process context
//
//----------------------------------------------------------------------

struct VST3_ProcessContext {
  uint32_t  num_inputs                = 0;
  uint32_t  num_outputs               = 0;
  uint32_t  num_samples               = 0;
  float*    inputs[VST3_MAX_INPUTS]   = {0};
  float*    outputs[VST3_MAX_OUTPUTS] = {0};
  float     sample_rate               = 0.0f;
  uint32_t  block_size                = 0;
  float     tempo                     = 0.0f;
  uint32_t  sample_pos                = 0;
  float     beat_pos                  = 0.0;
  uint32_t  timesig_num               = 0;
  uint32_t  timesig_denom             = 0;
  uint32_t  play_state                = 0;
};

//----------------------------------------------------------------------
//
// instance
//
//----------------------------------------------------------------------

class VST3_Instance

: public IComponent
, public IAudioProcessor
, public IUnitInfo
, public IConnectionPoint
, public IMidiMapping
, public IKeyswitchController
//, public INoteExpressionController
, public IEditController
, public IEditController2
, public IPlugView
, public ITimerHandler {

private:

  uint32_t                  MRefCount           = 1;
  IComponentHandler*   MComponentHandler   = nullptr;
  IComponentHandler2*  MComponentHandler2  = nullptr;
  IPlugFrame*          MPlugFrame          = nullptr;
  IHostApplication*    MHostApp            = nullptr;
  ParameterInfo*            MParamInfos         = nullptr;
  IRunLoop*            MRunLoop            = nullptr;
  uint32_t                  MIoMode             = 0;
  uint32_t                  MProcessMode        = 0;
  uint32_t                  MSampleSize         = 0;
  uint32_t                  MBlockSize          = 0;
  float                     MSampleRate         = 0.0f;
  bool                      MIsProcessing       = false;
  char                      MHostName[129]      = {0};
  VST3_Descriptor*          MDescriptor           = nullptr;
  VST3_Editor*              MEditor               = nullptr;
  float*                    MParameterValues      = nullptr;
  float*                    MHostParameterValues  = nullptr;
  VST3_UpdateQueue          MHostParameterQueue;

public:

  VST3_Instance() {
    printf("VST3_Instance\n");
    MRefCount   = 1;
  }

  virtual ~VST3_Instance() {
    printf("~VST3_Instance\n");
    _deleteParameterInfo();
    _destroyParameterBuffers();
  }

public:

  virtual void          on_create() {}
  virtual void          on_destroy() {}
  virtual void          on_initialize() {}
  virtual void          on_terminate() {}
  virtual void          on_activate() {}
  virtual void          on_deactivate() {}
  virtual void          on_prepare(float ASampleRate) {}
  virtual void          on_parameter(uint32_t AIndex, float AValue, uint32_t AMode) {}
  virtual void          on_midi(uint32_t AOffset, uint8_t AMsg1, uint8_t AMsg2, uint8_t AMsg3) {}
  virtual void          on_process(VST3_ProcessContext* AContext) {}
  virtual VST3_Editor*  on_openEditor(void* AParent) { return nullptr; }
  virtual void          on_closeEditor(VST3_Editor* AEditor) {}
  virtual void          on_updateEditor(VST3_Editor* AEditor) {}
  virtual uint32_t      on_saveState(void** ABuffer, uint32_t AMode) { *ABuffer = nullptr; return 0; }
  virtual void          on_restoreState(uint32_t ASize, void* APointer, uint32_t AMode) {}

public:

  void _setDescriptor(VST3_Descriptor* ADescriptor) {
    MDescriptor = ADescriptor;
    _createParameterBuffers();
    _createParameterInfo();
  }

  void _setDefaultParameterValues() {
    uint32_t num = MDescriptor->parameters.size();
    for (uint32_t i=0; i<num; i++) {
      VST3_Parameter* parameter = MDescriptor->parameters[i];
      float value = parameter->def_value;
      MParameterValues[i] = value;
    }
  }

  void _updateAllParameters() {
    uint32_t num = MDescriptor->parameters.size();
    for (uint32_t i=0; i<num; i++) {
      float v = MParameterValues[i];
      VST3_Parameter* parameter = MDescriptor->parameters[i];
      v = parameter->from01(v);
      on_parameter(i,v,0);
    }
  }

private:

  void _createParameterBuffers() {
    uint32_t num = MDescriptor->parameters.size();
    MParameterValues = (float*)malloc(num * sizeof(float));
    MHostParameterValues = (float*)malloc(num * sizeof(float));
    memset(MParameterValues,0,num * sizeof(float));
    memset(MHostParameterValues,0,num * sizeof(float));
  }

  void _destroyParameterBuffers() {
    if (MParameterValues) free(MParameterValues);
    if (MHostParameterValues) free(MHostParameterValues);
  }

  void _queueParameterToHost(uint32_t AIndex, float AValue) {
    MHostParameterValues[AIndex] = AValue;
    MHostParameterQueue.write(AIndex);
  }

  void _flushParametersToHost() {
    uint32_t index = 0;
    while (MHostParameterQueue.read(&index)) {
      float value = MHostParameterValues[index];
      //host_updateParameter(index,value);
      if (MComponentHandler) {
        //if (MComponentHandler2) MComponentHandler2->startGroupEdit();
        MComponentHandler->beginEdit(index);          // click
        MComponentHandler->performEdit(index,value);  // drag
        MComponentHandler->endEdit(index);            // release
        //if (MComponentHandler2) MComponentHandler2->finishGroupEdit();
      }
    }
  }

  void _setParameterFromEditor(uint32_t AIndex, float AValue) {
    _queueParameterToHost(AIndex,AValue);
  }

  void _createParameterInfo() {
    if (!MParamInfos) {
      uint32_t num = MDescriptor->parameters.size();
      MParamInfos = (ParameterInfo*)malloc( num * sizeof(ParameterInfo) );
      for (uint32_t i=0; i<num; i++) {
        VST3_Parameter* param = MDescriptor->parameters[i];
        MParamInfos[i].id = i;
        /*CharToUtf16*/ char_to_utf16(param->name,&MParamInfos[i].title);
        /*CharToUtf16*/ char_to_utf16(param->short_name,&MParamInfos[i].shortTitle);
        /*CharToUtf16*/ char_to_utf16(param->label,&MParamInfos[i].units);
        MParamInfos[i].stepCount = param->num_steps;
        MParamInfos[i].defaultNormalizedValue = param->def_value;
        MParamInfos[i].unitId = kRootUnitId; //-1;
        int32_t flags = 0;
        if (param->can_automate) flags += ParameterInfo::kCanAutomate;
        else flags += ParameterInfo::kIsReadOnly; // ??
        MParamInfos[i].flags = flags;
      }
    }
  }

  void _deleteParameterInfo() {
    if (MParamInfos) free(MParamInfos);
  }

  void _handleParametersInProcess(ProcessData& data) {
    IParameterChanges* paramChanges = data.inputParameterChanges;
    if (paramChanges) {
      int32_t num_param_changes = paramChanges->getParameterCount();
      if (num_param_changes > 0) {
        for (int32_t i=0; i<num_param_changes; i++) {
          IParamValueQueue* paramQueue = paramChanges->getParameterData(i);
          if (paramQueue) {
            uint32_t id = paramQueue->getParameterId();
            if (id < MDescriptor->parameters.size()) {
              //for (int32_t j=0; j<paramQueue->getPointCount(); j++) {
                int32_t offset = 0;
                double value = 0;
                int32_t pointcount = paramQueue->getPointCount();
                paramQueue->getPoint(pointcount-1,offset,value); // last point
                VST3_Parameter* param = MDescriptor->parameters[id];
                if (param) value = param->from01(value);
                on_parameter(id,value,0);
              //}
            } // id < param
            else {
              // if PLUGIN_RECEIVE_MIDI
              // IMidiMapping
              int32_t offset = 0;
              double value = 0.5;
              int32_t pointcount = paramQueue->getPointCount();
              paramQueue->getPoint(pointcount-1,offset,value);      // last point
              uint32_t midi_ev = (id & 0xffff0000);
              uint32_t midi_ch = (id & 0x0000ffff);
              switch(midi_ev) {
                case VST3_PARAM_AFTERTOUCH: {
                  //if (offset != 0)
                  on_midi(offset,MIDI_CHANNEL_AFTERTOUCH+midi_ch,value*127.0f,0);
                  break;
                } // at
                case VST3_PARAM_PITCHBEND: {
                  float v2 = value * 16383.0f;
                  uint32_t i2 = (uint32_t)v2;
                  //if (midi_ch != 0) {
                    uint32_t m2 = i2 & 0x7f;
                    uint32_t m3 = i2 >> 7;
                    //if (offset != 0)
                    on_midi(offset,MIDI_PITCHBEND+midi_ch,m2,m3);
                  //}
                  break;
                } // pb
                case VST3_PARAM_BRIGHTNESS: {
                  //if (offset != 0)
                  on_midi(offset,MIDI_CONTROL_CHANGE+midi_ch,74,value*127.0f);
                  break;
                }
              } // switch (midi_ev)
            } // id < num params
          } // paramqueue
        } // for all params
      } // numparams > 0
    } // paramchanges
  }

  void _handleEventsInProcess(ProcessData& data) {
    //if PLUGIN_RECEIVE_MIDI
    IEventList* inputEvents = data.inputEvents;
    if (inputEvents) {
      int32_t num = inputEvents->getEventCount();
      for (int32_t i=0; i<num; i++) {
        Event event;
        inputEvents->getEvent(i,event);
        uint32_t offset  = 0;
        uint8_t  msg1    = 0;
        uint8_t  msg2    = 0;
        uint8_t  msg3    = 0;
        //uint32_t type    = 0;//kve_none;
        //uint32_t noteid  = 0;
        //float    value   = 0.0f;
        switch (event.type) {
          case Event::kNoteOnEvent:
            offset  = event.sampleOffset;
            msg1    = MIDI_NOTE_ON + event.noteOn.channel;
            msg2    = event.noteOn.pitch;
            msg3    = event.noteOn.velocity * 127;
            //noteid  = event.noteOn.noteId;
            on_midi(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kNoteOffEvent:
            offset  = event.sampleOffset;
            msg1    = MIDI_NOTE_OFF + event.noteOff.channel;
            msg2    = event.noteOff.pitch;
            msg3    = event.noteOff.velocity * 127;
            //noteid  = event.noteOff.noteId;
            on_midi(offset,msg1,msg2,msg3);
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kDataEvent:
            break;
          case Event::kPolyPressureEvent:
            offset  = event.sampleOffset;
            msg1    = MIDI_POLY_AFTERTOUCH + event.polyPressure.channel;
            msg2    = event.polyPressure.pitch;
            msg3    = event.polyPressure.pressure * 127;
            //noteid  = event.polyPressure.noteId;
            on_midi(offset,msg1,msg2,msg3);
            break;
          case Event::kNoteExpressionValueEvent:
            //offset = event.sampleOffset;
            //noteid = event.noteExpressionValue.noteId;
            //value  = event.noteExpressionValue.value;
            //switch(event.noteExpressionValue.typeId) {
            //  case kTuningTypeID:     type = kve_bend; break;
            //  case kBrightnessTypeID: type = kve_slide; break;
            //  //case kVolumeTypeID:     type = kve_none;  break;
            //  //case kPanTypeID:        type = kve_none;  break;
            //  //case kVibratoTypeID:    type = kve_none;  break;
            //  //case kExpressionTypeID: type = kve_none;  break;
            //}polyPressure
            //on_noteExpression(offset,type,noteid,value);
            break;
          case Event::kNoteExpressionTextEvent:
            break;
          case Event::kChordEvent:
            break;
          case Event::kScaleEvent:
            break;
          default:
            break;
        } // switch
      } // for all events
    } // if input events
  }

public:

  //--------------------
  // FUnknown
  //--------------------

  uint32 PLUGIN_API addRef() final {
    printf("VST3_Instance.addRef\n");
    MRefCount++;
    return MRefCount;
  }

  uint32 PLUGIN_API release() final {
    printf("VST3_Instance.release\n");
    const uint32 r = --MRefCount; // const uint32 ?
    if (r == 0) {
      on_destroy();
      delete this;
    };
    return r;
  }

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) final {
    printf("VST3_Instance.queryInterface\n");
    *obj = nullptr;
    if ( /*FUnknownPrivate::*/iidEqual(IAudioProcessor_iid,_iid) ) {
      *obj = (IAudioProcessor*)this;
      addRef();
      return kResultOk;
    }
    if ( /*FUnknownPrivate::*/iidEqual(IEditController_iid,_iid) ) {
      *obj = (IEditController*)this;
      addRef();
      return kResultOk;
    }
    if ( /*FUnknownPrivate::*/iidEqual(IMidiMapping_iid,_iid) ) {
      *obj = (IMidiMapping*)this;
      addRef();
      return kResultOk;
    }
    if ( /*FUnknownPrivate::*/iidEqual(IUnitInfo_iid,_iid) ) {
      *obj = (IUnitInfo*)this;
      addRef();
      return kResultOk;
    }
//    if ( /*FUnknownPrivate::*/iidEqual(INoteExpressionController_iid,_iid) ) {
//      //*obj = (INoteExpressionController*)this;
//      //addRef();
//      //return kResultOk;
//      return kNoInterface;
//    }
    if ( /*FUnknownPrivate::*/iidEqual(IKeyswitchController_iid,_iid) ) {
      //*obj = (IKeyswitchController*)this;
      //addRef();
      //return kResultOk;
      return kNoInterface;
    }
    if ( /*FUnknownPrivate::*/iidEqual(IConnectionPoint_iid,_iid) ) {
      *obj = (IConnectionPoint*)this;
      addRef();
      return kResultOk;
    }
    //if ( /*FUnknownPrivate::*/iidEqual(ITimerHandler_iid,_iid) ) {
    //  *obj = (ITimerHandler*)this;
    //  addRef();
    //  return kResultOk;
    //}
    return kNoInterface;
  }

  //--------------------
  // IPluginBase
  //--------------------

  tresult PLUGIN_API initialize(FUnknown* context) final {
    printf("VST3_Instance.initialize\n");
    MHostApp = (IHostApplication*)context;
    //context->queryInterface(IHostApplication_iid, (void**)&MHostApp);
    if (MHostApp) {
      String128 u;
      MHostApp->getName(u);
      utf16_to_char(&u,MHostName);
    }
    else {
    }
    on_initialize();
    return kResultOk;
  }

  tresult PLUGIN_API terminate() final {
    printf("VST3_Instance.terminate\n");
    on_terminate();
    return kResultOk;
  }

  tresult PLUGIN_API getControllerClassId(TUID classId) final {
    printf("VST3_Instance.getControllerClassId\n");
    if (MDescriptor->has_editor) {
      memcpy(classId,MDescriptor->editor_id,16);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  tresult PLUGIN_API setIoMode(IoMode mode) final {
    printf("VST3_Instance.setIoMode\n");
    MIoMode = mode;
    return kResultOk;
  }

  int32 PLUGIN_API getBusCount(MediaType type, BusDirection dir) final {
    if (type == kAudio) {
      if ((dir == kOutput) && (MDescriptor->num_outputs > 0)) { return 1; }
      if ((dir == kInput)  && (MDescriptor->num_inputs  > 0)) { return 1; };
    }
    if (type == kEvent) {
      //if (dir==kOutput) return 1;
      //else
      if (MDescriptor->can_receive_midi || MDescriptor->is_synth) {
        if (dir == kInput) { return 1; }
      }
    }
    return 0;
  }

  tresult PLUGIN_API getBusInfo(MediaType type, BusDirection dir, int32 index, BusInfo& bus) final {
    if (type == kAudio) {
      bus.mediaType = kAudio;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = MDescriptor->num_inputs;
        /*CharToUtf16*/ char_to_utf16("Audio In",&bus.name);
      }
      else {
        bus.direction = kOutput;
        bus.channelCount = MDescriptor->num_outputs;
        /*CharToUtf16*/ char_to_utf16("Audio Out",&bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }
    else if (type == kEvent) {
      bus.mediaType = kEvent;
      if (dir == kInput) {
        bus.direction = kInput;
        bus.channelCount = 1;
        /*CharToUtf16*/ char_to_utf16("Midi In",&bus.name);
      }
      bus.flags = 0;//kDefaultActive;
      return kResultOk;
    }

    return kResultFalse;
  }

  tresult PLUGIN_API getRoutingInfo(RoutingInfo& inInfo, RoutingInfo& outInfo) final {
    outInfo.mediaType = inInfo.mediaType; // MediaTypes::kAudio;
    outInfo.busIndex  = inInfo.busIndex;  // 0;
    outInfo.channel   = -1;
    return kResultOk;
  }

  tresult PLUGIN_API activateBus(MediaType type, BusDirection dir, int32 index, TBool state) final {
    return kResultOk;
  }

  tresult PLUGIN_API setActive(TBool state) final {
    if (state) on_activate();
    else on_deactivate();
    return kResultOk;
  }

  tresult PLUGIN_API setState(IBStream* state) final {
    uint32_t  version = 0;
    uint32_t  mode = 0;
    int32_t   size = 0;
    uint32_t  num_params = 0;
    int32_t   num_read = 0;
    state->read(&version, sizeof(uint32_t),&num_read);
    state->read(&mode, sizeof(uint32_t),&num_read);
    state->read(&size, sizeof(int32_t),&num_read);
    switch (mode) {
      case 0: {
        // is it safe to malloc/free here?
        // use static, pre malloc'd buffer?
        void* ptr = malloc(size);
        state->read(&ptr,size,&num_read);
        on_restoreState(size,ptr,0);
        free(ptr);
        break;
      }
      case 1: {
        num_params = MDescriptor->parameters.size();
        for (uint32_t i=0; i<num_params; i++) {
          float v = 0.f;
          state->read(&v,sizeof(float),&num_read);
          MParameterValues[i] = v;
          //on_parameter(i,v,0);
        }
        _updateAllParameters();
        break;
      }
    }
    return kResultOk;
  }

  tresult PLUGIN_API getState(IBStream* state) final {
    uint32_t  version = MDescriptor->version;
    uint32_t  mode    = 0;
    void*     ptr     = nullptr;
    uint32_t  size    = 0;;
    size = on_saveState(&ptr,0);
    if ((size == 0) && (ptr == nullptr)) {
      ptr = MParameterValues;
      size = MDescriptor->parameters.size() * sizeof(float);
      mode = 1;
    }
    int num_written  = 0;
    state->write(&version,sizeof(uint32_t), &num_written);
    state->write(&mode,sizeof(uint32_t), &num_written);
    state->write(&size,sizeof(uint32_t), &num_written);
    state->write(ptr,size,&num_written);
    return kResultOk;
  }

  //--------------------
  // IAudioProcessor
  //--------------------

  tresult PLUGIN_API setBusArrangements(SpeakerArrangement* inputs, int32 numIns, SpeakerArrangement* outputs, int32 numOuts) final {
    return kResultTrue;
  }

  tresult PLUGIN_API getBusArrangement(BusDirection dir, int32 index, SpeakerArrangement& arr) final {
    if ((dir==kOutput) && (index==0)) {
      arr = kSpeakerL | kSpeakerR;
      return kResultOk;
    }
    return kResultFalse;
  }

  tresult PLUGIN_API canProcessSampleSize(int32 symbolicSampleSize) final {
    if (symbolicSampleSize==kSample32) {
      return kResultTrue;
    }
    return kResultFalse;
  }

  uint32 PLUGIN_API getLatencySamples() final {
    return 0;
  }

  tresult PLUGIN_API setupProcessing(ProcessSetup& setup) final {
    MProcessMode  = setup.processMode;        // kRealtime, kPrefetch, kOffline
    MSampleSize   = setup.symbolicSampleSize; // kSample32, kSample64
    MBlockSize    = setup.maxSamplesPerBlock;
    MSampleRate   = setup.sampleRate;
    on_prepare(MSampleRate);
    return kResultOk;
  }

  tresult PLUGIN_API setProcessing(TBool state) final {
    MIsProcessing = state;
    // if (MIsProcessing) on_prepare(MSampleRate);
    return kResultOk;
  }

  tresult PLUGIN_API process(ProcessData& data) final {
    _handleEventsInProcess(data);
    _handleParametersInProcess(data);
    bool _flush = ( (data.numInputs == 0) && (data.numOutputs == 0) && (data.numSamples == 0) );
    if (!_flush) {
      VST3_ProcessContext context;// = {0};
      uint32_t i;
      for (i=0; i<MDescriptor->num_inputs; i++)   { context.inputs[i]  = data.inputs[0].channelBuffers32[i]; }
      for (i=0; i<MDescriptor->num_outputs; i++)  { context.outputs[i] = data.outputs[0].channelBuffers32[i]; }
      context.num_inputs    = MDescriptor->num_inputs;
      context.num_outputs   = MDescriptor->num_outputs;
      context.num_samples   = data.numSamples;
      //context.oversample    = 1;
      context.sample_rate   = data.processContext->sampleRate;
      context.sample_pos    = data.processContext->continousTimeSamples;
      context.beat_pos      = data.processContext->projectTimeMusic;
      context.tempo         = data.processContext->tempo;
      context.timesig_num   = data.processContext->timeSigNumerator;
      context.timesig_denom = data.processContext->timeSigDenominator;
      context.play_state    = PLUGIN_PLAYSTATE_NONE;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kPlaying)      context.play_state |= PLUGIN_PLAYSTATE_PLAYING;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kRecording)    context.play_state |= PLUGIN_PLAYSTATE_RECORDING;
      if (data.processContext->state & ProcessContext::StatesAndFlags::kCycleActive)  context.play_state |= PLUGIN_PLAYSTATE_LOOPING;
      on_process(&context);
    }
    return kResultOk;
  }

  uint32 PLUGIN_API getTailSamples() final {
    return kNoTail;
  }

  //--------------------
  // IMidiMapping
  //--------------------

  tresult PLUGIN_API getMidiControllerAssignment(int32 busIndex, int16 channel, CtrlNumber midiControllerNumber, ParamID& id) final {
    //if (busIndex == 0) {
      switch (midiControllerNumber) {
        case kAfterTouch: // 128
          id = VST3_PARAM_AFTERTOUCH + channel;
          return kResultOk;
        case kPitchBend: // 129
          id = VST3_PARAM_PITCHBEND + channel;
          return kResultOk;
        case kCtrlFilterResonance: // cc 74 (slide)
          id = VST3_PARAM_BRIGHTNESS + channel;
          return kResultOk;
      }
    //}
    return kResultFalse;
  }

  //--------------------
  // INoteExpressionController
  //--------------------

//  tresult PLUGIN_API getNoteExpressionInfo(int32 busIndex, int16 channel, int32 noteExpressionIndex, NoteExpressionTypeInfo& info) final {
//    VST3_Trace("vst3: instance/getNoteExpressionInfo busIndex:%i channel:%i noteExpressionIndex:%i\n",busIndex,channel,noteExpressionIndex);
//    //if (busIndex==0) {
//      switch(noteExpressionIndex) {
//        case 0:
//          info.typeId                 = 0; // ??
//          char_to_utf16("Tuning",info.title);
//          char_to_utf16("Tun",info.shortTitle);
//          char_to_utf16("",info.units);
//          info.unitId                 = 0;
//          info.valueDesc.defaultValue = 0.0;
//          info.valueDesc.minimum      = 0;
//          info.valueDesc.maximum      = 1;
//          info.associatedParameterId  = -1;
//          info.flags                  = NoteExpressionTypeInfo::kIsBipolar;
//          return kResultOk;
//        case 1:
//          info.typeId                 = 1; // ??
//          char_to_utf16("Brightness",info.title);
//          char_to_utf16("Bri",info.shortTitle);
//          char_to_utf16("",info.units);
//          info.unitId                 = 0;
//          info.valueDesc.defaultValue = 0.0;
//          info.valueDesc.minimum      = 0;
//          info.valueDesc.maximum      = 1;
//          info.associatedParameterId  = -1;
//          info.flags                  = 0;
//          return kResultOk;
//      }
//    //}
//    return kResultFalse;
//  }

//  tresult PLUGIN_API getNoteExpressionStringByValue(int32 busIndex, int16 channel, NoteExpressionTypeID id, NoteExpressionValue valueNormalized, String128 string) final {
//    char temp[100];
//    FloatToString(temp,valueNormalized);
//    char_to_utf16(temp,string);
//    return kResultOk;
//  }

//  tresult PLUGIN_API getNoteExpressionValueByString(int32 busIndex, int16 channel, NoteExpressionTypeID id, const TChar* string, NoteExpressionValue& valueNormalized) final{
//    char temp[129];
//    utf16_to_char(string,temp);
//    float res = const char*ToFloat(temp);
//    valueNormalized = res;
//    return kResultOk;
//  }

  //--------------------
  // IKeyswitchController
  //--------------------

  int32 PLUGIN_API getKeyswitchCount(int32 busIndex, int16 channel) final {
    return 0;
  }

  tresult PLUGIN_API getKeyswitchInfo(int32 busIndex, int16 channel, int32 keySwitchIndex, KeyswitchInfo& info) final {
    return kResultFalse;
  }

  //--------------------
  // IConnectionPoint
  //--------------------

  tresult PLUGIN_API connect(IConnectionPoint* other) final {
    //IMessage* msg;
    //msg->setMessageID("test");
    //other->notify(msg);
    return kResultFalse;
  }

  tresult PLUGIN_API disconnect(IConnectionPoint* other)  final {
    return kResultFalse;
  }

  tresult PLUGIN_API notify(IMessage* message) final {
    return kResultFalse;
  }

  //--------------------
  // IUnitInfo
  //--------------------

  int32 PLUGIN_API getUnitCount() final {
    return 1;
  }

  tresult PLUGIN_API getUnitInfo(int32 unitIndex, UnitInfo& info) final {
    if (unitIndex==0) {
      info.id = kRootUnitId;
      info.parentUnitId = kNoParentUnitId;
      /*CharToUtf16*/ char_to_utf16("root",&info.name);
      info.programListId = kNoProgramListId;
      return kResultOk;
    }
    return kResultFalse;
  }

  int32 PLUGIN_API getProgramListCount() final {
    return 0; // 1
  }

  tresult PLUGIN_API getProgramListInfo(int32 listIndex, ProgramListInfo& info) final {
    if (listIndex == 0) {
      info.id = 0;
      /*CharToUtf16*/ char_to_utf16("program",&info.name);
      info.programCount = 1;
      return kResultOk;
    }
    return kResultFalse;
  }

  tresult PLUGIN_API getProgramName(ProgramListID listId, int32 programIndex, String128 name) final {
    if ((listId == 0) && (programIndex == 0)) {
      /*CharToUtf16*/ char_to_utf16("program",&name);
      return kResultOk;
    }
    return kResultFalse;
  }

  tresult PLUGIN_API getProgramInfo(ProgramListID listId, int32 programIndex, CString attributeId, String128 attributeValue) final {
    ////attributeId = "";
    //if ((listId == 0) && (programIndex == 0) /* attributeId */) {
    //  /*CharToUtf16*/ char_to_utf16("",attributeValue);
    //  return kResultOk;
    //}
    return kResultFalse;
  }

  tresult PLUGIN_API hasProgramPitchNames(ProgramListID listId, int32 programIndex) final {
    return kResultFalse;
  }

  tresult PLUGIN_API getProgramPitchName(ProgramListID listId, int32 programIndex, int16 midiPitch, String128 name) final {
    // /*CharToUtf16*/ char_to_utf16("pitch",name);
    return kResultFalse;
  }

  UnitID PLUGIN_API getSelectedUnit() final {
    return 0;
  }

  tresult PLUGIN_API selectUnit(UnitID unitId) final {
    return kResultOk;
  }

  tresult PLUGIN_API getUnitByBus(MediaType type, BusDirection dir, int32 busIndex, int32 channel, UnitID& unitId) final {
    unitId = 0;
    return kResultOk;
  }

  tresult PLUGIN_API setUnitProgramData(int32 listOrUnitId, int32 programIndex, IBStream* data) final {
    return kResultFalse;
  }

  //--------------------
  // IEditController
  //--------------------

  tresult PLUGIN_API setComponentState(IBStream* state) final {
    return kResultOk;
  }


  tresult PLUGIN_API setEditorState(IBStream* state) final {
    return kResultOk;
  }

  tresult PLUGIN_API getEditorState(IBStream* state) final {
    return kResultOk;
  }

  int32 PLUGIN_API getParameterCount() final {
    return MDescriptor->parameters.size();
  }

  tresult PLUGIN_API getParameterInfo(int32 paramIndex, ParameterInfo& info) final {
    if (paramIndex >= 0) {
      if (paramIndex < (int32)MDescriptor->parameters.size()) {
        //VST3_Parameter* param = MDescriptor->getParameter(paramIndex);
        //if (param) {
          memcpy(&info,&MParamInfos[paramIndex],sizeof(ParameterInfo));
          return kResultOk;
        //}
      } // index < numparams
      else {
        switch (paramIndex) {
          case kAfterTouch: // 128
            break;
          case kPitchBend: // 129
            break;
          case kCtrlFilterResonance: // cc 74 (slide)
            break;
        }
        switch (paramIndex & 0xffff0000) {
          case VST3_PARAM_AFTERTOUCH:
            break;
          case VST3_PARAM_PITCHBEND:
            break;
          case VST3_PARAM_BRIGHTNESS:
            break;
        }
        return kResultFalse;
      }
    }
    return kResultFalse;
  }

  tresult PLUGIN_API getParamStringByValue(ParamID id, ParamValue valueNormalized, String128 string) final {
    if (id < MDescriptor->parameters.size()) {
      char temp[129]; // ???
      VST3_Parameter* param = MDescriptor->parameters[id];
      param->displayText(valueNormalized,temp);
      /*CharToUtf16*/ char_to_utf16(temp,string);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  tresult PLUGIN_API getParamValueByString(ParamID id, TChar* string, ParamValue& valueNormalized) final {
    if (id < MDescriptor->parameters.size()) {
      char temp[129];
      utf16_to_char(string,temp);
      float v = atoi(temp);
      VST3_Parameter* param = MDescriptor->parameters[id];
      float v2 = param->to01(v);
      valueNormalized = v2;
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  ParamValue PLUGIN_API normalizedParamToPlain(ParamID id, ParamValue valueNormalized) final {
    if (id < MDescriptor->parameters.size()) {
      VST3_Parameter* param = MDescriptor->parameters[id];
      float v = param->from01(valueNormalized);
      return v;
    }
    else {
      return valueNormalized;
    }
  }

  ParamValue PLUGIN_API plainParamToNormalized(ParamID id, ParamValue plainValue) final {
    if (id < MDescriptor->parameters.size()) {
      VST3_Parameter* param = MDescriptor->parameters[id];
      float v = param->to01(plainValue);
      return v;
    }
    else {
      return plainValue;
    }
  }

  ParamValue PLUGIN_API getParamNormalized(ParamID id) final {
    if (id < MDescriptor->parameters.size()) {
      float v = MParameterValues[id];
      return v;
    }
    else {
      return 0;
    }
  }

  tresult PLUGIN_API setParamNormalized(ParamID id, ParamValue value) final {
    if (id >= MDescriptor->parameters.size()) {
      return kResultFalse; // ???
    }
    MParameterValues[id] = value;
    if (MEditor) {
      MEditor->updateParameterFromHost(id,value);
    }
    return kResultOk;
  }

  tresult PLUGIN_API setComponentHandler(IComponentHandler* handler) final {
    MComponentHandler = handler;
    return kResultOk;
  }

  IPlugView* PLUGIN_API createView(FIDString name) final {
    if (MDescriptor->has_editor) {
      if (name && (strcmp(name,/*ViewType::*/kEditor) == 0)) {
        addRef();
        return (IPlugView*)this;
      }
    }
    return nullptr;
  }

  //--------------------
  // IEditController2
  //--------------------

  tresult PLUGIN_API setKnobMode(KnobMode mode) final {
    return kResultFalse;
  }

  tresult PLUGIN_API openHelp(TBool onlyCheck) final {
    return kResultFalse;
  }

  tresult PLUGIN_API openAboutBox(TBool onlyCheck) final {
    return kResultFalse;
  }

  //--------------------
  // IPlugView
  //--------------------

  tresult PLUGIN_API isPlatformTypeSupported(FIDString type) final {
    // "X11EmbedWindowID"
    #ifdef VST3_LINUX
    if (type && strcmp(type,kPlatformTypeX11EmbedWindowID) == 0) {
      return kResultOk;
    }
    #endif // VST3_LINUX
    return kResultFalse;
  }
  tresult PLUGIN_API attached(void* parent, FIDString type) final {
    #ifndef VST3_NO_GUI
      if (MDescriptor->has_editor) {
        if (MPlugFrame) {
          uint32_t w = MDescriptor->editor_width;
          uint32_t h = MDescriptor->editor_height;
          ViewRect r;
          r.left    = 0;
          r.top     = 0;
          r.right   = w;
          r.bottom  = h;
          MPlugFrame->resizeView(this,&r);
        }
        MEditor = on_openEditor(parent);
        //if (MRunLoop)
        MRunLoop->registerTimer(this,VST3_TIMER_MS);
        return kResultOk;
      }
    #endif // VST3_NO_GUI
    return kResultFalse;
  }

  tresult PLUGIN_API removed() final {
      #ifndef VST3_NO_GUI
      if (MDescriptor->has_editor) {
        //if (MRunLoop)
        MRunLoop->unregisterTimer(this);
        on_closeEditor(MEditor);
        MEditor = nullptr;
        return kResultOk;
      }
      #endif // VST3_NO_GUI
    return kResultFalse;
  }

  tresult PLUGIN_API onWheel(float distance) final {
    return kResultOk;
  }

  tresult PLUGIN_API onKeyDown(char16 key, int16 keyCode, int16 modifiers) final {
    return kResultOk;
  }

  tresult PLUGIN_API onKeyUp(char16 key, int16 keyCode, int16 modifiers) final {
    return kResultOk;
  }

  tresult PLUGIN_API getSize(ViewRect* size) final {
    if (MDescriptor->has_editor) {
      size->left    = 0;
      size->top     = 0;
      size->right   = MDescriptor->editor_width;
      size->bottom  = MDescriptor->editor_height;
      return kResultOk;
    }
    return kResultFalse;
  }

  tresult PLUGIN_API onSize(ViewRect* newSize) final {
    if (MDescriptor->has_editor) {
      //TODO: resize/redraw editor
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  tresult PLUGIN_API onFocus(TBool state) final {
    return kResultOk;
  }

  tresult PLUGIN_API setFrame(IPlugFrame* frame) final {
    if (MDescriptor->has_editor) {
      MPlugFrame = frame;
      MPlugFrame->queryInterface(IRunLoop_iid, (void**)&MRunLoop);
      return kResultOk;
    }
    else {
      return kResultFalse;
    }
  }

  tresult PLUGIN_API canResize() final {
    return kResultFalse;
  }

  tresult PLUGIN_API checkSizeConstraint(ViewRect* rect) final {
    return kResultOk;
  }

  //--------------------
  // ITimerHandler
  //--------------------

  void onTimer() final {
    #ifndef VST3_NO_GUI
    on_updateEditor(MEditor);
    _flushParametersToHost();
    #endif
  }

};

//----------------------------------------------------------------------
//
// plugin
//
//----------------------------------------------------------------------

template <class DESC, class INST>
class VST3_Plugin
: public IPluginFactory3 {

//------------------------------
private:
//------------------------------

  uint32_t  MRefCount     = 1;
  FUnknown* MHostContext  = nullptr;
  DESC      MDescriptor;

//------------------------------
public:
//------------------------------

  VST3_Plugin() {
    MRefCount = 1;
    printf("VST3_Plugin\n");
  }

  virtual ~VST3_Plugin() {
    printf("~VST3_Plugin\n");
  }

//------------------------------
public:
//------------------------------

  //--------------------
  // FUnknown
  //--------------------

  uint32 PLUGIN_API addRef() final {
    printf("VST3_Plugin.addRef\n");
    MRefCount++;
    return MRefCount;
  }

  uint32 PLUGIN_API release() final {
    printf("VST3_Plugin.release\n");
    const uint32_t r = --MRefCount;
    if (r == 0) delete this;
    return r;
  }

  tresult PLUGIN_API queryInterface(const TUID _iid, void** obj) final {
    printf("VST3_Plugin.queryInterface\n");
    if (/*FUnknownPrivate::*/iidEqual(IPluginFactory2_iid,_iid)) {
      *obj = (IPluginFactory2*)this;
      addRef();
      return kResultOk;
    }
    if (/*FUnknownPrivate::*/iidEqual(IPluginFactory3_iid,_iid)) {
      *obj = (IPluginFactory3*)this;
      addRef();
      return kResultOk;
    }
    *obj = nullptr;
    return kNoInterface;
  }

  //--------------------
  // IPluginFactory
  //--------------------

  tresult PLUGIN_API getFactoryInfo(PFactoryInfo* info) final {
    printf("VST3_Plugin.getFactoryInfo\n");
    strcpy(info->vendor,MDescriptor.author);
    strcpy(info->url,MDescriptor.url);
    strcpy(info->email,MDescriptor.email);
    info->flags = PFactoryInfo::kNoFlags;
    return kResultOk;
  }

  int32 PLUGIN_API countClasses() final {
    printf("VST3_Plugin.countClasses\n");
    return 1;
  }

  tresult PLUGIN_API getClassInfo(int32 index, PClassInfo* info) final {
    printf("VST3_Plugin.getClassInfo\n");
    switch (index) {
      case 0:
        memcpy(info->cid,MDescriptor.plugin_id,16);
        info->cardinality = PClassInfo::kManyInstances;
        strncpy(info->category,kVstAudioEffectClass,PClassInfo::kCategorySize);
        strncpy(info->name,MDescriptor.name,PClassInfo::kNameSize);
        return kResultOk;
    }
    return kResultFalse;
  }

  tresult PLUGIN_API createInstance(FIDString cid, FIDString _iid, void** obj) final {
    printf("VST3_Plugin.createInstance\n");
    if (/*FUnknownPrivate::*/iidEqual(MDescriptor.plugin_id,cid)) {
      INST* instance = new INST(/*&MDescriptor*/);
      instance->_setDescriptor(&MDescriptor);
      instance->on_create();
      instance->_setDefaultParameterValues();
      instance->_updateAllParameters();
      *obj = (/*Vst::*/IComponent*)instance;
      return kResultOk;
    }
    *obj = nullptr;
    return kNotImplemented;
  }

  //--------------------
  // IPluginFactory2
  //--------------------

  tresult PLUGIN_API getClassInfo2(int32 index, PClassInfo2* info) final {
    printf("VST3_Plugin.getClassInfo2\n");
    switch (index) {
      case 0:
        memcpy(info->cid,MDescriptor.plugin_id,16);
        info->cardinality = PClassInfo::kManyInstances;
        strcpy(info->category,kVstAudioEffectClass);
        strcpy(info->name,MDescriptor.name);
        info->classFlags = 0;
        if (MDescriptor.is_synth) {
          strcpy(info->subCategories,/*Vst::PlugType::*/kInstrument);
        }
        else {
          strcpy(info->subCategories,/*Vst::PlugType::*/kFx);
        }
        strcpy(info->vendor,MDescriptor.author);
        strcpy(info->version,MDescriptor.version_text);
        strcpy(info->sdkVersion,kVstVersionString);
        return kResultOk;
    }
    return kResultFalse;
  }

  //--------------------
  // IPluginFactory3
  //--------------------

  tresult PLUGIN_API getClassInfoUnicode(int32 index, PClassInfoW* info) final {
    printf("VST3_Plugin.getClassInfoUnicode\n");
    return kResultFalse;
  }

  tresult PLUGIN_API setHostContext(FUnknown* context) final {
    printf("VST3_Plugin.setHostContext\n");
    MHostContext = context;
    return kResultOk;
  }

};

//----------------------------------------------------------------------
//
// entrypoint
//
//----------------------------------------------------------------------

//#ifdef VST3_LINUX
#ifdef __gnu_linux__
  #define __VST3_DLLEXPORT __attribute__ ((visibility ("default")))
#endif

#ifdef _WIN32
  #define __VST3_DLLEXPORT __attribute__ ((dllexport))
#endif

//----------

#define VST3_MAIN_SYMBOL  asm ("GetPluginFactory");
IPluginFactory* PLUGIN_API vst3_entrypoint() VST3_MAIN_SYMBOL

#define VST3_ENTRYPOINT(DESC,INST)                \
                                                  \
  __VST3_DLLEXPORT                                \
  IPluginFactory* PLUGIN_API vst3_entrypoint() {  \
    printf("GetPluginFactory\n");                 \
    return new VST3_Plugin<DESC,INST>();          \
  }

//----------------------------------------------------------------------
#endif
